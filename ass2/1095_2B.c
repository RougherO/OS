/***********************************************************************
 * Name: Chirag Chowdhury
 * Roll: 002211001095
 *
 * Date: 12.08.2024
 *
 * Assignment: 2B, IPC using Named Pipe (FIFO)
 *      Steps involved in this program:
 *      Step 1: Given File -> (process-1) -> |FIFO-1| -> (process-2) -> output-2
 *      Step 2: Given File -> (process-2) -> |FIFO-2| -> (process-1) -> output-1
 *      Step 3: Comapre output-1 & output-2
 *
 *
 * Input Description: File Name
 *
 * Output Description: Comments related with process creation and execution + File Comaprision (true/false) + Time Required for Double Transfer
 *
 *
 * Compilation Command: gcc 1095_2B.c
 * Execution Sequence: ./a.out <file_name>
 *
 *
 * Sample Input: very-large-file
 * Sample Output:
 /-----------------------------------
Created processes with Pid-7834 & Pid-7835

Pid-7834: waiting for reader...
Pid-7834: Done writing
Pid-7834: waiting for writer...
Pid-7834: got a writer
Pid-7834: Done reading
Pid-7835: waiting for writer...
Pid-7835: got a writer
Pid-7835: Done reading
Pid-7835: waiting for reader...
Pid-7835: Done writing

Time required for double tranfer = 4.068030 seconds

Compare generated files
Files are identical
 -----------------------------------/
***********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/time.h>

void write_fifo(char const*, char const*);
void read_fifo(char const*, char const*);
int compare_files(char const*, char const*);

int main(int argc, char** argv)
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    pid_t proc1, proc2;
    int status               = 0;
    char const* fifo1        = "Imitator";
    char const* fifo2        = "Mr. Mime";
    char const* output_file1 = "output_file1";
    char const* output_file2 = "output_file2";
    double time_spent;
    struct timeval start, end;

    // Removes link if already exists
    unlink(fifo1);
    unlink(fifo2);

    // creat FIFOs
    if (mkfifo(fifo1, 0666) == -1 || mkfifo(fifo2, 0666) == -1) {
        perror("mkfifo");
        exit(EXIT_FAILURE);
    }

    // Start the Clock
    gettimeofday(&start, NULL);

    // creating process 1
    proc1 = fork();
    if (proc1 < 0) {
        perror("Problem creating proc1");
        exit(EXIT_FAILURE);
    }

    // creating process 2
    if (proc1 != 0) {
        proc2 = fork();
        if (proc2 < 0) {
            perror("Problem creating proc2");
            exit(EXIT_FAILURE);
        }
    }

    /*
     * pid values from diffent prospective
     *           Parent      Proc1       Proc2
     * Parent      0           x           y
     * Proc1:      z           0           0
     * Proc2:      z           x           0
     */

    // In process 1
    if (proc1 == 0) {
        // Assigning a +ve value to proc2
        proc2 = 1;
    }

    // Parent Process
    if (proc1 != 0 && proc2 != 0) {
        printf("Created processes with Pid-%d & Pid-%d\n\n", proc1, proc2);
    }

    // Step 1
    if (proc1 == 0) {
        // process 1 writes into FIFO
        write_fifo(fifo1, argv[1]);
    } else if (proc2 == 0) {
        // Processes 2 reads form FIFO
        read_fifo(fifo1, output_file2);
    }

    // Step 2
    if (proc2 == 0) {
        // process 2 writes into FIFO
        write_fifo(fifo2, argv[1]);
        exit(EXIT_SUCCESS);
    } else if (proc1 == 0) {
        // Processes 1 reads form FIFO
        read_fifo(fifo2, output_file1);
        exit(EXIT_SUCCESS);
    } else {
        // Wait for processes to finish in Parent
        waitpid(proc1, &status, 0);
        waitpid(proc2, &status, 0);
        // Stop the clock
        gettimeofday(&end, NULL);
        time_spent = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;

        // Get time for double transfer
        printf("\nTime required for double tranfer = %f seconds\n", (time_spent));
    }

    // Step 3 - Parent Process
    if (proc1 != 0 && proc2 != 0) {
        printf("\nCompare generated files\n");

        int check_flag = compare_files(output_file1, output_file2);
        if (check_flag == 0) {
            printf("Files are identical\n");
        } else if (check_flag == 1) {
            printf("Files aren't identical\n");
        } else {
            printf("Error: File Comparison.\n");
        }
    }

    unlink(fifo1);
    unlink(fifo2);

    return 0;
}

void write_fifo(char const* fifo, char const* filename)
{
    pid_t pid = getpid();
    FILE* fp;
    int fd;
    char line[200];

    fp = fopen(filename, "r");
    if (fp == NULL) {
        perror("Failed to open file for reading");
        exit(EXIT_FAILURE);
    }

    printf("Pid-%d: waiting for reader...\n", pid);

    fd = open(fifo, O_WRONLY);
    if (fd < 0) {
        perror("Failed to open FIFO for writing");
        fclose(fp);
        exit(EXIT_FAILURE);
    }

    while (fgets(line, sizeof(line), fp)) {
        int len = strlen(line);
        int num = write(fd, line, len);
        if (num != len) {
            perror("Failed to write into FIFO");
            exit(EXIT_FAILURE);
        }
    }

    close(fd);
    fclose(fp);

    printf("Pid-%d: Done writing\n", pid);
}

void read_fifo(char const* fifo, char const* output_file)
{
    pid_t pid = getpid();
    FILE* fp;
    int fd;
    ssize_t num;
    char buffer[300];

    fp = fopen(output_file, "w");
    if (fp == NULL) {
        perror("Failed to open file for writing");
        exit(EXIT_FAILURE);
    }

    printf("Pid-%d: waiting for writer...\n", pid);

    fd = open(fifo, O_RDONLY);
    if (fd < 0) {
        perror("Failed to open FIFO for reading");
        fclose(fp);
        exit(EXIT_FAILURE);
    }

    printf("Pid-%d: got a writer\n", pid);

    while ((num = read(fd, buffer, sizeof(buffer))) > 0) {
        fwrite(buffer, 1, num, fp);
    }

    if (num == -1) {
        perror("Failed to read from FIFO");
    }

    close(fd);
    fclose(fp);

    printf("Pid-%d: Done reading\n", pid);
}

int compare_files(char const* file1, char const* file2)
{
    FILE* f1 = fopen(file1, "r");
    FILE* f2 = fopen(file2, "r");

    if (!f1) {
        perror("Compare: Couldn't open file1");
        return -1;
    }
    if (!f2) {
        perror("Compare: Couldn't open file2");
        fclose(f1);
        return -1;
    }

    char ch1, ch2;
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
        printf("%c + %c", ch1, ch2);
        fclose(f1);
        fclose(f2);
        return 1;
    }

    fclose(f1);
    fclose(f2);
    return 0;
}