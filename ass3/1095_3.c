/***********************************************************************
 *
 * Name: Chirag Chowdhury
 * Roll: 002211001095
 *
 * Date: 19.08.2024
 *
 * Assignment: 3, IPC using Socket
 *
 *   Objective:
 *       Transfer a file (1GB) between parent and child processes
 *     uisng a socket, and compare them.
 *
 *       This communicaton will be happening between processes within the same host,
 *     so will be using UNIX Domain Sockets.
 *
 *       Steps involved in this program:
 *     Step 1: Create Socket, establish connection and print its info
 *     Step 2: Given File -> (parent) -> |Socket| -> (child) -> output-2
 *     Step 3: Given File -> (child) -> |Socket| -> (parent) -> output-1
 *     Step 4: Comapre output-1 & output-2
 *     Step 5: Print file transfer time
 *
 *
 * Input Description: Source file
 *
 * Output Description: Socket and Transfer details, Transfer time, Output Files comparision result
 *
 *
 * Compilation Command: gcc 1095_3.c
 * Execution Sequence: ./a.out <file_name>
 *
 *
 * Socket Info: $ ss -x | (head -n 1 && grep "unix_socket.sock")
 /-----------------------------------
Netid  State      Recv-Q Send-Q Local Address:Port                 Peer Address:Port
u_str  ESTAB      95232  0      unix_socket.sock 158751492             * 158751491
u_str  ESTAB      0      172800 unix_socket.sock 159921672             * 159921671
 ----------------------------------/
 *
 *
 * Sample Input: very-large-file (size = 10 MB)
 * Sample Output:
 /-----------------------------------
[+]Server is listening on unix_socket.sock
[+]Client is ready to connect
[+]Client Connected on <path_not_specified>
[+]Pid-9837: Sending file
[+]Pid-9838: Receiving file
[+]Pid-9837: File sent
[+]Pid-9838: File received
[+]Pid-9837: Receiving file
[+]Pid-9838: Sending file
[+]Pid-9838: File sent
[+]Pid-9837: File received

[+]Time taken for double transfer: 0.064731

[+]Comparing files...
[+]Files are identical
 -----------------------------------/
***********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <sys/time.h>

#define SOCKET_PATH              "unix_socket.sock"
#define BUFFER_SIZE              1024
#define BYTE_CHECK_TRANSFER_FLAG 2

void error_exit(char const*);                     // Prints the specified error message to stderr and terminates the program
void create_server(int*, struct sockaddr_un*);    // Creates a UNIX domain socket, binds it to the specified file path, and prepares it to listen for incoming connections
int create_client(struct sockaddr_un);            // Creates a UNIX domain socket and connects it to the server using the provided address
int accept_connection(int, struct sockaddr_un);   // Waits for a client to connect to the server, accepts the connection, and provides a new file descriptor
void send_file(int, char const*);                 // Reads from specified file and sends it to the connected socket. Then, the write end of the socket is shut down
void receive_file(int, char const*);              // Receives data from connected socket and writes it to a specified file. Then, the read end of the socket is shut down
int compare_files(char const*, char const*);      // Reads two files byte by byte and compares them. Returns 0 if identical, or 1 if not

int main(int argc, char* argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>", argv[0]);
        exit(EXIT_FAILURE);
    }

    int server_fd, client_fd, connection_fd;
    struct sockaddr_un server_addr, client_addr;
    pid_t child;
    char const* output_file1 = "output_file1";
    char const* output_file2 = "output_file2";
    struct timeval start, end;
    double time_taken;

    int status = 0;

    // Step 1
    create_server(&server_fd, &server_addr);
    client_fd     = create_client(server_addr);
    connection_fd = accept_connection(server_fd, client_addr);

    // Closing server as we only need one connection
    close(server_fd);

    // Starting the clock
    gettimeofday(&start, NULL);

    // Create child process
    if ((child = fork()) < 0) {
        error_exit("Fork failed");
    }

    // Step 2

    // In parent
    if (child > 0) {
        send_file(connection_fd, argv[1]);
    }
    // In child
    else {
        receive_file(client_fd, output_file2);
    }

    // Step 3

    // In child
    if (child == 0) {
        send_file(client_fd, argv[1]);
        exit(EXIT_SUCCESS);
    }
    // In parent
    else {
        receive_file(connection_fd, output_file1);
        waitpid(child, &status, 0);
    }

    // Stopping clock after double transfer
    gettimeofday(&end, NULL);
    time_taken = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
    printf("\n[+]Time taken for double transfer: %f\n", time_taken);

    // Step 4 - Parent Process
    if (child > 0) {
        printf("\n[+]Comparing files...\n");

        if (compare_files(output_file1, output_file2) == 0) {
            printf("[+]Files are identical\n");
        } else {
            printf("[+]Files aren't identical\n");
        }
    }

    // Clean up
    unlink(SOCKET_PATH);
    close(client_fd);
    close(connection_fd);

    return 0;
}

void error_exit(char const* message)
{
    perror(message);
    unlink(SOCKET_PATH);
    exit(EXIT_FAILURE);
}

void create_server(int* server_fd, struct sockaddr_un* server_addr)
{
    // Create UNIX Domain Socket
    if ((*server_fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
        error_exit("[-]Socket failed");
    }

    // Unlink path if already exists
    unlink(SOCKET_PATH);

    // Specifing the address
    memset(server_addr, 0, sizeof(struct sockaddr_un));
    server_addr->sun_family = AF_UNIX;
    strncpy(server_addr->sun_path, SOCKET_PATH, sizeof(server_addr->sun_path) - 1);

    // Bind the socket to the specified path
    if (bind(*server_fd, (struct sockaddr*)server_addr, sizeof(struct sockaddr_un)) < 0) {
        close(*server_fd);
        error_exit("[-]Bind failed");
    }

    // Listen for incoming connections
    if (listen(*server_fd, 5) < 0) {
        close(*server_fd);
        error_exit("Listen failed");
    }

    printf("[+]Server is listening on %s\n", SOCKET_PATH);
}

int create_client(struct sockaddr_un server_addr)
{
    int client_fd;

    // Create UNIX Domain Socket
    if ((client_fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
        error_exit("[-]Socket failed");
    }

    // Connect to the server
    if (connect(client_fd, (struct sockaddr*)&server_addr, sizeof(struct sockaddr_un)) < 0) {
        close(client_fd);
        error_exit("[-]Client failed to connect");
    }

    printf("[+]Client is ready to connect\n");

    return client_fd;
}

int accept_connection(int server_fd, struct sockaddr_un client_addr)
{
    int connection_fd;
    socklen_t client_addr_len = sizeof(client_addr);

    if ((connection_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_addr_len)) < 0) {
        error_exit("[-]Failed to accept connection");
    }

    char* client_path = client_addr.sun_path[0] == '\0' ? "<path_not_specified>" : client_addr.sun_path;
    printf("[+]Client Connected on %s\n", client_path);

    return connection_fd;
}

void send_file(int fd, char const* filename)
{
    FILE* file = fopen(filename, "rb");
    if (!file) {
        error_exit("[-]Failed to open file for reading");
    }

    pid_t pid = getpid();
    char buffer[BUFFER_SIZE];
    size_t bytes_read;
    long total_bytes_sent = 0;
    long byte_checker     = 1024 * 1024 * BYTE_CHECK_TRANSFER_FLAG;
    int flag_counter      = 0;

    printf("[+]Pid-%d: Sending file\n", pid);

    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, file)) > 0) {
        if (send(fd, buffer, bytes_read, 0) < 0) {
            error_exit("[-]Failed to send file data");
        }

        /*
        //
        total_bytes_sent += bytes_read;
        if (total_bytes_sent % byte_checker == 0) {
            flag_counter++;
            printf("[+]Pid-%d: %d MB sent, counter: %d\n", pid, BYTE_CHECK_TRANSFER_FLAG, flag_counter);
        }
        */
    }

    printf("[+]Pid-%d: File sent\n", pid);

    // Disables write for this socket
    shutdown(fd, SHUT_WR);

    fclose(file);
}

