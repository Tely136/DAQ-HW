#ifndef DAQ_TCPIP_H
#define DAQ_TCPIP_H

#include <stdio.h>
#include <stdint.h>
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

// #define IP_ADDRESS "localhost"  // the IP address to bind to
#define IP_ADDRESS "192.168.137.20"  // the IP address to bind to

#define PORT "8080"  // the port users will be connecting to

#define BACKLOG 1   // how many pending connections queue will hold

/* Blocking sockets only. These helpers do not convert payload byte order.
 * On a transfer error, some bytes may already have been consumed/sent;
 * close the connection rather than retrying the whole message.
 */
static inline int send_all(int fd, const void *data, size_t bytes)
{
    const unsigned char *buffer = data;
    size_t sent = 0;

    while (sent < bytes) {
        size_t remaining = bytes - sent;
        size_t chunk = remaining > (size_t)SSIZE_MAX
                     ? (size_t)SSIZE_MAX : remaining;
        ssize_t n = send(fd, buffer + sent, chunk, MSG_NOSIGNAL);
        if (n < 0) {
            if (errno == EINTR)
                continue;
            return -1;
        }
        if (n == 0) {
            errno = EIO;
            return -1;
        }
        sent += (size_t)n;
    }
    return 0;
}

/* Returns 1 for a complete payload, 0 for EOF before any payload bytes,
 * or -1 for an error (including EOF partway through: ECONNRESET).
 * A zero-byte request succeeds immediately.
 */
static inline int recv_exact(int fd, void *data, size_t bytes)
{
    unsigned char *buffer = data;
    size_t received = 0;

    while (received < bytes) {
        size_t remaining = bytes - received;
        size_t chunk = remaining > (size_t)SSIZE_MAX
                     ? (size_t)SSIZE_MAX : remaining;
        ssize_t n = recv(fd, buffer + received, chunk, 0);
        if (n < 0) {
            if (errno == EINTR)
                continue;
            return -1;
        }
        if (n == 0) {
            if (received == 0)
                return 0;
            errno = ECONNRESET;
            return -1;
        }
        received += (size_t)n;
    }
    return 1;
}

/* Reads an LF-terminated text command, removes LF, and NUL-terminates it.
 * Capacity includes the final NUL. Empty lines are valid.
 * Returns 1 for a line, 0 for EOF before any bytes, or -1 with errno set.
 * Embedded NUL is rejected (EPROTO); oversized lines fail with EMSGSIZE.
 * Close the connection after errors: an unfinished line may remain unread.
 * Reading one byte at a time deliberately leaves subsequent lines/binary
 * payloads in the socket, so this can be mixed safely with recv_exact().
 */
static inline int recv_line(int fd, char *line, size_t capacity)
{
    size_t used = 0;

    if (line == NULL || capacity == 0) {
        errno = EINVAL;
        return -1;
    }
    line[0] = '\0';

    for (;;) {
        unsigned char c;
        int result = recv_exact(fd, &c, 1);
        if (result < 0)
            return -1;
        if (result == 0) {
            if (used == 0)
                return 0;
            errno = ECONNRESET;
            return -1;
        }
        if (c == '\n')
            return 1;
        if (c == '\0') {
            errno = EPROTO;
            return -1;
        }
        if (used == capacity - 1) {
            errno = EMSGSIZE;
            return -1;
        }
        line[used++] = (char)c;
        line[used] = '\0';
    }
}

void sigchld_handler(int s)
{
    (void)s; // quiet unused variable warning

    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;

    while(waitpid(-1, NULL, WNOHANG) > 0);

    errno = saved_errno;
}

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int tcp_listen(int port) {
    int sockfd, new_fd;
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr; // connector's address info
    struct sigaction sa;
    socklen_t sin_size;
    int yes=1;
    int rv;
    char s[INET6_ADDRSTRLEN];

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    // hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(IP_ADDRESS, PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("server: bind");
            continue;
        }

        break;
    }

    freeaddrinfo(servinfo); // all done with this structure

    if (p == NULL)  {
        fprintf(stderr, "server: failed to bind\n");
        exit(1);
    }

    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }

    sa.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    printf("server: waiting for connections...\n");

    while(1) {
        sin_size = sizeof their_addr;
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr,
            &sin_size);
        if (new_fd == -1) {
            perror("accept");
            continue;
        }

        inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof s);
        printf("server: got connection from %s\n", s);
        break; // exit the loop after accepting one connection
    }

    return new_fd;
}

#endif /* DAQ_TCPIP_H */
