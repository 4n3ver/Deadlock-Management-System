#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "class_thread.h"

class_thread_t tid[2];
int counter;
class_mutex_t lock;

void* doSomeThing(void *arg)
{
    unsigned long i = 0;
    counter += 1;
    printf("\n Job %d started\n", counter);

    for(i=0; i<(0xFFFFFFF);i++)
    {
        class_mutex_lock(&lock);
        class_mutex_unlock(&lock);
    }

    printf("\n Job %d finished\n", counter);

    return NULL;
}

int main(void)
{
    int i = 0;
    int err;

    if (class_mutex_init(&lock) != 0)
    {
        printf("\n mutex init failed\n");
        return 1;
    }

    struct timespec begints, endts;
    clock_gettime(CLOCK_MONOTONIC, &begints);
    while(i < 2)
    {
        err = class_thread_create(&(tid[i]), &doSomeThing, NULL, NULL);
        if (err != 0)
            printf("\ncan't create thread :[%s]", strerror(err));
        i++;
    }

    class_thread_join(tid[0], NULL);
    class_thread_join(tid[1], NULL);
    clock_gettime(CLOCK_MONOTONIC, &endts);
    class_mutex_destroy(&lock);

    printf("\nExe time: %lu s\n", endts.tv_sec - begints.tv_sec);

    return 0;
}