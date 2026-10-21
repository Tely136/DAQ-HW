#include <stdio.h>
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

int tcp_server(uint32_t* counts, int n_bins)
{
    // listen on sock_fd, new connection on new_fd
    int new_fd;
    struct sockaddr_storage their_addr; // connector's address info
    socklen_t sin_size;
    int rv;

    new_fd = tcp_listen(atoi(PORT));        

        // char buff[1024];
        // rv = read(new_fd, buff, sizeof(buff));
        // if (rv == -1) {
        //     perror("read");
        // }

        // char *msg = "OK";
        // rv = write(new_fd, msg, strlen(msg));
        // if (rv == -1) {
        //     perror("write");
        // }

        int sent = 0;
        while (sent < n_bins * sizeof(uint32_t)) {
            rv = write(new_fd, ((char*)counts) + sent, n_bins * sizeof(uint32_t) - sent);
            if (rv == -1) {
                perror("write");
                break;
            }
            sent += rv;
    

        fprintf(stdout, "server: sent %d bytes\n", sent);

        close(new_fd);
        }
        // fprintf(stdout, "server: received %d bytes: %s\n", rv, buff);
    

    return 0;
}