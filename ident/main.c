#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

/*
00003f80  24 49 64 3a 20 66 2e 63  2c 76 20 31 2e 35 20 31  |$Id: f.c,v 1.5 1|
00003f90  39 39 39 2f 30 38 2f 32  37 20 32 33 3a 33 36 3a  |999/08/27 23:36:|
00003fa0  34 32 20 70 65 74 65 72  20 45 78 70 20 24 00 00  |42 peter Exp $..|
*/

long get_file_size(const char *file) {
    struct stat statBuf;
    if (stat(file, &statBuf) == 0) {
        return statBuf.st_size;
    }
    return -1L;
}

int main(int argc, char *argv[]) {

    if(argc != 2) {
        printf("Usage $a.out [filename] \n");
        return 0;
    }

    char *file_path = argv[1];
    FILE *fp;
    char buff[8192];
    memset(buff, 0, sizeof(buff));
    char c;
    int count = 0;

    char result_str[1024];
    memset(result_str, 0, sizeof(result_str));
    int result_index = 0;

    bool in_str = false;
    bool finished = false;

    long size = get_file_size(file_path);

    if ((fp = fopen(file_path, "r")) == NULL) {
        printf("file open error!!\n");
        return 1;
    }

    while (fgets(buff, sizeof(buff), fp) != NULL) {
        c = buff[count];
        count = 0;

        while (count < sizeof(buff)) {

            if(count >= size){
                printf("EOF [%d]]\n", count);
                break;
            }

            if( in_str == false && c == (char)0x24) {
                if( in_str == false && count+4 < size &&
                    buff[count+1] == (char)0x49 &&
                    buff[count+2] == (char)0x64 &&
                    buff[count+3] == (char)0x3a &&
                    buff[count+4] == (char)0x20 ) {
                    in_str = true;
                    count = count + 4;
                } else {
                    count++;
                }
            } else {
                count++;
            }

            if(in_str == true) {
                if(buff[count] == (char)0x24) {
                    finished = true;
                    break;
                }
                result_str[result_index] = buff[count];
                result_index++;
            }
            c = buff[count]; 
        }

        if( finished == true || count == size ) {
            break;
        }
    }
    printf("Result str[%s]\n", result_str);

    fclose(fp);
    return 0;
}