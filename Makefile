CC = gcc

all: helpers.o taskfileparse.o
	$(CC) src/main.c -o minicron helpers.o -lm

debug: helpersDEBUG.o
	$(CC) src/main.c -o minicrond helpersDEBUG.o -lm -g

helpers.o: src/helpers/helpers.c src/helpers/helpers.h
	$(CC) src/helpers/helpers.c -c -o $@ -lm

helpersDEBUG.o: src/helpers/helpers.c src/helpers/helpers.h
	$(CC) src/helpers/helpers.c -c -o $@ -lm -g

clean:
	rm -f helpers.o helpersDEBUG.o minicron minicrond

taskfileparse.o: src/taskfileparse/taskfileparse.c src/taskfileparse/taskfileparse.h
	$(CC) src/taskfileparse/taskfileparse.c -c -o $@

taskfileparse.o: src/taskfileparse/taskfileparse.c src/taskfileparse/taskfileparse.h
	$(CC) src/taskfileparse/taskfileparse.c -c -o $@ -g
