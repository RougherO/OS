#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stddef.h>
#include <pthread.h>

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

    Table table;

    table.size    = n;
    table.records = malloc(n * sizeof(Record));

    for (int i = 0; i != table.size; i++) {
        table.records[i].key.X = get_rand();
        table.records[i].key.Y = get_rand();
    }

    pthread_t threads[3];

    /* start the threads */
    pthread_create(&threads[0], NULL, t1_func, (void*)&table);
    pthread_create(&threads[1], NULL, t2_func, (void*)&table);
    pthread_create(&threads[2], NULL, t3_func, (void*)&table);

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
    for (size_t i = 0; i != table.size; i++) {
        sprintf(line, "(%.3f, %.3f)", table.records[i].key.X, table.records[i].key.X);
        printf("| %*s ", width, line);
        sprintf(line, "%.3f", table.records[i].value.A);
        printf("| %*s ", width, line);
        sprintf(line, "%.3f", table.records[i].value.B);
        printf("| %*s ", width, line);
        sprintf(line, "%.3f", table.records[i].value.C);
        printf("| %*s ", width, line);
        sprintf(line, "%.3f", table.records[i].value.D);
        printf("| %*s ", width, line);
        sprintf(line, "%.3f", table.records[i].value.E);
        printf("| %*s |\n", width, line);
    }

    /* clean up */
    free(table.records);
    pthread_barrier_destroy(&barrier);
    /* clean up over */
}