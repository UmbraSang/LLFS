CFLAGS=-Wall -Werror
INC=-I/io/
INC+=-I/apps/

all: llfs

llfs: File.o test01.o test02.o
	gcc $(CFLAGS) -o go File.o test01.o test02.o

File.o: io/File.c
	cd io
	gcc -c $(CFLAGS) File.c -lm

test01.o: apps/test01.c
	cd apps
	gcc -c $(CFLAGS) test01.c

test02.o: apps/test02.c
	cd apps
	gcc -c $(CFLAGS) test02.c

clean:
	rm -rf *.o main