void receive_file(int fd, char const* output_filename)
{
    FILE* file = fopen(output_filename, "wb");
    if (!file) {
        error_exit("[-]Failed to open file for writing");
    }

    pid_t pid = getpid();
    char buffer[BUFFER_SIZE];
    ssize_t bytes_received;
    long total_bytes_received = 0;
    long byte_checker         = 1024 * 1024 * BYTE_CHECK_TRANSFER_FLAG;
    int flag_counter          = 0;

    printf("[+]Pid-%d: Receiving file\n", pid);

    while ((bytes_received = recv(fd, buffer, BUFFER_SIZE, 0)) > 0) {
        // printf("%s\n", buffer);
        fwrite(buffer, 1, bytes_received, file);

        /*
        //
        total_bytes_received += bytes_received;
        if (total_bytes_received % byte_checker == 0) {
            flag_counter++;
            printf("[+]Pid-%d: %d MB received, counter: %d\n", pid, BYTE_CHECK_TRANSFER_FLAG, flag_counter);
        }
        */
    }

    if (bytes_received < 0) {
        error_exit("[-]Failed to receive file data");
    }

    printf("[+]Pid-%d: File received\n", pid);

    //  Disables read for this socket
    shutdown(fd, SHUT_RD);

    fclose(file);
}

int compare_files(char const* file1, char const* file2)
{
    FILE* f1 = fopen(file1, "r");
    FILE* f2 = fopen(file2, "r");

    if (!f1) {
        error_exit("[-]Compare: Couldn't open file1");
    }
    if (!f2) {
        fclose(f1);
        error_exit("[-]Compare: Couldn't open file2");
    }

    char ch1 = fgetc(f1);
    char ch2 = fgetc(f2);

    // Match one character at a time
    while (ch1 != EOF && ch2 != EOF) {
        if (ch1 != ch2) {
            fclose(f1);
            fclose(f2);
            return 1;
        }
        ch1 = fgetc(f1);
        ch2 = fgetc(f2);
    }

    // One file ended prematurly
    if ((ch1 != EOF) || (ch2 != EOF)) {
        fclose(f1);
        fclose(f2);
        return 1;
    }

    fclose(f1);
    fclose(f2);
    return 0;
}