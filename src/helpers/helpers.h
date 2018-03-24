#include <time.h>
#include <stdlib.h>

char *currentTime();

int currentHour();
int currentMinute();
int currentSecond();

long long str2int(char*);

struct TASKFILE_LINE;

struct TASKFILE_LINE parseTaskfileLine(char*);

void generateTestingFile();
