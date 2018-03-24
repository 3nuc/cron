CC = gcc -m32

all: helpers.o
	$(CC) src/main.c -c -o minicron

helpers.o: src/helpers/helpers.c src/helpers/helpers.h
	$(CC) src/helpers/helpers.c -c -o helpers.o
