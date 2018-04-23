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

	int out = open(pathToOutfile, O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	if(out < 0) {
		syslog(LOG_ERR, "Cannot access/create output file");
		exit(EXIT_FAILURE);
	}

	write(out, "what\n", 5);

	close(out);

	//printf("FeelsGoodMan with my pid %d", getpid());
	//handleCommand("ls -l / | wc -l | wc | wc -l | wc | wc", pathToOutfile);
	handleCommand("cat /XD", pathToOutfile, 2);
	//handleCommand("cat /home/maxim/studia/cron/src/daemon.h | grep --line-buffered std", pathToOutfile);
	closeLogging();

	return 0;

	// int* numberOfTasks=malloc(sizeof(*numberOfTasks));
	// struct TASKFILE_LINE *tasks = getTaskArray(pathToTaskfile,numberOfTasks);

	// for(int i = 0; i < *numberOfTasks; i++) {
	// 	handleCommand(tasks[i].command);
	// }
	// return 0;
}
