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
#include "xmulti_channel_scaler.h"

#include "params.h"
#include "daq.h"
#include "daq_tcpip.h"


int main() {
    int status;
    struct daq_device daq;
    struct daq_config config;  
    char buf[1024];

    daq.configured = 0;

    // initialize device
    status = daq_open(&daq);
    if (status != 0) {
        fprintf(stderr, "Failed to open DAQ\n");
        return 1;
    }

    // wait for client to connect and return socket file descriptor
    int sockfd = tcp_listen(atoi(PORT));
    if (sockfd < 0) {
        return 1; // need to look at this
    }

    // send ready message to client
    status = send(sockfd, "READY\n", 6, 0);
    if (status == -1) {
        perror("write");
        return 1;
    }

    // begin loop
    while(1) {

        // receive command from client
        status = recv_line(sockfd, buf, sizeof(buf));
        if (status == -1) {
            perror("recv_line");
            return 1;
        } else if (status == 0) {
            fprintf(stderr, "Client disconnected\n");
            return 1;
        }

        // handle config message
        if (strstr(buf, "CONFIG") != NULL) {
            status = daq_configure(&daq, &config, buf);
            if (status != 0) {
                fprintf(stderr, "Failed to configure DAQ\n");
                return 1;
            }

            printf("Configure successful\n");
            continue;
        }

        // handle start acquisition message
        if (strstr(buf, "START") != NULL) {

            // check if already configured
            if (!daq.configured) {
                printf("DAQ not configured\n");
                continue;
            }
            else {

                // if configured, run the acquisition
                uint32_t counts[config.n_bins];
                status = aquire_test(&daq, &config, counts);
                if (status != 0) {
                    fprintf(stderr, "DAQ acquisition failed with status %d\n", status);
                    // return status;  
                }

                // send result to client
                send_all(sockfd, counts, config.n_bins * sizeof(*counts));
                continue;
            }
        }

        // handle end message
        if (strstr(buf, "END") != NULL) {
            break;
        }
    }

    return 0;
}
