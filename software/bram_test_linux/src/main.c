#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

#define BRAM_ADDRESS 0x40002000
#define BRAM_SIZE    0x2000 /* 8 KiB, from the hardware address map. */
#define TEST_WORDS   4

typedef uint32_t addr_t;

int main() {

    int fd = open("/dev/mem", O_RDWR | O_SYNC);
    if (fd < 0) {
        perror("failed to open /dev/mem");
        return 1;
    }

    void* mapping = mmap(NULL, BRAM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, BRAM_ADDRESS);
    if (mapping == MAP_FAILED) {
        perror("mmap failed");
        close(fd);
        return 1;
    }
    close(fd);

    volatile addr_t* bram = mapping;

    for (int i=0; i<TEST_WORDS; i++) {
        bram[i] = i+1;
    }

    int result = 0;
    for (int i=0; i<TEST_WORDS; i++) {
        if ((int)bram[i] != i+1) {
            result = 1;
        }
    }

    munmap(mapping, BRAM_SIZE);
    puts(result == 0 ? "BRAM test passed" : "BRAM test failed");
    return result;
}