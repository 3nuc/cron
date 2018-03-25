cc = gcc
objects = helpers.o taskfileparse.o main.o
flags = -g -lm
name = minicron

$(name): $(objects)
	cc -o $@ $(objects) $(flags)

main.o: main.c

helpers.o: helpers.h helpers.c

taskfileparse.o: taskfileparse.h taskfileparse.c

clean:
	rm -f $(objects)

purge:
	rm -f $(objects) $(name)
