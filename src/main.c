//#include "taskfileparse.h"
#include "logger.h"
#include "daemon.h"
#include <stdio.h>

char** commandToPieces(char* cmd) {
	int spaces = 0, i;
	for( i = 0; i < strlen(cmd); i++) {
		if(cmd[i] == ' ') spaces++;
	}

	char** result = malloc(sizeof(char*)*(spaces+2));
	for( i = 0; i < spaces+2; i++) {
		result[i] = malloc(100);
	}

	int j = 0, c = 0;
	for( i = 0; i < strlen(cmd); i++) {
		if(cmd[i] == ' ') {
			result[j][c] = '\0';
			c = 0;
			j++;
		}
		else {
			result[j][c] = cmd[i];
			c++;
		}
	}
	result[j][c] = '\0';
	result[j+1] = 0;

	return result;
}

void handleCommand(char* commandString, int outtype) {

	int i;
	int* numberOfPipedCommands=malloc(sizeof *numberOfPipedCommands);
	char** commands;

	if(stringContainsCharacter(commandString,'|')) { //if command is like 'program1 | program2'
		commands = splitByPipe(commandString, numberOfPipedCommands);
	}
	else {
		*numberOfPipedCommands = 1;
		commands = malloc(sizeof(char*));
		commands = malloc(100);
		commands[0] = commandString;
		//printf("%s\n", commands[0]);
	}

	int fds[*numberOfPipedCommands * 2];
	for( i = 0; i < *numberOfPipedCommands; i++) {
		if(pipe(fds + i*2) < 0) {
			syslog(LOG_ERR, "Failed creating a pipe");
			exit(EXIT_FAILURE);
		}
	}

	char buff[4096];

	for( i = 0; i < *numberOfPipedCommands; i++) {
		// int pid = fork();

		// if( pid < 0 ) {
		// 	syslog(LOG_ERR, "Failed creating a process for the command %s", commands[i]);
		// 	exit(EXIT_FAILURE);
		// }

		//printf("%s\n", commands[i]);
		char** pieces = commandToPieces(commands[i]);
		executeCommand(pieces, 2);
	}

	for( i = 0; i < *numberOfPipedCommands*2; i++) {
		close(fds[i]);
	}

	free(commands);
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

	write(out, "yayy\n", 5);

	close(out);

	//printf("FeelsGoodMan with my pid %d", getpid());
	handleCommand("/bin/cat /home/maxim/studia/cron/src/daemon.h", 2);
	closeLogging();

	return 0;

	// int* numberOfTasks=malloc(sizeof(*numberOfTasks));
	// struct TASKFILE_LINE *tasks = getTaskArray(pathToTaskfile,numberOfTasks);

	// for(int i = 0; i < *numberOfTasks; i++) {
	// 	handleCommand(tasks[i].command);
	// }
	// return 0;
}
