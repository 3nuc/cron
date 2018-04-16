#include "taskfileparse.h"
#include <stdio.h>

void handleCommand(char* commandString) {


	if(stringContainsCharacter(commandString,'|')) { //if command is like 'program1 | program2'
		int* numberOfPipedCommands=malloc(sizeof *numberOfPipedCommands);
		char** commands = splitByCharacter(commandString, numberOfPipedCommands, '|');
			
		for(int j = 0; j<*numberOfPipedCommands; j++) {
			int* commandArgc=malloc(sizeof *commandArgc);
			char** commandArgv = splitByCharacter(removeEdgeSpaces(commands[j]), commandArgc, ' ');
			
			for(int k = 0; k < *commandArgc; k++) {
				printf("	|%s|\n", commandArgv[k]);
				//the above printf prints arguments (the same format as argv[]) of a command.
				//so "cd -d -xd" will print "cd", "-d" "-xd".
			}
		
			if(j==*numberOfPipedCommands-1) { //if this is the last cycle of this for loop
				free(commandArgc);
				free(commandArgv);
			}
			printf("\n");

		}
		
		free(commands);
		free(numberOfPipedCommands);
	}
	
	else {  //for normal commands (without pipe)
		printf("%s\n", commandString);
		/*createWholesomeFork(commandString)*/
	};
}

int main(int argc, char* argv[]) {
	int argsOK = checkArgs(argc, argv);
	if(argsOK) return argsOK;
		
	const char* pathToTaskfile = argv[1];
	const char* pathToOutfile = argv[2];

	int* numberOfTasks=malloc(sizeof(*numberOfTasks));
	struct TASKFILE_LINE *tasks = getTaskArray(pathToTaskfile,numberOfTasks);

	for(int i = 0; i < *numberOfTasks; i++) {
		handleCommand(tasks[i].command);
	}


	return 0;
}
