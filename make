CFLAGS=-Wall -Werror

all: main

main: File.o test01.o test02.o
    gcc $(CFLAGS) -o go File.o test01.o test02.o -lm

File.o: File.c
    gcc -c $(CFLAGS) File.c -lm

test01.o: test01.c
    gcc $(CFLAGS) -c test01.c

test02.o: test02.c
    gcc $(CFLAGS) -c test02.c

clean:
    rm -rf *.o main