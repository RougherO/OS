#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define BUF_SIZE 1024

double get_delta(struct timespec start, struct timespec end)
{
    return (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) * 1e-6;
}

void create_file(char const* filename, size_t size)
{
    FILE* big_file = fopen(filename, "w");

    char const* msg = "Hello World!";
    fputs(msg, big_file);
    for (size_t i = strlen(msg); i < size; ++i) {
        fputc(0, big_file);
    }
}

void send_file(char const* filename, char const* fifoname)
{
    int fifo_fd = open(fifoname, O_WRONLY);
    if (fifo_fd < 0) {
        perror("error opening fifo in send_file");
        exit(1);
    }

    int file_fd = open(filename, O_RDONLY);
    if (file_fd < 0) {
        perror("error opening file in send_file");
        exit(1);
    }

    char buffer[BUF_SIZE] = { 0 };
    ssize_t bytes         = 0;
    while ((bytes = read(file_fd, buffer, BUF_SIZE)) > 0) {
        if (write(fifo_fd, buffer, bytes) != bytes) {
            perror("error writing to fifo in send_file");
            exit(1);
        }
    }
    if (bytes < 0) {
        perror("error reading file in send_file");
        exit(1);
    }

    close(fifo_fd);
    close(file_fd);
}

void recv_file(char const* filename, char const* fifoname)
{
    int fifo_fd = open(fifoname, O_RDONLY);
    if (fifo_fd < 0) {
        perror("error opening fifo in recv_file");
        exit(1);
    }

    int file_fd = open(filename, O_CREAT | O_WRONLY, 0666);
    if (file_fd < 0) {
        perror("error opening file in recv_file");
        exit(1);
    }

    char buffer[BUF_SIZE] = { 0 };
    ssize_t bytes         = 0;
    while ((bytes = read(fifo_fd, buffer, BUF_SIZE)) > 0) {
        if (write(file_fd, buffer, bytes) != bytes) {
            perror("error writing to file in recv_file");
            exit(1);
        }
    }
    if (bytes < 0) {
        perror("error reading fifo in recv_file");
        exit(1);
    }

    close(file_fd);
    close(fifo_fd);
}

int comp_file(char const* file1, char const* file2)
{
    int fd1 = open(file1, O_RDONLY);
    if (fd1 < 0) {
        perror("error opening file in comp_file");
        exit(1);
    }

    int fd2 = open(file2, O_RDONLY);
    if (fd2 < 0) {
        perror("error opening file in comp_file");
        exit(1);
    }

    char buffer1[BUF_SIZE], buffer2[BUF_SIZE];
    ssize_t bytes1, bytes2;

    int is_same = 0;
    while ((bytes1 = read(fd1, buffer1, BUF_SIZE)) > 0
           && (bytes2 = read(fd2, buffer2, BUF_SIZE)) > 0) {
        /* Compare the file byte by byte */
        if (bytes1 != bytes2 || memcmp(buffer1, buffer2, BUF_SIZE) != 0) {
            is_same = -1;
            break;
        }
    }

    close(fd1);
    close(fd2);

    return is_same;
}

void print_file(char const* filename)
{
    FILE* file = fopen(filename, "r");

    char buffer[BUF_SIZE];
    while (fgets(buffer, BUF_SIZE, file)) {
        printf("%s", buffer);
    }
}

int main()
{
    char const* sp_filename = "sendp"; /* sent file's name from parent */
    char const* rp_filename = "recvp"; /* received file's name in parent */
    char const* rc_filename = "recvc"; /* received file's name in child */
    char const* fifo1       = "fifo1";
    char const* fifo2       = "fifo2";

    create_file(sp_filename,
                1024 * 1024 * 1024 * sizeof(char)); /* Creating a 1G file */

    if (mkfifo(fifo1, 0666) < 0) {
        perror("fifo1");
        exit(1);
    }
    if (mkfifo(fifo2, 0666) < 0) {
        perror("fifo2");
        exit(1);
    }

    struct timespec proc_start, proc_end;
    timespec_get(&proc_start, TIME_UTC);
    switch (fork()) {
        case -1:
            perror("fork");
            exit(1);
            break;

        case 0:
            /* Receive and send file from and to parent process */
            recv_file(rc_filename, fifo1);
            send_file(rc_filename, fifo1);

            timespec_get(&proc_end, TIME_UTC); /* Get the current time in child process */
            printf("Time taken for the child process to recv and send file: %lfms\n",
                   get_delta(proc_start, proc_end));
            break;

        default:
            /* Send and receive file to and from child process */
            send_file(sp_filename, fifo1);
            recv_file(rp_filename, fifo1);

            timespec_get(&proc_end, TIME_UTC); /* Get the current time in parent process */
            printf("Time taken for the parent process to send and recv file: %lfms\n",
                   get_delta(proc_start, proc_end));

            wait(NULL); /* Wait for child to exit. Prevent zombie process */

            if (comp_file(sp_filename, rp_filename) < 0) {
                printf("Files are different\n");
            } else {
                printf("Files are same\n");
                printf("Content of %s:\n", sp_filename);
                print_file(sp_filename);
                printf("Content of %s:\n", rp_filename);
                print_file(rp_filename);
            }

            /* Deleting the FIFO's, cleaningup */
            unlink(fifo1);
            unlink(fifo2);
    }
}