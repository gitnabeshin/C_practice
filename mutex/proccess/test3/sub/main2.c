#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

#include "libtest.h"

int main(int argc, char const *argv[]) {

    if( init() != 0 ){
        return 1;
    }

    printf("=========================\n");
    printf("[SUB] pid=%d, start.\n", getpid());
    printf("=========================\n");

    lib_func( "1", getpid() );
    lib_func( "2", getpid() );

    printf("=========================\n");
    printf("[SUB] pid=%d, end.\n", getpid());
    printf("=========================\n");

    end();
    terminate();

    return 0;
}