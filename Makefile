main: my_bcast.o
	gcc main.c my_bcast.o -O2 -o main -lmpi

my_bcast.o:
	gcc -c my_bcast.c -o my_bcast.o -lmpi
