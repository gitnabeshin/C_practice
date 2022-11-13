#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

// #define FILE_SIZE (1048576) //1Mbyte
#define FILE_SIZE (50)
static char line_buff[32];

#define LOG_FILE1 "./log1.txt"
#define LOG_FILE2 "./log2.txt"

static FILE* pFile;
static FILE* pFile1;
static FILE* pFile2;

static int plog_index;
static int current_buf_no;

int log_init(void) {

    system("rm ./log1.txt");
    system("rm ./log2.txt");

    pFile1 = fopen(LOG_FILE1, "w");
    pFile2 = fopen(LOG_FILE2, "w");

    pFile = pFile1;
    plog_index = 0;
    current_buf_no = 1;

    return 0;
}

void log_end(void) {
    fclose(pFile1);
    fclose(pFile2);
    plog_index = 0;
}

void switch_buffer(void) {
    if(current_buf_no == 1) {
        pFile = pFile2;
        current_buf_no = 2;
    } else {
        pFile = pFile1;
        current_buf_no = 1;
    }

    fseek(pFile, 0L, SEEK_SET);
    plog_index = 0;
}

void print_start_log(const char* format, va_list args) {
    long available = FILE_SIZE - plog_index;
    long ret = vsnprintf(line_buff, sizeof(line_buff), format, args);
    if(ret > sizeof(line_buff)){
        ret = sizeof(line_buff);
    }

    if(ret > available){
        // Full (all str is not written. cut on the way.)
        printf("Buffer Full. input:%ld, avaliable:%ld/%d.\n", ret, available, FILE_SIZE );
    } else {
        // OK
        ret = fwrite(line_buff, ret, 1, pFile);
        plog_index = plog_index + ret;
        available = FILE_SIZE - plog_index;
        if(available >= 1 ){
            ret = fprintf(pFile, "%c", '\n');
            plog_index = plog_index + ret;
        }
    }
}

void my_log(const char *format, ...) {

    long available = FILE_SIZE - plog_index;
    long ret = 0;

    va_list args;
    va_start(args, format);
    ret = vsnprintf(line_buff, sizeof(line_buff), format, args);
    va_end(args);

    if(ret > sizeof(line_buff)){
        printf("Line buffer Full. input:%ld, avaliable:%ld.\n", ret, sizeof(line_buff));
        ret = sizeof(line_buff) - 1;
    }

    if(ret > available){
        // Full (all str is not written. cut on the way.)
        printf("Buffer Full. input:%ld, avaliable:%ld/%d.\n", ret, available, FILE_SIZE );
        switch_buffer();

        // write again into another log buffer
        va_start(args, format);
        print_start_log(format, args);
        va_end(args);

    } else {
        // OK
        fwrite(line_buff, ret , 1, pFile);
        plog_index = plog_index + ret;
        available = FILE_SIZE - plog_index;
        // printf("    available[%ld]\n", available);
        if(available >= 1 ){
            ret = fprintf(pFile, "%c", '\n');
            plog_index = plog_index + ret;
        }
    }
}

// test
//#define FILE_SIZE (50)
void test_1_page(void) {
    printf("= 1 =\n");
    my_log("[%d]:log_%d. %d:%s", 1, 10, 8, "abd");
    printf("= 2 =\n");
    my_log("[%d]:log_%d.", 2, 20);
    printf("= 3 =\n");
    my_log("[%d]:log_%d.", 3, 30);
}

void test_2_page(void) {
    printf("= 4 =\n");
    my_log("[%d]:log_%d.", 4, 40);
    printf("= 5 =\n");
    my_log("[%d]:log_%d.", 5, 50);
    printf("= 6 =\n");
    my_log("[%d]:log_%d.", 6, 60);
}

void test_3_page(void) {
    printf("= 7 =\n");
    my_log("[%d]:log_%d.", 7, 70);

    printf("= 8 =\n");
    my_log("[%d]:log_%d.", 8, 80);
}

void test_4_page(void) {
    printf("= 9 =\n");
    my_log("[%d]:log_%d.", 9, 90);

    printf("= 10 =\n");
    my_log("[%d]:log_%d.", 10, 100);

    printf("= 11 =\n");
    my_log("[%d]:log_%d.", 11, 110);
}

void test_overflow(void) {
    printf("= 12 =\n");
    my_log("[%d]:log_%d.123456789012345678901234567890.", 12, 120);
}

int main(void) {

    if(log_init() != 0){
        exit( EXIT_FAILURE );
    }

    test_1_page();
    test_2_page();
    test_3_page();
    test_4_page();
    test_overflow();

    log_end();

    return 0;
}