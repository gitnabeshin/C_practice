# pthread mutex in shared library

## Point

```
    // To make mutex is shared with some processes, processes must be forked.
    pid_t pid = fork();
```

Run test3/main.out, test3/sub/main2.out

But not worked. because mutex object is different.

## Env

```
nabeshin@iMacNabeshin test3 % ls
Makefile        README.md       libtest.c       libtest.h       main.c          sub
nabeshin@iMacNabeshin test3 % make
gcc -shared -o libtest.so libtest.c
gcc -L./ -o main.out main.c -ltest
nabeshin@iMacNabeshin test3 % cd sub 
nabeshin@iMacNabeshin sub % make
gcc -L../ -I../ -o main2.out main2.c -ltest
cp ../libtest.so ./
nabeshin@iMacNabeshin sub % cd ../
nabeshin@iMacNabeshin test3 % ls -l
total 248
-rw-r--r--  1 nabeshin  staff    161 11 20 21:44 Makefile
-rw-r--r--  1 nabeshin  staff    158 11 20 22:32 README.md
-rw-r--r--  1 nabeshin  staff   2466 11 20 22:30 libtest.c
-rw-r--r--  1 nabeshin  staff     99 11 20 21:53 libtest.h
-rwxr-xr-x  1 nabeshin  staff  50408 11 20 22:32 libtest.so
-rw-r--r--  1 nabeshin  staff    690 11 20 22:21 main.c
-rwxr-xr-x  1 nabeshin  staff  49688 11 20 22:32 main.out
drwxr-xr-x  6 nabeshin  staff    204 11 20 22:32 sub
nabeshin@iMacNabeshin test3 % ls ./sub   
Makefile        libtest.so      main2.c         main2.out
nabeshin@iMacNabeshin test3 % 
```
