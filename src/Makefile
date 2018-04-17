CC = gcc -std=c11
objects = helpers.o taskfileparse.o logger.o daemon.o main.o
CFLAGS = -g -lm
name = minicron

$(name): $(objects)
	$(CC) -o $@ $(objects) $(CFLAGS)

main.o: main.c

helpers.o: helpers.h helpers.c

taskfileparse.o: taskfileparse.h taskfileparse.c

logger.o: logger.h logger.c

daemon.o: daemon.h daemon.c

clean:
	rm -f $(objects)

purge:
	rm -f $(objects) $(name)
