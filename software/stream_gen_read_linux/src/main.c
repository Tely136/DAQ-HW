#include <fcntl.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <string.h>

#include "xtrigger.h"
#include "xstream_generator.h"
#include "xstream_reader.h"

#include "params.h"
#include "daq.h"
#include "daq_tcpip.h"


int main() {
    int status;
    struct daq_device daq;
    struct daq_config config = {
        .n_bins = N_BINS,
        .n_clk = N_CLK,
        .n_shot = N_SHOT
    };  
    char buf[1024];

    daq.configured = 0;

    // setup code here
    daq_open(&daq);

    // wait for client to connect and return socket file descriptor
    int sockfd = tcp_listen(atoi(PORT));
    if (sockfd < 0) {
        return 1; // need to look at this
    }

    status = send(sockfd, "READY\n", 6, 0);
    if (status == -1) {
        perror("write");
        return 1;
    }

    while(1) {
        status = recv_line(sockfd, buf, sizeof(buf));

        if (status == -1) {
            perror("recv_line");
            return 1;
        } else if (status == 0) {
            fprintf(stderr, "Client disconnected\n");
            return 1;
        }

        if (strstr(buf, "CONFIG") != NULL) {
            status = daq_configure(&daq, &config, buf);
            if (status != 0) {
                fprintf(stderr, "Failed to configure DAQ\n");
                return 1;
            }

            printf("Configure successful\n");
            continue;
        }

        if (strstr(buf, "START") != NULL) {
            if (!daq.configured) {
                printf("DAQ not configured\n");
                continue;
            }
            else {
                uint32_t counts[config.n_bins];
                status = aquire_test(&daq, &config, counts);
                if (status != 0) {
                    fprintf(stderr, "DAQ acquisition failed with status %d\n", status);
                    return status;  
                }

                send_all(sockfd, counts, config.n_bins * sizeof(*counts));
                continue;
            }
        }

        if (strstr(buf, "END") != NULL) {
            break;
        }
    }

    return 0;
}
