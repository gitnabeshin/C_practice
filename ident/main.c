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

int main(void) {
	FILE *fp;
	char buff[1024000];
    char c;
    int count = 0;

    char start_str[] = "$Id: ";
    char end_str[] = "$";

    int i = 0;
    while(start_str[i]){
        printf("**** %c \n", start_str[i++]);
    }


    char result_str[1024];
    memset(result_str, 0, sizeof(result_str));
    int result_index = 0;

    bool in_str = false;

    long size = get_file_size("libtest.out");
	printf("file size[%ld]\n", size);

/////
size = 32;

	if ((fp = fopen("libtest.out", "r")) == NULL) {
	// if ((fp = fopen("hoge.txt", "r")) == NULL) {
		printf("file open error!!\n");
		return 1;
	}
	
	while (fgets(buff, sizeof(buff), fp) != NULL) {
        c = buff[count];

        while (count < sizeof(buff)) {

            printf(" %d:  ##%d, %d \n", count, buff[count], (unsigned int)c);
            if((int)c == 0xcf){
                printf("@@@@@@@@@@@@@@@@@@@@\n");
            }

            if(c == 36) {

                printf("  @[%d][%c]\n", __LINE__, c);

                if( in_str == false && count+4 < size && buff[count+1] == 73 && 
                    buff[count+2] == 100 && buff[count+3] == 58 && buff[count+4] == 32 ) {

                    printf("  @[%d]]\n", __LINE__);
                    in_str = true;
                    count = count + 4;
                } else if(in_str == true) {
                    printf("  @[%d]]\n", __LINE__);
                    break;
                } else {
                    count++;
                }
            } else {
                count++;
            }

            if(count >= size){
                printf("EOF [%d]]\n", count);
                break;
            }

            if(in_str == true) {
                printf("  @[%d]]\n", __LINE__);
                result_str[result_index] = buff[count];
                result_index++;
            }
            c = buff[count]; 
        }

        if(count == size || result_index > 0){
            break;
        }
	}
	printf("file size[%d]]\n", count);
	printf("Result str[%s]]\n", result_str);

	fclose(fp);
	
	return 0;
}