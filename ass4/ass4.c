/**
 *   Linux 6.6.47-1-MANJARO (roughero) 	13/09/24 	_x86_64_	(8 CPU)
 *
 *   08:52:48 PM IST  pgpgin/s pgpgout/s   fault/s  majflt/s  pgfree/s pgscank/s pgscand/s pgsteal/s  pgprom/s   pgdem/s
 *   08:52:49 PM IST      8.00    112.00   4239.00      0.00  26506.00      0.00      0.00      0.00      0.00      0.00
 *   08:52:50 PM IST      0.00    260.00   1640.00      0.00  46028.00      0.00      0.00      0.00      0.00      0.00
 *   08:52:51 PM IST      0.00    208.00    242.00      0.00   2056.00      0.00      0.00      0.00      0.00      0.00
 *   08:52:52 PM IST      0.00    256.00   9424.00    852.00  30197.00      0.00      0.00      0.00      0.00      0.00
 *   08:52:53 PM IST      0.00      0.00 232751.00  22792.00  27341.00      0.00      0.00      0.00      0.00      0.00
 *   08:52:54 PM IST      0.00 783628.00 231562.00   8720.00  15768.00      0.00      0.00      0.00      0.00      0.00
 *   08:52:55 PM IST      0.00 890068.00 215535.00   3710.00  34719.00   9363.00      0.00  15834.00      0.00      0.00
 *   08:52:56 PM IST      0.00 938124.00 227453.00   2241.00  32855.00   6510.00      0.00  12644.00      0.00      0.00
 *   08:52:57 PM IST     56.00 1033424.00 236688.00   1325.00  39674.00   5623.00      0.00  10062.00      0.00      0.00
 *   08:52:58 PM IST      0.00 893264.00 246764.00    922.00  24462.00      0.00      0.00      0.00      0.00      0.00
 *   08:52:59 PM IST      0.00 1149052.00 230996.00    590.00  35517.00   5312.00      0.00   9246.00      0.00      0.00
 *   08:53:00 PM IST      0.00 837212.00 267206.00    448.00  26004.00      0.00      0.00      0.00      0.00      0.00
 *   08:53:01 PM IST      0.00 401408.00  56588.00     83.00  33152.00      0.00      0.00      0.00      0.00      0.00
 *   08:53:02 PM IST      8.00  28952.00   3375.00      0.00  76253.00  26560.00   4235.00  57088.00      0.00      0.00
 *   08:53:03 PM IST      0.00      0.00    567.00      0.00   1869.00      0.00      0.00      0.00      0.00      0.00
 *   08:53:04 PM IST      0.00      0.00    136.00      0.00   1165.00      0.00      0.00      0.00      0.00      0.00
 *
 *
 *   Average:         4.50 434748.00 122822.88   2605.19  28347.88   3335.50    264.69   6554.62      0.00      0.00
 */

#define _GNU_SOURCE   // for fallocate

#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>

#include <sys/mman.h>
#include <unistd.h>
#include <time.h>
#include <stdint.h>

#define FILE_SIZE (8L * 1024 * 1024 * 1024)   // 8GB

unsigned char get_random_byte()
{
    return rand() % 256;
}

off_t get_random_offset()
{
    return ((off_t)rand() << 31 | rand()) % FILE_SIZE;
}

int main()
{
    char const* filename = "bigfile";
    int fd;

    srand(time(NULL));

    fd = open(filename, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd < 0) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    if (fallocate(fd, 0, 0, FILE_SIZE) < 0) {
        perror("fallocate");
        close(fd);
        exit(EXIT_FAILURE);
    }

    void* mapped_file = mmap(NULL, FILE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); /* map the file denoted by `fd` to memery */
    if (mapped_file == MAP_FAILED) {
        perror("mmap");
        close(fd);
        exit(EXIT_FAILURE);
    }

    while (1) {
        unsigned char X = get_random_byte();
        off_t F         = get_random_offset();

        *((unsigned char*)(mapped_file + F)) = X;

        unsigned char X_prime = *((unsigned char*)(mapped_file + F));

        if (X != X_prime) {
            printf("Verification failed at offset %#lx: wrote %#x, read 0x%x\n", F, X, X_prime);
            munmap(mapped_file, FILE_SIZE);
            close(fd);
            exit(EXIT_FAILURE);
        }

        printf("Verified: offset %#lx, value %#x\n", F, X);
    }

    munmap(mapped_file, FILE_SIZE);
    close(fd);
}
