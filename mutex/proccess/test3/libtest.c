#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>

#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#include "libtest.h"

static pthread_mutex_t *mut;
static int shmid;
static int init_called = 0;

#define SHM_NAME "/my_mutex_test"
#define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

static int in_init(void) {
    printf("%s called.\n", "in_init");

    pthread_mutexattr_t mut_attr;

    int fd;
    size_t size = sizeof(pthread_mutex_t);

    // shm_unlink(SHM_NAME);

    if ((fd = shm_open(SHM_NAME,  O_CREAT | O_RDWR, FILE_MODE)) == -1) {
        printf("error: shm_open(%d)\n", errno);
        return 1;
    }

    if (ftruncate(fd, size) == -1) {
        printf("error: ftruncate(%d) size=%zu\n", errno, size);
        return 1;
    }

    if ((mut = mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd,
                    0)) == MAP_FAILED) {
        printf("error: mmap(%d)\n", errno);
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

void end(void) {
    printf("%s called.\n", "end");

    size_t size = sizeof(pthread_mutex_t);
    if (munmap(mut, size) == -1) {
        printf("error: munmap(%d)\n", errno);
    }
}

void terminate(void) {
    printf("%s called.\n", "terminate");

    if (shm_unlink(SHM_NAME) != 0) {
        printf("error: shm_unlink(%d)\n", errno);
    }
}
