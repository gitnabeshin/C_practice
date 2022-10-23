#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include <sys/socket.h>

int main(void)
{
    printf("RAND_MAX =%d\n", RAND_MAX);
    printf("INT_MAX  =%d\n", INT_MAX);
    printf("UINT_MAX =%u\n", UINT_MAX);
    printf("LONG_MAX =%ld\n", LONG_MAX);
    printf("ULONG_MAX=%lu\n\n", ULONG_MAX);

    int a=0;
    char c[] = "abc";
    printf("int  p=%p, a=%d \n", &a, a);
    printf("char p=%p, c=%s size=%ld\n\n", &c, c, sizeof(c));

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    printf("sock p=%p \n", &sock);
    printf("&sock+2   p=%p \n", &sock+2);
    printf("&sock+133 p=%p \n", &sock+133);
    printf("&sock+133 - &sock+2 =%ld \n", (&sock+133) - (&sock+2) );
    printf("sock_size=%ld, &sock_size=%ld \n\n", sizeof(sock), sizeof(&sock));

    char buf[128];
    memset(&buf, 0, sizeof(buf));
    sprintf(buf, "%p.txt", &sock);
    printf("name=%s\n", buf);
    printf("name=%s\n", buf + 2);

    // FILE *fp = fopen("hoge.txt", "w+");
    FILE *fp = fopen(buf+2, "w+");
    fprintf(fp, "this is %s\n", c);
    printf("FILE p=%p \n", fp);
    printf("ptr_len=%ld \n\n", sizeof(fp));

    fclose(fp);
    // unlink(buf+2);

    close(sock);

    return 0;
}