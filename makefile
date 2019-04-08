CFLAGS=-Wall -Werror

all: llfs

llfs: File.o test01.o test02.o
	gcc $(CFLAGS) -o go File.o test01.o test02.o

File.o: File.c
	gcc -c $(CFLAGS) io/File.c -lm

test01.o: test01.c
	gcc -c $(CFLAGS) apps/test01.c

test02.o: test02.c
	gcc -c $(CFLAGS) apps/test02.c

clean:
	rm -rf *.o main