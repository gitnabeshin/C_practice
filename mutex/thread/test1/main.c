#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>

#define THREADS (2)

void *func( void* pArg ) {
    pthread_mutex_t* pmut = (pthread_mutex_t*)pArg;
    printf("before pthread_mutex_lock()\n");
    if (pthread_mutex_lock(pmut) != 0) {
        printf("error: pthread_mutex_lock(%d)\n", errno);
    }

    printf("    press enter to unlock...\n");
    getchar();

    if (pthread_mutex_unlock(pmut) != 0) {
        printf("error: pthread_mutex_unlock(%d)\n", errno);
    }
    printf("after pthread_mutex_unlock()\n");

    return NULL;
}

int main( void ) {

    pthread_t threads[THREADS];
    int i = 0;

    pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_init(&mut, NULL);

    for (i = 0; i < THREADS; i++) {
        if (pthread_create(&threads[i], NULL, func, &mut) != 0) {
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

    pthread_mutex_destroy(&mut);

    return 0;
}