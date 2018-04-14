#include "taskfileparse.h"
#include <stdio.h>

void handleCommand(char* commandString) {

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
		printf("%s\n", commandString);
		/*createWholesomeFork(commandString)*/
	}; 
}

int main(int argc, char* argv[]) {
	int argsBad = checkArgs(argc, argv);
	if(argsBad) return argsBad;
		
	const char* pathToTaskfile = argv[1];
	const char* pathToOutfile = argv[2];

	int* numberOfTasks=malloc(sizeof(*numberOfTasks));
	struct TASKFILE_LINE *tasks = getTaskArray(pathToTaskfile,numberOfTasks);

	for(int i = 0; i < *numberOfTasks; i++) {
		handleCommand(tasks[i].command);
	}
	return 0;
}
