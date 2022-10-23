#include <stdio.h> 

static char const rcsid[] = "$Id: f.c,v 1.5 1999/08/27 23:36:42 peter Exp $";

int main() {
    return printf("%s\n", rcsid) == EOF;
}