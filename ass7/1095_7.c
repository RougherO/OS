/**
 *
 * Name:
 * Roll:
 *
 * Date:
 *
 * Assignment:

    Consider a main process which creates three threads Th1, Th2, and Th3. The main process also creates two random quantities (X, Y), both less than 10.
    These two values will be placed by the main process in the shared memory (One variant of IPC Primitive)  that is accessible by all the three threads Th1, Th2 and Th3.
    The shared memory will be created by the main process also. For each pair of values (X,Y), it is required that some computations will be done by various threads.
    The thread Th1 will compute A (X*Y) and the thread Th2 will compute B (X*Y)/2). Similarly, Th3 computes C (X+Y), Th2 again computes D ((X*Y)/(X+Y)), and finally Th1 computes E ((X+Y)(X-Y)).
    All these values are kept in the shared memory in a tabular fashion as shown below. The number of (X,Y) pairs will be taken as an argument from the CLI.
    It is the responsibility of the main process to populate required numbers of (X,Y)s in the shared memory. The program will only exit when all A,B,C etc. are computed for all given (X,Y) values.
    Before exiting, all (X,Y)s, As, Bs etc. should be displayed. Whenever, the threads complete one phase of computations (A, B, C, D and E), they will go for another pair of (X,Y) values;
    but they will start all together. This can be achieved by proper synchronization.

    Use the proper shell command to display the Shared Memory Status/Info/Statistics and attach this sample output as a comment.

    Example:—

        Input: N, number of random pairs
        Output Format:

            Pairs(X,Y)  |	A	|	B	|	C	|	D	|	E
            ————————————————————————————————————————————————————
                (1, 2) 	|	2	|	1	|	3	|	.66	|	-3
            ————————————————————————————————————————————————————
                (4, 1)  |	4	|	2	|	5	|	.8	|	15
 *
 * Input Description: Number of pairs
 *
 * Output Description: Prints the table of all pairs
 *
 *
 * Compilation Command: gcc 1095_7.c
 * Execution Sequence: ./a.out 10
 *
 *
 * Sample Input:
 * Sample Output:
 /----------------------------------------------------------------------------------------------------------------------------------------------
    |          Pairs(X, Y) |                    A |                    B |                    C |                    D |                    E |
    |       (1.033, 1.033) |                1.285 |                0.642 |                2.277 |                0.564 |               -0.482 |
    |       (6.979, 6.979) |               16.552 |                8.276 |                9.351 |                1.770 |               43.082 |
    |       (2.829, 2.829) |                9.652 |                4.826 |                6.241 |                1.547 |               -3.631 |
    |       (1.478, 1.478) |               12.174 |                6.087 |                9.714 |                1.253 |              -65.643 |
    |       (6.694, 6.694) |               31.516 |               15.758 |               11.402 |                2.764 |               22.650 |
    |       (7.410, 7.410) |               65.568 |               32.784 |               16.259 |                4.033 |              -23.405 |
    |       (5.828, 5.828) |               28.732 |               14.366 |               10.758 |                2.671 |                9.656 |
    |       (6.916, 6.916) |               10.046 |                5.023 |                8.368 |                1.200 |               45.718 |
    |       (8.542, 8.542) |                4.905 |                2.452 |                9.116 |                0.538 |               72.640 |
    |       (4.271, 4.271) |               19.848 |                9.924 |                8.918 |                2.226 |               -3.354 |
 -----------------------------------------------------------------------------------------------------------------------------------------------/
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stddef.h>
#include <pthread.h>
#include <sys/shm.h>

/* Data Structure */
typedef struct {
    float X;
    float Y;
} Key;

typedef struct {
    float A;
    float B;
    float C;
    float D;
    float E;
} Value;

typedef struct {
    Key key;
    Value value;
} Record;

typedef struct {
    Record* records;
    size_t size;
} Table;

