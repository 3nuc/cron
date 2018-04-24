//#include "taskfileparse.h"
#include "logger.h"
#include "daemon.h"
#include <stdio.h>

int main(int argc, char* argv[]) {

	forkDaemon();

	int argsBad = checkArgs(argc, argv);
	if(argsBad) return argsBad;
		
	const char* pathToTaskfile = argv[1];
	const char* pathToOutfile = argv[2];
	int* numberOfTasks=malloc(sizeof(*numberOfTasks));
	*numberOfTasks = 0;
	struct TASKFILE_LINE* tasks = getTaskArray(pathToTaskfile, numberOfTasks);
	printf("tasks loaded\n");

	int out = open(pathToOutfile, O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	if(out < 0) {
		syslog(LOG_ERR, "Cannot access/create output file");
		exit(EXIT_FAILURE);
	}

	write(out, "\nUruchomiono dnia: PLSADD\n", 26);
	close(out);
	
	printf("Initializing malloc vars\n");
	int *startArr = malloc(sizeof (*startArr));
	int *endArr = malloc(sizeof (*endArr));
	int *sleepFor = malloc(sizeof (*sleepFor));
	
	printf("Malloc done, now init to 0\n");
	*startArr = 0;
	*endArr = 0;
	*sleepFor=0;

	printf("Running func ... \n");
	
	getTaskCurrentHourIndexRange(startArr, endArr, tasks, *numberOfTasks, sleepFor);
	
	free(startArr);
	free(endArr);
	free(slepFor);
	
	
	printf("%d %d tasks:%d", *startArr, *endArr,*numberOfTasks);
	handleCommand("ls -l / | wc -l | wc | wc -l | wc | wc", pathToOutfile, 2);
	handleCommand("ls -l /", pathToOutfile, 2);
	handleCommand("ls -l / | wc -l", pathToOutfile, 2);
	handleCommand("cat /XD", pathToOutfile, 2);
	handleCommand("cat /XD | grep --line-buffered std", pathToOutfile, 2);
	handleCommand("cat /home/maxim/studia/cron/src/daemon.h | grep --line-buffered std", pathToOutfile, 2);
	closeLogging();

	return 0;
}
