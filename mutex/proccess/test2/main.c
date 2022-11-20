#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

#include "libtest.h"

int main(int argc, char const *argv[]) {

    if( init() != 0 ){
        return 1;
    }

    // To make mutex is shared with some processes, processes must be forked.
    pid_t pid = fork();
    // For child:  return 0 for child.
    // For Parent: return pid of child

    lib_func( "1", pid );
    lib_func( "2", pid );

    // parent only
    if (pid != 0) {
        printf("%s: waiting...\n", pid == 0 ? "\t[child]" : "[parent]");
        // wait for child process
        wait(NULL);
        printf("%s: after wait()\n", pid == 0 ? "\t[child]" : "[parent]");

        terminate();
    }
}