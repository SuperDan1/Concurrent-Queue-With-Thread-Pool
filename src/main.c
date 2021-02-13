#define NUM_THREADS 200

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include "queue.h"

struct threadArgs
{
    struct queue *q;
    char *c;
};

void *putArg(void *params)
{
    struct threadArgs *args = params;
    pushQueue(args->q, args->c);
}

int main()
{
    pthread_t tids[NUM_THREADS]; //线程id
    uint32_t index = 10;
    struct queue *g_q;
    g_q = initQueue();
    char c[LENTH] = "test\0";
    char b[LENTH] = "btest\0";
    char a[LENTH] = "atest\0";
    char *h = "";
    int i = 0;
    for (i = 0; i < NUM_THREADS; ++i)
    {
        struct threadArgs *args;
        args = (struct threadArgs *)malloc(sizeof(struct threadArgs));
        args->q = g_q;
        args->c = c;
        pthread_create(&tids[i], NULL, putArg, args);
    }

    while (index--)
    {
        h = popQueue(g_q);
        printf("%s\n", h);
        if (strcmp(h, "0") == 0)
        {
            printf("queue is empty , sleep for a while");
            sleep(3);
        }
        else
        {
            sleep(1);
        }
    }
    return 0;
}