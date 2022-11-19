#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>

int main( void ) {
    pthread_mutex_t *mut;
    pthread_mutexattr_t mut_attr;

    // create shm
    int shmid = shmget(IPC_PRIVATE, sizeof(pthread_mutex_t), 0600);
    if (shmid < 0) {
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

    pid_t pid = fork();
    // For child:  return 0 for child.
    // For Parent: return pid of child

    printf("[%s] before pthread_mutex_lock()\n", pid == 0 ? "child" : "parent");
    if (pthread_mutex_lock(mut) != 0) {
        printf("error: pthread_mutex_lock(%d)\n", errno);
        return 1;
    }

    printf("    [%s] press enter to unlock...\n", pid == 0 ? "child" : "parent");
    getchar();

    if (pthread_mutex_unlock(mut) != 0) {
        printf("error: pthread_mutex_unlock(%d)\n", errno);
        return 1;
    }
    printf("[%s] after pthread_mutex_unlock()\n", pid == 0 ? "child" : "parent");

    // detouch shm
    shmdt(mut);

    // parent only
    if (pid != 0) {
        printf("    [%s] waiting...\n", pid == 0 ? "child" : "parent");
        // wait for child process
        wait(NULL);
        printf("[%s] after wait()\n", pid == 0 ? "child" : "parent");

        // remove shm
        if (shmctl(shmid, IPC_RMID, NULL) != 0) {
            printf("error: shmctl(%d)\n", errno);
            return 1;
        }
    }

    return 0;
}