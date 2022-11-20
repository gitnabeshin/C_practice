main.out: main.c libtest.so
	gcc -L./ -o main.out main.c -ltest

libtest.so: libtest.c
	gcc -shared -o libtest.so libtest.c

clean:
	rm ./libtest.so ./main.out

