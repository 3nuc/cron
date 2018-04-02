CC = gcc
objects = helpers.o taskfileparse.o main.o logger.o
flags = -g -lm
name = minicron

$(name): $(objects)
	$(CC) -o $@ $(objects) $(flags)

main.o: main.c

helpers.o: helpers.h helpers.c

taskfileparse.o: taskfileparse.h taskfileparse.c

logger.o: logger.h logger.c

clean:
	rm -f $(objects)

purge:
	rm -f $(objects) $(name)
