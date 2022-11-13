#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

// #define BUFF_SIZE (1048576) //1Mbyte
#define BUFF_SIZE (50) //1Mbyte

static char* plog_buff;

static int plog_index;

int log_init(void){
    plog_buff = calloc( BUFF_SIZE, sizeof(char) );
    if(plog_buff == NULL){
        return -1;
    }

    plog_index = 0;
    return 0;
}

int my_log(const char *format, ...) {

    long available = BUFF_SIZE - plog_index;
    long ret = 0;

    va_list args;
    va_start(args, format);
    ret = vsnprintf(plog_buff + plog_index, available, format, args);
    va_end(args);

    if(ret > available){
        // Full (all str is not written. cut on the way.)
        printf("Buffer Full. input:%ld, avaliable:%ld/%d.\n", ret, available, BUFF_SIZE );
    } else {
        // OK
        plog_index = plog_index + ret;
        available = BUFF_SIZE - plog_index;
        // printf("    available[%ld]\n", available);
        if(available >= 1 ){
            plog_buff[plog_index] = '\n';
            plog_index = plog_index + 1;
        }
    }

    return plog_index;
}

void log_end(void){
    free(plog_buff);
    plog_index = 0;
}

int main(void) {

    if(log_init() != 0){
        exit( EXIT_FAILURE );
    }
 
    my_log("[%d]:log_%d. %d:%s", 1, 10, 8, "abd");
    my_log("[%d]:log_%d.", 2, 20);

    printf("=== 3 ===\n");
    my_log("[%d]:log_%d.", 3, 30);
    printf("%s\n", plog_buff);

    printf("=== 4 ===\n");
    my_log("[%d]:log_%d.", 4, 40);
    printf("%s\n", plog_buff);

    printf("=== 5 ===\n");
    my_log("[%d]:log_%d.", 5, 50);
    printf("%s\n", plog_buff);

    log_end();

    return 0;
}