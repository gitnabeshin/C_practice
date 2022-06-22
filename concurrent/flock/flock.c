#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/file.h>

#define SHARED_FILE "/tmp/hoge.local"

int main(int argc, char *argv[]) {
    int fd;
    char buff[100];
    int i;

    if(argc != 3) {
        printf("Usage: [1]:delay(sec), [2]:p_name");
        return -1;
    }

    int delay = atoi(argv[1]);

    if ((fd = open(SHARED_FILE, O_WRONLY | O_APPEND)) != -1) {
        for (i = 0; i < 10; i++) {
            printf("locking... [%d]\n", i);
            if (flock(fd, LOCK_EX) == 0) {
                sprintf(buff, "[%s]: %d\n", argv[2], i);
                write(fd, buff, strlen(buff));
                printf("writing [%d]\n", i);
                sleep(delay);

                flock(fd, LOCK_UN);
                sleep(1);
            }
        }
        close(fd);
    } else {
        printf("ERROR: open errno=[%d]\n", errno);
    }
    return 0;
}