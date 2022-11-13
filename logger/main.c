#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

// #define BUFF_SIZE (1048576) //1Mbyte
#define BUFF_SIZE (50)

static char* plog_buff;
static char* plog_buff1;
static char* plog_buff2;

static int plog_index;
static int current_buf_no;

int log_init(void) {
    plog_buff1 = calloc( BUFF_SIZE, sizeof(char) );
    plog_buff2 = calloc( BUFF_SIZE, sizeof(char) );
    if(plog_buff1 == NULL || plog_buff2 == NULL){
        return -1;
    }

    plog_buff = plog_buff1;
    plog_index = 0;
    current_buf_no = 1;

    return 0;
}

void switch_buffer(void) {
    if(current_buf_no == 1) {
        plog_buff = plog_buff2;
        current_buf_no = 2;
    } else {
        plog_buff = plog_buff1;
        current_buf_no = 1;
    }

    plog_index = 0;
}

void print_start_log(const char* format, va_list args) {
    long available = BUFF_SIZE - plog_index;
    long ret = vsnprintf(plog_buff + plog_index, available, format, args);

    if(ret > available){
        // Full (all str is not written. cut on the way.)
        printf("Buffer Full. input:%ld, avaliable:%ld/%d.\n", ret, available, BUFF_SIZE );
    } else {
        // OK
        plog_index = plog_index + ret;
        available = BUFF_SIZE - plog_index;
        if(available >= 1 ){
            plog_buff[plog_index] = '\n';
            plog_index = plog_index + 1;
        }
    }
}

void my_log(const char *format, ...) {

    long available = BUFF_SIZE - plog_index;
    long ret = 0;

    va_list args;
    va_start(args, format);
    ret = vsnprintf(plog_buff + plog_index, available, format, args);
    va_end(args);

    if(ret > available){
        // Full (all str is not written. cut on the way.)
        printf("Buffer Full. input:%ld, avaliable:%ld/%d.\n", ret, available, BUFF_SIZE );
        switch_buffer();

        // write again into another log buffer
        va_start(args, format);
        print_start_log(format, args);
        va_end(args);
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
}

void log_end(void){
    free(plog_buff1);
    free(plog_buff2);
    plog_index = 0;
}

void dump_log(void){
    printf("[current buf=%d]\n", current_buf_no);
    printf("--- buf1 ---\n%s\n", plog_buff1);
    printf("--- buf2 ---\n%s\n------------\n\n", plog_buff2);
}

// test
//#define BUFF_SIZE (50)
void test_1_page(void) {
    printf("= 1 =\n");
    my_log("[%d]:log_%d. %d:%s", 1, 10, 8, "abd");
    printf("= 2 =\n");
    my_log("[%d]:log_%d.", 2, 20);
    printf("= 3 =\n");
    my_log("[%d]:log_%d.", 3, 30);

    dump_log();
}

void test_2_page(void) {
    printf("= 4 =\n");
    my_log("[%d]:log_%d.", 4, 40);
    printf("= 5 =\n");
    my_log("[%d]:log_%d.", 5, 50);
    printf("= 6 =\n");
    my_log("[%d]:log_%d.", 6, 60);

    dump_log();
}

void test_3_page(void) {
    printf("= 7 =\n");
    my_log("[%d]:log_%d.", 7, 70);

    printf("= 8 =\n");
    my_log("[%d]:log_%d.", 8, 80);

    dump_log();
}

void test_4_page(void) {
    printf("= 9 =\n");
    my_log("[%d]:log_%d.", 9, 90);

    printf("= 10 =\n");
    my_log("[%d]:log_%d.", 10, 100);

    printf("= 11 =\n");
    my_log("[%d]:log_%d.", 11, 110);

    dump_log();
}

int main(void) {

    if(log_init() != 0){
        exit( EXIT_FAILURE );
    }

    test_1_page();
    test_2_page();
    test_3_page();
    test_4_page();

    log_end();

    return 0;
}