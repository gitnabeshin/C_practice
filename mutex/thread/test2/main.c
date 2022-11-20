#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>

#include "libtest.h"

#define THREADS (2)

void *func( void* pArg ) {

    lib_func("aaa", 1);
    lib_func("bbb", 2);

    return NULL;
}

int main( void ) {

    pthread_t threads[THREADS];
    int i = 0;

    for (i = 0; i < THREADS; i++) {
        if (pthread_create(&threads[i], NULL, func, NULL) != 0) {
        printf("Error:(%d) pthread_create %d \n", errno, i + 1);
        exit(1);
        }
    }
    
    for (i = 0; i < THREADS; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
        printf("Error:(%d) pthread_join %d \n", errno, i + 1);
        exit(2);
        }
    }

    terminate();

    return 0;
}
