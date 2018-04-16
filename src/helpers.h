#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

struct TASKFILE_LINE {
	int hour;
	int minute;
	char* command;
	int info;
};

char* strcut(char* start, char* end);

int currentHour();
int currentMinute();
int currentSecond();

int str2int(char* str);

char* removeEdgeSpaces(char* string);

struct TASKFILE_LINE parseTaskfileLine(char* line);

int stringContainsCharacter(char* string, char character);

char** splitByCharacter(char* string, int* numberOfCommandsArg, char splittingCharacter);

void printTask(struct TASKFILE_LINE task);

int checkArgs(int argc, char* argv[]);
