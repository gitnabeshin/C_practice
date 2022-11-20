#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>

#include "libtest.h"

static pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
static int init_called = 0;

static int in_init(void) {
    printf("%s called.\n", "in_init");
    pthread_mutex_init(&mut, NULL);
    return 0;
}

int init(void) {
    if(init_called == 0) {
        init_called = 1;
        return in_init();
    }
    return 0;
}

void lib_func(const char* str, const int pid) {

    printf("before pthread_mutex_lock()\n");
    if (pthread_mutex_lock(&mut) != 0) {
        printf("error: pthread_mutex_lock(%d)\n", errno);
    }

    printf("press enter to unlock...\n\n");
    getchar();

    if (pthread_mutex_unlock(&mut) != 0) {
        printf("error: pthread_mutex_unlock(%d)\n", errno);
    }
    printf("after pthread_mutex_unlock() ==== \n");
}

void terminate(void) {
    printf("%s called.\n", "terminate");
    pthread_mutex_destroy(&mut);
}
