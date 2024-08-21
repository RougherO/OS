#include <signal.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <wait.h>
#include <sys/un.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>

#define BUF_SIZE 1024
#define EOF_MSG  "EOF"
#define EOF_LEN  strlen(EOF_MSG)

void sync_signal(int sig)   // Signal handler for synchronization
{
}

double get_delta(struct timespec start, struct timespec end)
{
    return (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) * 1e-6;
}

void create_file(char const* filename, size_t size)
{
    FILE* big_file = fopen(filename, "wb");

    char const* msg = "Hello World!";
    fputs(msg, big_file);
    for (size_t i = strlen(msg); i < size; ++i) {
        fputc(0, big_file);
    }

    fclose(big_file);
}

void send_file(char const* filename, int sock_fd)
{
    int file_fd = open(filename, O_RDONLY);
    if (file_fd < 0) {
        perror("error opening file in send_file");
        exit(1);
    }

    char buffer[BUF_SIZE] = { 0 };
    ssize_t bytes         = 0;
    while ((bytes = read(file_fd, buffer, BUF_SIZE)) > 0) {
        if (write(sock_fd, buffer, bytes) != bytes) {
            perror("error writing to socket in send_file");
            exit(1);
        }
    }
    if (bytes < 0) {
        perror("error reading file in send_file");
        exit(1);
    }
    if (write(sock_fd, EOF_MSG, EOF_LEN) != EOF_LEN) { /* Sending EOF message at last to signal all bytes have been sent */
        perror("error in sending eof signal");
        exit(1);
    }

    close(file_fd);
}

void recv_file(char const* filename, int sock_fd)
{
    int file_fd = open(filename, O_CREAT | O_WRONLY, 0666);
    if (file_fd < 0) {
        perror("error opening file in recv_file");
        exit(1);
    }

    char buffer[BUF_SIZE] = { 0 };
    ssize_t bytes         = 0;
    while ((bytes = read(sock_fd, buffer, BUF_SIZE)) > 0) {
        if (!strncmp(buffer + bytes - EOF_LEN, EOF_MSG, EOF_LEN)) { /* Checking for EOF signal in the receieved msg */
            bytes -= EOF_LEN;
            if (write(file_fd, buffer, bytes) != bytes) {
                perror("error writing to file in recv_file");
                exit(1);
            }
            break;
        } else {
            if (write(file_fd, buffer, bytes) != bytes) {
                perror("error writing to file in recv_file");
                exit(1);
            }
        }
    }
    if (bytes < 0) {
        perror("error reading socket in recv_file");
        exit(1);
    }

    close(file_fd);
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
    ssize_t bytes1 = 0, bytes2 = 0;

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
    FILE* file = fopen(filename, "rb");

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
    char const* sock_path   = "ipc_socket";

    create_file(sp_filename, 1024 * 1024 * 1024 * sizeof(char)); /* Creating a 1G file */

    struct sockaddr_un sock;
    memset(&sock, 0, sizeof(sock));
    sock.sun_family = AF_UNIX;
    memcpy(sock.sun_path, sock_path, sizeof(sock.sun_path));

    struct timespec proc_start, proc_end;
    timespec_get(&proc_start, TIME_UTC);

    pid_t pid = fork();

    switch (pid) {
        case -1:
            perror("fork");
            exit(1);
            break;

        case 0: {
            /**
             * Here we are pausing the child process
             * until parent process is ready to start listening.
             * It may happen that parent process is preempted before it
             * was able to create the socket for connection.
             * This singal is used to pause the child process until parent process
             * signals that it is ready to accept connection.
             */
            signal(SIGUSR1, sync_signal);
            pause();

            int csock_fd = socket(AF_UNIX, SOCK_STREAM, 0);

            if (connect(csock_fd, (struct sockaddr*)&sock, sizeof(sock)) < 0) {
                perror("could not connect to socket in child");
                exit(1);
            }

            /* Receive and send file from and to parent process */
            recv_file(rc_filename, csock_fd);
            send_file(rc_filename, csock_fd);

            close(csock_fd);

            timespec_get(&proc_end, TIME_UTC); /* Get the current time in child process */
            printf("Time taken for the child process to recv and send file: %lfms\n",
                   get_delta(proc_start, proc_end));
        } break;

        default: {
            int psock_fd = socket(AF_UNIX, SOCK_STREAM, 0);
            if (psock_fd < 0) {
                perror("error creating parent socket");
                exit(1);
            }

            if (bind(psock_fd, (struct sockaddr*)&sock, sizeof(sock)) < 0) {
                perror("failed to bind parent socket");
                exit(1);
            }

            if (listen(psock_fd, 1) < 0) {
                perror("failed to listen on parent socket");
                exit(1);
            }

            kill(pid, SIGUSR1); /* Sending signal to child process that parent is ready to accept connection. */

            int csock_fd = accept(psock_fd, NULL, NULL);
            if (csock_fd < 0) {
                perror("failed to accept connection on parent socket");
                exit(1);
            }

            /* Send and receive file to and from child process */
            send_file(sp_filename, csock_fd);
            recv_file(rp_filename, csock_fd);

            close(csock_fd);

            timespec_get(&proc_end, TIME_UTC); /* Get the current time in parent process */
            printf("Time taken for the parent process to send and recv file: %lfms\n",
                   get_delta(proc_start, proc_end));

            wait(NULL); /* Wait for child to exit. Prevent zombie process */

            if (comp_file(sp_filename, rp_filename) < 0) {
                printf("Files are different\n");
                printf("Content of %s:\n", sp_filename);
                print_file(sp_filename);
                puts("");
                printf("Content of %s:\n", rp_filename);
                print_file(rp_filename);
                puts("");
            } else {
                printf("Files are same\n");
                printf("Content of %s:\n", sp_filename);
                print_file(sp_filename);
                puts("");
                printf("Content of %s:\n", rp_filename);
                print_file(rp_filename);
                puts("");
            }

            /* Deleting the FIFO's, cleaningup */
            unlink(sock_path);
        }
    }
}