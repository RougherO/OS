/***********************************************************************
 * Name: Chirag Chowdhury
 * Roll: 002211001095
 *
 * Date: 05.08.2024
 *
 * Assignment: 2A
 *       [Catch the signal ‘SIGINT’ and display “Ha Ha, Not Stopping”.
 *        Use ‘signal’ system call. Always use “perror” to check the return status of a library/system call.]
 *
 * Input Description: None
 *
 * Output Description: Program Comments
 *
 *
 * Compilation Command: gcc 1095_2A.c
 * Execution Sequence: ./a.out
 *
 *
 * Sample Input: None
 * Sample Output:
 /-----------------------------------
Somebody stop me
Somebody stop me
Somebody stop me
Somebody stop me
^CHa Ha, Not Stopping
Somebody stop me
Somebody stop me
Somebody stop me
^CHa Ha, Not Stopping
Somebody stop me
^Z
[1]+  Stopped                 ./a.out
 -----------------------------------/
***********************************************************************/

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

void handle_sigint(int);

int main(int argc, char** argv)
{
    if (signal(SIGINT, handle_sigint) == SIG_ERR) {
        perror("signal");
        exit(1);
    }

    while (1) {
        printf("Somebody stop me\n");
        sleep(1);
    }

    return 0;
}

void handle_sigint(int sig)
{
    printf("Ha Ha, Not Stopping\n");
}