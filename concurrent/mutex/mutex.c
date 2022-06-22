/***********************************************************
 * mutex test for multi process
 * *********************************************************/

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <pthread.h>

#define SHARED_FILE "/tmp/hoge.local"

static pthread_mutex_t* mutex;
static int shmid;
static bool is_created = true;

bool mutex_init(void) {
    pthread_mutexattr_t mut_attr;

    key_t key = ftok(SHARED_FILE, 1);
    if( key == -1 ) {
        return false;
    }

    shmid = shmget(key, sizeof(pthread_mutex_t), IPC_CREAT | IPC_EXCL | 0600);
    if (shmid < 0) {
        if (errno != EEXIST) {
            printf("ERROR: shmget errno=%d", errno);
            return false;
        }

        printf("shm already created.");
        shmid = shmget(key, sizeof(pthread_mutex_t), 0600);
        is_created = false;
    }

    mutex = (pthread_mutex_t*)shmat(shmid, NULL, 0);
    if ((int)mutex == -1) {
        printf("ERROR: shmat errno=%d", errno);
        return false;
    }

    if (is_created) {
        if( pthread_mutexattr_init(&mut_attr) != 0 ) {
            return false;
        }

        // set mutex to share with processes
        if (pthread_mutexattr_setpshared(&mut_attr, PTHREAD_PROCESS_SHARED) != 0) {
            return false;
        }

        if (pthread_mutex_init(mutex, &mut_attr) != 0) {
            return false;
        }
    }

    return true;
}


void mutex_test(int delay) {
    for (int i = 0; i < 10; i++) {

        printf("[%d] lockeding....\n", i);

        if (pthread_mutex_lock(mutex) != 0) {
            printf("ERROR: pthread_mutex_lock errno=%d", errno);
            return;
        }
        printf("[%d] locked\n", i);

        sleep(delay);

        if (pthread_mutex_unlock(mutex) != 0) {
            printf("ERROR: pthread_mutex_unlock errno=%d", errno);
            return;
        }
        printf("[%d] unlocked\n", i);

        sleep(1);
    }
}

int main(int argc, char *argv[]) {

    if(argc != 2) {
        printf("Usage: input delay time(sec)");
        return -1;
    }

    int delay = atoi(argv[1]);

    if( !mutex_init() ) {
        if(is_created) {
            shmctl(shmid, IPC_RMID, NULL); // remove shm
            return -1;
        }
    }

    mutex_test(delay);

    pthread_mutex_destroy(mutex);
    shmctl(shmid, IPC_RMID, NULL); // remove shm
    return 0;
}