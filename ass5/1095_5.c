/***********************************************************************
 *
 * Name: Chirag Chowdhury
 * Roll: 002211001095
 *
 * Date: 06.09.2024
 *
 * Assignment: 5, Creating DEADLOCK using Thread Programming
 *
 *      Objectives:
 *          Define two global variables (Total_1 and Total_2) and initialize them to 1000.
 *          Use two mutexes to protect these two global variables.
 *          Also create two threads whose, function is to generate a 'random quantity' (not more than 50) and subtract it from one of the Totals and add it to the other one.
 *          While manipulating the Totals, each Thread should lock both the mutex and then unlock it after changing the Totals, to avoid deadlocks.
 *          Also, include that information (as comment in your source file) how to avoid this kind of Deadlock.
 *
 *      If these two (Threads, Totals and Mutex) are not good enough for Deadlock; Then you need to create another set of Thread, Total and Mutex; and carry on the same experiment.
 *      Make sure there are enough printf in your program so that it can be clearly understood that there is a Deadlock. One way to ensure enough printf is to display the total of Total_1 and Total_2 all the time after every operation on it.
 *
 *
 * Input Description: None
 *
 * Output Description:
 *      Displays messages showing when the threads attempt to acquire the locks,
 *      the random value generated, the updated totals, and whether the locks were released.
 *      If deadlock occurs, the threads will be stuck without any further output.
 *
 * Compilation Command: gcc 1095_5.c
 * Execution Sequence: ./a.out 0
 *
 *
 * Sample Input:
 * Sample Output:
 /-----------------------------------
Thread 1 locked Mutex 1
Thread 2 locked Mutex 2
^C
 * Execution Sequence: ./a.out 1
 *
 *
 * Sample Input:
 * Sample Output:
 /-----------------------------------
Thread 1 locked Mutex 1
Thread 1 locked Mutex 2
Thread 1 updated: Total_1 = 973, Total_2 = 1027
Thread 1 unlocked Mutex 2
Thread 1 unlocked Mutex 1
Thread 2 locked Mutex 1
Thread 2 locked Mutex 2
Thread 2 updated: Total_1 = 999, Total_2 = 1001
Thread 2 unlocked Mutex 1
Thread 2 unlocked Mutex 2
Thread 1 locked Mutex 1
Thread 1 locked Mutex 2
Thread 1 updated: Total_1 = 983, Total_2 = 1017
Thread 1 unlocked Mutex 2
Thread 1 unlocked Mutex 1
Thread 2 locked Mutex 1
Thread 2 locked Mutex 2
Thread 2 updated: Total_1 = 997, Total_2 = 1003
Thread 2 unlocked Mutex 1
Thread 2 unlocked Mutex 2
Thread 1 locked Mutex 1
Thread 1 locked Mutex 2
Thread 1 updated: Total_1 = 958, Total_2 = 1042
Thread 1 unlocked Mutex 2
Thread 1 unlocked Mutex 1
Thread 2 locked Mutex 1
Thread 2 locked Mutex 2
Thread 2 updated: Total_1 = 979, Total_2 = 1021
Thread 2 unlocked Mutex 1
Thread 2 unlocked Mutex 2
Thread 2 locked Mutex 1
Thread 2 locked Mutex 2
Thread 2 updated: Total_1 = 1020, Total_2 = 980
Thread 2 unlocked Mutex 1
Thread 2 unlocked Mutex 2
Thread 1 locked Mutex 1
Thread 1 locked Mutex 2
Thread 1 updated: Total_1 = 1017, Total_2 = 983
Thread 1 unlocked Mutex 2
Thread 1 unlocked Mutex 1
Thread 2 locked Mutex 1
Thread 2 locked Mutex 2
Thread 2 updated: Total_1 = 1038, Total_2 = 962
Thread 2 unlocked Mutex 1
Thread 2 unlocked Mutex 2
Thread 1 locked Mutex 1
Thread 1 locked Mutex 2
Thread 1 updated: Total_1 = 1031, Total_2 = 969
Thread 1 unlocked Mutex 2
Thread 1 unlocked Mutex 1
Thread 2 locked Mutex 1
Thread 2 locked Mutex 2
Thread 2 updated: Total_1 = 1054, Total_2 = 946
Thread 2 unlocked Mutex 1
Thread 2 unlocked Mutex 2
Thread 1 locked Mutex 1
Thread 1 locked Mutex 2
Thread 1 updated: Total_1 = 1020, Total_2 = 980
Thread 1 unlocked Mutex 2
Thread 1 unlocked Mutex 1
Thread 2 locked Mutex 1
Thread 2 locked Mutex 2
Thread 2 updated: Total_1 = 1066, Total_2 = 934
Thread 2 unlocked Mutex 1
Thread 2 unlocked Mutex 2
Thread 1 locked Mutex 1
Thread 1 locked Mutex 2
Thread 1 updated: Total_1 = 1053, Total_2 = 947
Thread 1 unlocked Mutex 2
Thread 1 unlocked Mutex 1
Thread 2 locked Mutex 1
Thread 2 locked Mutex 2
Thread 2 updated: Total_1 = 1075, Total_2 = 925
Thread 2 unlocked Mutex 1
Thread 2 unlocked Mutex 2
Thread 1 locked Mutex 1
Thread 1 locked Mutex 2
Thread 1 updated: Total_1 = 1031, Total_2 = 969
Thread 1 unlocked Mutex 2
Thread 1 unlocked Mutex 1
Thread 2 locked Mutex 1
Thread 2 locked Mutex 2
Thread 2 updated: Total_1 = 1061, Total_2 = 939
Thread 2 unlocked Mutex 1
Thread 2 unlocked Mutex 2
Thread 1 locked Mutex 1
Thread 1 locked Mutex 2
Thread 1 updated: Total_1 = 1045, Total_2 = 955
Thread 1 unlocked Mutex 2
Thread 1 unlocked Mutex 1
Thread 2 locked Mutex 1
Thread 2 locked Mutex 2
Thread 2 updated: Total_1 = 1050, Total_2 = 950
Thread 2 unlocked Mutex 1
Thread 2 unlocked Mutex 2
Thread 1 locked Mutex 1
Thread 1 locked Mutex 2
Thread 1 updated: Total_1 = 1020, Total_2 = 980
Thread 1 unlocked Mutex 2
Thread 1 unlocked Mutex 1
 -----------------------------------/
 *
 * Remarks:
 *      1. Acquire locks in the same order to prevent deadlocks.
 *
***********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

#define MAX_RANDOM 50

/* Global variables */
int Total_1 = 1000;
int Total_2 = 1000;

