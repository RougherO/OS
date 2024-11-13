/***********************************************************************
 *
 * Name: Chirag Chowdhury
 * Roll: 002211001095
 *
 * Date: 31.08.2024
 *
 * Assignment: 4, mmap & page fault
 *
 *      Objective:
 *          1. Craete a big file (about 8GB) using ‘fallocate’.
 *          2. Using mmap() call, map the file in the virtual address space.
 *          3. This file should be written with a single byte value (say X) at a specific offset (say F). X (between 0-255) & F (between 0 and 8G) are genearted using random function.
 *          4. Now, read data (say X`) from from the same specific offset (F) then verify that X and X` are the same.
 *          5. In case of verification failure, an error message is to be printed and also the program should terminate.
 *          6. Repeat steps 3 to 5 indefinitely
 *          7. In another terminal execute the command ‘sar –B 1’ (at 1 second gap) to observe for the page fault.
 *             Start the comamnd before program execution. So, one can observe that the page faults are increasing.
 *
 *        (Display the offset as hex number format; comment the output of the 'sar' command)
 *
 * Input Description: None
 *
 * Output Description:
 *
 *
 * Compilation Command: gcc 1095_4.c
 * Execution Sequence: ./a.out
 *
 *
 * Sample Input:
 * Sample Output:
 /-----------------------------------
File created: somewhat-big-file

Success: Offset 0x0037a371, Value 0x11
Success: Offset 0x00004cb9, Value 0xfa
Success: Offset 0x004fede4, Value 0xb7
Success: Offset 0x000fdf96, Value 0x30
Success: Offset 0x0020ba23, Value 0xf3
Success: Offset 0x0017a268, Value 0x2d
Success: Offset 0x003872c9, Value 0xfd
Success: Offset 0x001f7e1b, Value 0x1c
Success: Offset 0x00197f07, Value 0x43
Success: Offset 0x0077bf80, Value 0x73
Success: Offset 0x00684b58, Value 0xda
Success: Offset 0x000829d7, Value 0xdd
^C
 -----------------------------------/
 *
 *
 * Observe Page Faults:
 * 1. Using "sar -B 1"
 /-----------------------------------
01:38:53 PM  pgpgin/s pgpgout/s   fault/s  majflt/s  pgfree/s pgscank/s pgscand/s pgsteal/s    %vmeff
01:38:54 PM      0.00    148.00   3390.00      0.00   1334.00      0.00      0.00      0.00      0.00
01:38:55 PM      0.00    112.00   1745.00      0.00   1289.00      0.00      0.00      0.00      0.00
01:38:56 PM      0.00    152.00   1563.00      0.00   1017.00      0.00      0.00      0.00      0.00
01:38:57 PM      0.00      0.00  95549.00  64633.00  27364.00      0.00      0.00      0.00      0.00
01:38:58 PM      0.00    232.00 103460.00  72112.00  29190.00      0.00      0.00      0.00      0.00
01:38:59 PM      0.00    652.00 132758.00  99432.00  32883.00      0.00      0.00      0.00      0.00
01:39:00 PM      0.00  19156.00 118353.00  79068.00  37912.00      0.00      0.00      0.00      0.00
01:39:01 PM      0.00  28928.00  99249.00  49913.00  43995.00      0.00      0.00      0.00      0.00
01:39:02 PM      0.00  24748.00  78142.00  37214.00  49502.00      0.00      0.00      0.00      0.00
01:39:03 PM      0.00  45284.00  71657.00  31041.00  27691.00      0.00      0.00      0.00      0.00
01:39:04 PM      0.00  50600.00  64184.00  20766.00  47913.00      0.00      0.00      0.00      0.00
01:39:05 PM      0.00  76504.00  63484.00  17578.00  25918.00      0.00      0.00      0.00      0.00
01:39:06 PM      0.00 103772.00  57553.00  11917.00  43601.00      0.00      0.00      0.00      0.00
01:39:07 PM      0.00 104768.00  62724.00   9023.00  46446.00      0.00      0.00      0.00      0.00
01:39:08 PM      0.00 119732.00  64455.00   6959.00  26703.00      0.00      0.00      0.00      0.00
01:39:09 PM      0.00  55412.00  57402.00   4772.00  44116.00      0.00      0.00      0.00      0.00
01:39:10 PM      0.00 111348.00  57656.00   3535.00  23625.00      0.00      0.00      0.00      0.00
01:39:11 PM      0.00 111572.00  43449.00   1945.00  37718.00      0.00      0.00      0.00      0.00
01:39:12 PM      0.00  66808.00 112989.00    529.00  42332.00      0.00      0.00      0.00      0.00
01:39:13 PM      4.00  17676.00 256943.00      0.00 900315.00  28191.00   1247.00  58338.00    198.17
01:39:14 PM      0.00    788.00  49364.00      0.00  95014.00      0.00      0.00      0.00      0.00
01:39:15 PM      0.00     24.00     90.00      0.00    431.00      0.00      0.00      0.00      0.00
01:39:16 PM      0.00      0.00    166.34      0.00    159.41      0.00      0.00      0.00      0.00
01:39:17 PM      0.00      0.00   4263.00^C
      0.00   6449.00      0.00      0.00      0.00      0.00

Average:         0.17  39084.38  66663.47  21259.35  66343.98   1174.14     51.94   2429.74    198.17
 -----------------------------------/
 *
 *
 * 2. Using custom script "./page_fault_listner.sh"; to focus on just one process that is this one
 /-----------------------------------
process 'a.out' has started with pid-'114572'


time            state   minor_fault             major_fault     virtual_memory_size     resident_set_size
13:41:55         S               78              0               2707456                 288
13:41:56         S               78              0               2707456                 288
13:41:57         R               107             2808            8592777216              3232
13:41:58         R               938             124836                  8592777216              126048
13:41:59         R               1838            227933                  8592777216              230048
13:42:00         S               3376            311384                  8592777216              314304
13:42:01         R               6418            375216                  8592777216              378912
13:42:02         S               11459           423160                  8592777216              427520
13:42:03         R               17112           448774                  8592777216              453536
13:42:04         R               26620           471273                  8592777216              476416
13:42:05         S               38198           485426                  8592777216              490784
13:42:07         R               53187           496124                  8592777216              501696
13:42:08         S               68644           502793                  8592777216              508480
13:42:09         R               82972           506468                  8592777216              512256
13:42:10         R               101232                  509879                  8592777216              515712


time            state   minor_fault             major_fault     virtual_memory_size     resident_set_size
13:42:11         R               123092                  512287                  8592777216              518144
13:42:12         R               149618                  514192                  8592777216              520096
13:42:13         D               160191                  514739                  0               0
process 'a.out' has stopped running
 -----------------------------------/
 *
 *
 *
***********************************************************************/

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
