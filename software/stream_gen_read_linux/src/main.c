#include <fcntl.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>

#include "xtrigger.h"
#include "xstream_generator.h"
#include "xstream_reader.h"

#include "params.h"
#include "daq.h"
#include "daq_tcpip.h"


int main() {
    struct daq_device daq;
    struct daq_config config = {
        .n_bins = N_BINS,
        .n_clk = N_CLK,
        .avg_shot = AVG_SHOT
    };  

    // setup code here
    daq_open(&daq);



    /* Do not reconfigure an acquisition left running by an earlier process. */
    if (!XStream_reader_IsIdle(&daq.reader)) {
        fprintf(stderr, "Reader is still running; wait for it to finish before retrying.\n");
        return 1;
    }
    XStream_reader_DisableAutoRestart(&daq.reader);




    // wait for client to connect and return socket file descriptor
    int sockfd = tcp_listen(atoi(PORT));
    if (sockfd < 0) {
        goto cleanup; // need to look at this
    }


    daq_configure(&daq, &config);
    


    XStream_reader_InterruptEnable(&daq.reader, 0x1);  // ap_done
    XStream_reader_InterruptGlobalEnable(&daq.reader);

    

cleanup:
    /* This program performs one acquisition. The next run re-arms UIO. */
    XStream_reader_InterruptGlobalDisable(&daq.reader);
    XStream_reader_InterruptDisable(&daq.reader, 0x3);
    XStream_reader_InterruptClear(&daq.reader,
        XStream_reader_InterruptGetStatus(&daq.reader));
        
    close(irq_fd);

    daq_close(&daq);

    printf("%s\n", exit_status == 0 ? "Run Finished Successfully." : "Run failed.");
    return exit_status;
}