/* Mutexes */
typedef struct {
    pthread_mutex_t mutex;
    char const* name;
} Mutex;

Mutex mutex1;
Mutex mutex2;

/* Function for Thread 1 */
void* thread_function_1()
{
    int i;
    for (i = 0; i < 10; i++) {
        /* Generate a random quantity */
        int quantity = rand() % MAX_RANDOM;

        /* Lock mutex1 first, then mutex2 */
        pthread_mutex_lock(&mutex1.mutex);
        printf("Thread 1 locked %s\n", mutex1.name);

        pthread_mutex_lock(&mutex2.mutex);
        printf("Thread 1 locked %s\n", mutex2.name);

        /* Update totals */
        Total_1 -= quantity;
        Total_2 += quantity;

        /* Print updated totals */
        printf("Thread 1 updated: Total_1 = %d, Total_2 = %d\n", Total_1, Total_2);

        /* Unlock mutexes */
        pthread_mutex_unlock(&mutex2.mutex);
        printf("Thread 1 unlocked %s\n", mutex2.name);

        pthread_mutex_unlock(&mutex1.mutex);
        printf("Thread 1 unlocked %s\n", mutex1.name);

        sleep(1); /* Just to make the output clearer */
    }
    return NULL;
}

/* Function for Thread 2 */
void* thread_function_2(void* arg)
{
    int i;
    for (i = 0; i < 10; i++) {
        /* Generate a random quantity */
        int quantity = rand() % MAX_RANDOM;

        /* Set mutex according to deadlock condition */
        Mutex* m1 = NULL;
        Mutex* m2 = NULL;

        if (*(int*)arg == 1) {
            m1 = &mutex2;
            m2 = &mutex1;
        } else {
            m1 = &mutex1;
            m2 = &mutex2;
        }

        /* Lock mutex2 first, then mutex1 */
        pthread_mutex_lock(&m1->mutex);
        printf("Thread 2 locked %s\n", m1->name);

        pthread_mutex_lock(&m2->mutex);
        printf("Thread 2 locked %s\n", m2->name);

        /* Update totals */
        Total_2 -= quantity;
        Total_1 += quantity;

        /* Print updated totals */
        printf("Thread 2 updated: Total_1 = %d, Total_2 = %d\n", Total_1, Total_2);

        /* Unlock mutexes */
        pthread_mutex_unlock(&m1->mutex);
        printf("Thread 2 unlocked %s\n", m1->name);

        pthread_mutex_unlock(&m2->mutex);
        printf("Thread 2 unlocked %s\n", m2->name);

        sleep(1); /* Just to make the output clearer */
    }
    return NULL;
}

int main(int argc, char** argv)
{
    srand(time(NULL)); /* Seed for random number generation */

    int deadlock       = 1;
    char const* errmsg = "[Usage]: <exe> <0|1>\n"
                         " 0: Lock free case\n"
                         " 1: Dead lock case\n";

    if (argc > 2 || argc < 2) {
        printf(errmsg);
        exit(1);
    } else if (argc == 2) {
        deadlock = strtol(argv[1], NULL, 0);
    }

    /* Initialize mutexes */
    mutex1.name = "Mutex 1";
    pthread_mutex_init(&mutex1.mutex, NULL);
    mutex2.name = "Mutex 2";
    pthread_mutex_init(&mutex2.mutex, NULL);

    /* Create threads */
    pthread_t thread1, thread2;
    pthread_create(&thread1, NULL, thread_function_1, NULL);
    pthread_create(&thread2, NULL, thread_function_2, (void*)&deadlock);

    /* Wait for threads to finish */
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    /* Destroy mutexes */
    pthread_mutex_destroy(&mutex1.mutex);
    pthread_mutex_destroy(&mutex2.mutex);

    return 0;
}

/*
 * Deadlock Avoidance:
 * 1. Always lock mutexes in the same order across all threads.
 * 2. Use a try-lock mechanism where possible to avoid waiting indefinitely.
 * 3. Implement timeout logic when trying to acquire locks.
 * 4. Consider using higher-level concurrency primitives such as semaphores or condition variables that can avoid deadlocks.
 */