/* Random float generator */
float get_rand()
{
    return (rand() / (float)RAND_MAX /* put it in 0..1 range */) * 10;
}

/* Global Variables */
pthread_barrier_t barrier;

void* t1_func(void* arg)
{
    Table* table = (Table*)arg;

    for (size_t index = 0; index != table->size; ++index) {
        pthread_barrier_wait(&barrier);
        float X = table->records[index].key.X;
        float Y = table->records[index].key.Y;

        table->records[index].value.A = X * Y;
        table->records[index].value.E = (X + Y) * (X - Y);
    }

    return NULL;
}

void* t2_func(void* arg)
{
    Table* table = (Table*)arg;

    for (size_t index = 0; index != table->size; ++index) {
        pthread_barrier_wait(&barrier);
        float X = table->records[index].key.X;
        float Y = table->records[index].key.Y;

        table->records[index].value.B = (X * Y) / 2;
        table->records[index].value.D = (X * Y) / (X + Y);
    }

    return NULL;
}

void* t3_func(void* arg)
{
    Table* table = (Table*)arg;

    for (size_t index = 0; index != table->size; ++index) {
        pthread_barrier_wait(&barrier);
        float X = table->records[index].key.X;
        float Y = table->records[index].key.Y;

        table->records[index].value.C = X + Y;
    }
}

int main(int argc, char** argv)
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <number_of_pairs>\n", argv[0]);
        exit(1);
    }

    int n = strtol(argv[1], NULL, 10);
    if (n < 1) {
        fprintf(stderr, "Number of pairs should be greater than 1\n");
        exit(1);
    }

    /* init */
    srand(time(NULL));
    pthread_barrier_init(&barrier, NULL, 3);
    /* init over */

    int shmid = shmget(IPC_PRIVATE, sizeof(Table) + n * sizeof(Record), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget failed");
        exit(1);
    }

    void* memptr = shmat(shmid, NULL, 0);
    if (memptr == (void*)-1) {
        perror("shmat failed");
        exit(1);
    }

    Table* table   = (Table*)memptr;
    table->size    = n;
    table->records = (Record*)((Table*)memptr + 1);

    for (int i = 0; i != table->size; i++) {
        table->records[i].key.X = get_rand();
        table->records[i].key.Y = get_rand();
    }

    pthread_t threads[3];

    /* start the threads */
    pthread_create(&threads[0], NULL, t1_func, (void*)table);
    pthread_create(&threads[1], NULL, t2_func, (void*)table);
    pthread_create(&threads[2], NULL, t3_func, (void*)table);

    /* join threads */
    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);
    pthread_join(threads[2], NULL);

    /* pretty printing */
    int const width        = 20;
    char const* headings[] = { "Pairs(X, Y)", "A", "B", "C", "D", "E" };
    printf("| %*s | %*s | %*s | %*s | %*s | %*s |\n",
           width, headings[0],
           width, headings[1],
           width, headings[2],
           width, headings[3],
           width, headings[4],
           width, headings[5]);

    char line[256] = { 0 };
    for (size_t i = 0; i != table->size; i++) {
        sprintf(line, "(%.3f, %.3f)", table->records[i].key.X, table->records[i].key.X);
        printf("| %*s ", width, line);
        sprintf(line, "%.3f", table->records[i].value.A);
        printf("| %*s ", width, line);
        sprintf(line, "%.3f", table->records[i].value.B);
        printf("| %*s ", width, line);
        sprintf(line, "%.3f", table->records[i].value.C);
        printf("| %*s ", width, line);
        sprintf(line, "%.3f", table->records[i].value.D);
        printf("| %*s ", width, line);
        sprintf(line, "%.3f", table->records[i].value.E);
        printf("| %*s |\n", width, line);
    }

    /* clean up */
    shmdt(memptr);
    shmctl(shmid, IPC_RMID, NULL);
    pthread_barrier_destroy(&barrier);
    /* clean up over */
}