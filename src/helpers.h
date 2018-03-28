#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>


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

struct TASKFILE_LINE parseTaskfileLine(char* line);

void printTask(struct TASKFILE_LINE task);
