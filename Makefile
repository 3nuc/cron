CC = gcc

all: helpers.o
	$(CC) src/main.c -o minicron helpers.o -lm

debug: helpersDEBUG.o
	$(CC) src/main.c -o minicrond helpersDEBUG.o -lm -g

helpers.o: src/helpers/helpers.c src/helpers/helpers.h
	$(CC) src/helpers/helpers.c -c -o $@ -lm

helpersDEBUG.o: src/helpers/helpers.c src/helpers/helpers.h
	$(CC) src/helpers/helpers.c -c -o $@ -lm -g

clean:
	rm helpers.o helpersDEBUG.o minicron minicrond
