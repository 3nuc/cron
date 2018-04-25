//#include "taskfileparse.h"
#include "logger.h"
#include "daemon.h"
#include <stdio.h>

int main(int argc, char* argv[]) {
	
	int* startArr = malloc(sizeof *startArr);
	int* endArr = malloc(sizeof *endArr);
	int* sleepFor = malloc(sizeof *sleepFor);
	
	*startArr = 0;
	*endArr = 0;
	*sleepFor = 0;
	forkDaemon();

	int argsBad = checkArgs(argc, argv);
	if(argsBad) return argsBad;
		
	const char* pathToTaskfile = argv[1];
	const char* pathToOutfile = argv[2];
	int* numberOfTasks=malloc(sizeof(*numberOfTasks));
	*numberOfTasks = 0;
	printf("loading tasks\n");
	struct TASKFILE_LINE* tasks = getTaskArray(pathToTaskfile, numberOfTasks);
	printf("tasks loaded: %d\n", *numberOfTasks);
	int out = open(pathToOutfile, O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	if(out < 0) {
		syslog(LOG_ERR, "Cannot access/create output file");
		exit(EXIT_FAILURE);
	}

	write(out, "\nUruchomiono dnia: PLSADD\n", 26);
	close(out);
	
	

	printf("Running func ... \n");
	
	getTaskCurrentHourIndexRange(startArr, endArr, tasks, *numberOfTasks, sleepFor);
	
	printf("%d %d sleepfor:%d tasks:%d\n", *startArr, *endArr,*sleepFor,*numberOfTasks);
	closeLogging();

	return 0;
}
