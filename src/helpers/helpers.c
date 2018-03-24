#include "helpers.h"
//time.h
//stdlib.h




char *currentTime() {
	char * currtimestr = ctime(time(NULL)
	currtimestr[strlen(currtimestr)-1]='/0';
	return currtimestr;
}

int currentHour() {
	struct tm *tm_struct = localtime(time(NULL));
	return tm_struct->tm_hour;
}

int currentMinute() {
	struct tm *tm_struct = localtime(time(NULL));
	return tm_struct->tm_min;
}

int currentSecond() { //probably wont be needed but just in case
	struct tm *tm_struct = localtime(time(NULL));
	return tm_struct->tm_sec;
}

long long str2int(char* str) {
	return strtonum(str, 0, 100); //values are always between 0 and 100 when using this func 
}

struct TASKFILE_LINE {
	int hour;
	int minute;
	char* command;
	int info;
};

TASKFILE_LINE parseTaskfileLine(char* line) {
	TASKFILE_LINE result;	
	const char splitOn[2] = ":";
	result->hour=str2int(strtok(line, splitOn));
	result->minute=str2int(strtok(line,splitOn));
	result->command=strtok(line,splitOn);
	result->info=str2int(strtok(line);

	return result;
		
}
void generateTestingFile() { //generates taskfile.txt with random commands 1 minute, 2 minutes and 3 minutes from now
	const currentTime = currentTime(); 	
}
