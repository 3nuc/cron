//#include "taskfileparse.h"
#include "logger.h"
#include "daemon.h"
#include <stdio.h>

void handleCommand(char* commandString, const char* outPath) {

	if(stringContainsCharacter(commandString,'|')) { //if command is like 'program1 | program2'
		int* numberOfPipedCommands=malloc(sizeof *numberOfPipedCommands);
		char** commands = splitByCharacter(commandString, numberOfPipedCommands, '|');

		int fds[(*numberOfPipedCommands) * 2];	// pipes
		for(int i = 0; i < *numberOfPipedCommands; i++) {
			if(pipe(fds + i*2) < 0) {
				syslog(LOG_ERR, "Failed creating a pipe");
				exit(EXIT_FAILURE);
			}
		}

		char buff[4096];
		
		int* commandArgc;
		char** commandArgv;

		for(int j = 0; j<*numberOfPipedCommands; j++) {
			commandArgc=malloc(sizeof *commandArgc);
			commandArgv = splitByCharacter(removeEdgeSpaces(commands[j]), commandArgc, ' ');
			
			for(int k = 0; k < *commandArgc; k++) {
				printf("	|%s|\n", commandArgv[k]);
				//the above printf prints arguments (the same format as argv[]) of a command.
				//so "cd -d -xd" will print "cd", "-d" "-xd".
			}

			pid_t pid = fork();

			if( pid < 0 ) {
				syslog(LOG_ERR, "Failed creating a process for the command %s", commandArgv[0]);
				exit(EXIT_FAILURE);
			}
			if( pid > 0 ) {	// parent
				//waitpid(pid,0,0);
			}
			else {	//child

				if(j != 0) {
					dup2(fds[(j-1)*2], STDIN_FILENO);
				}
				
				dup2(fds[j*2+1], STDOUT_FILENO);
				
				syslog(LOG_NOTICE, "Forked command %s", commandArgv[0]);

				commandArgv[*commandArgc] = 0; // null termination is required at the end of argv[] in exec

				for(int i = 0; i < (*numberOfPipedCommands)*2; i++) {
					close(fds[i]);
				}

				execvp(commandArgv[0], commandArgv);

				//exec error
				syslog(LOG_ERR, "EXEC error of command %s", commandArgv[0]);
				exit(EXIT_FAILURE);
			}
		}

		for(int i = 0; i < (*numberOfPipedCommands)*2; i++) {
			if(((*numberOfPipedCommands)-1)*2 != i)
				close(fds[i]);
		}

		int out = open(outPath, O_WRONLY | O_APPEND);
		int nbytes = read(fds[((*numberOfPipedCommands)-1)*2], buff, sizeof(buff));
		write(out, buff, nbytes);

		close(out);
		close(fds[((*numberOfPipedCommands)-1)*2]);

		free(commandArgc);
		free(commandArgv);
		free(commands);
		free(numberOfPipedCommands);

		
	}
	
	else {  //for normal commands (without pipe)
		printf("%s\n", commandString);
		/*createWholesomeFork(commandString)*/
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

	write(out, "what\n", 5);

	close(out);

	//printf("FeelsGoodMan with my pid %d", getpid());
	handleCommand("ls -l / | wc -l | wc | wc -l | wc | wc", pathToOutfile);
	//handleCommand("cat /home/maxim/studia/cron/src/daemon.h | grep --line-buffered std");
	closeLogging();

	return 0;

	// int* numberOfTasks=malloc(sizeof(*numberOfTasks));
	// struct TASKFILE_LINE *tasks = getTaskArray(pathToTaskfile,numberOfTasks);

	// for(int i = 0; i < *numberOfTasks; i++) {
	// 	handleCommand(tasks[i].command);
	// }
	// return 0;
}
