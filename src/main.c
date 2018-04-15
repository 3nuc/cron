//#include "taskfileparse.h"
#include "logger.h"
#include "daemon.h"
#include <stdio.h>

void handleCommand(char* commandString, int outtype) {

	if(stringContainsCharacter(commandString,'|')) { //if command is like 'program1 | program2'
		int* numberOfPipedCommands=malloc(sizeof *numberOfPipedCommands);
		char** commands = splitByPipe(commandString, numberOfPipedCommands);
			
		for(int j = 0; j<*numberOfPipedCommands; j++) {
			//createWholesomeThreadFor(commandString);
			printf("%s\n",commands[j]);
		}
		
		free(commands);
	}
	
	else {  //for normal commands (without pipe)
		executeCommand(commandString,2);
	}; 
}

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

	write(out, "yay\n", 4);

	close(out);

	//printf("FeelsGoodMan with my pid %d", getpid());
	handleCommand("woohoo", 2);

	closeLogging();

	return 0;

	// int* numberOfTasks=malloc(sizeof(*numberOfTasks));
	// struct TASKFILE_LINE *tasks = getTaskArray(pathToTaskfile,numberOfTasks);

	// for(int i = 0; i < *numberOfTasks; i++) {
	// 	handleCommand(tasks[i].command);
	// }
	// return 0;
}
