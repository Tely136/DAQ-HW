#include <fcntl.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <string.h>

#include "params.h"
#include "daq.h"
#include "daq_tcpip.h"


int main() {
    int status = 1;
    int sockfd = -1;
    struct daq_device daq = {0};
    struct daq_config config = {0};
    char buf[1024];
    daq.irq_fd = -1;

    if (daq_open(&daq) != 0) {
        fprintf(stderr, "Failed to open DAQ\n");
        goto cleanup;
    }

    sockfd = tcp_listen(PORT);
    if (sockfd < 0 || send_all(sockfd, "READY\n", 6) != 0) {
        goto cleanup;
    }

    while (1) {
        int received = recv_line(sockfd, buf, sizeof(buf));
        if (received <= 0) {
            fprintf(stderr, "Client disconnected or receive failed\n");
            goto cleanup;
        }

        if (strncmp(buf, "CONFIG ", 7) == 0) {
            if (daq_configure(&daq, &config, buf) != 0) {
                goto cleanup;
            }
            printf("Configuration successful\n");
            continue;
        }

        if (strcmp(buf, "START") == 0) {
            if (!daq.configured) {
                fprintf(stderr, "DAQ not configured\n");
                goto cleanup;
            }

            uint32_t counts[MAX_BINS];
            int result = daq_acquire(&daq, &config, counts);
            if (result < 0) {
                goto cleanup;
            }
            // completed data is still useful when the self-test fails
            if (send_all(sockfd, counts, config.n_bins * sizeof(*counts)) != 0) {
                goto cleanup;
            }
            continue;
        }

        if (strcmp(buf, "END") == 0) {
            status = 0;
            break;
        }

        fprintf(stderr, "Unknown command: %s\n", buf);
        goto cleanup;
    }

cleanup:
    if (sockfd >= 0) {
        close(sockfd);
    }
    daq_close(&daq);
    return status;
}