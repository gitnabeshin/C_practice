#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>

#include "libtest.h"

static pthread_mutex_t *mut;
static int shmid;
static int init_called = 0;

static int in_init(void) {
    printf("%s called.\n", "in_init");

    pthread_mutexattr_t mut_attr;

    // create shm
    shmid = shmget(IPC_PRIVATE, sizeof(pthread_mutex_t), 0600);
    if (shmid == -1) {
        printf("error: shmget(%d)\n", errno);
        return 1;
    }

    // attach shm
    mut = shmat(shmid, NULL, 0);
    if ((int)mut == -1) {
        printf("error: shmat(%d)\n", errno);
        return 1;
    }

    pthread_mutexattr_init(&mut_attr);
    if (pthread_mutexattr_setpshared(&mut_attr, PTHREAD_PROCESS_SHARED) != 0) {
        printf("error: pthread_mutexattr_setpshared(%d)\n", errno);
        return 1;
    }

    pthread_mutex_init(mut, &mut_attr);
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
    printf("%s: [lib] called(%s) #################### \n", pid == 0 ? "\t[child]" : "[parent]", str);

    printf("%s: before pthread_mutex_lock()\n", pid == 0 ? "\t[child]" : "[parent]");
    if (pthread_mutex_lock(mut) != 0) {
        printf("error: pthread_mutex_lock(%d)\n", errno);
    }

    printf("%s: press enter to unlock...\n\n", pid == 0 ? "\t[child]" : "[parent]");
    getchar();

    if (pthread_mutex_unlock(mut) != 0) {
        printf("error: pthread_mutex_unlock(%d)\n", errno);
    }
    printf("%s: after pthread_mutex_unlock() ==== \n", pid == 0 ? "\t[child]" : "[parent]");
    printf("%s: [lib] end    #################### \n", pid == 0 ? "\t[child]" : "[parent]");
}

void terminate(void) {
    printf("%s called.\n", "terminate");

    // detouch shm
    if (shmdt(mut) != 0) {
        printf("error: shmdt(%d)\n", errno);
    }

    // remove shm
    if (shmctl(shmid, IPC_RMID, NULL) != 0) {
        printf("error: shmctl(%d)\n", errno);
    }
}
