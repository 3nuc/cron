#include "taskfileparse.h"
#include <stdio.h>
#include <errno.h>

int main(int argc, char * argv[]) {
	
	if(argc > 3) { 
		errno=E2BIG;
		perror("Too many arguments (2 needed)");
		return errno;
	}

	else if (argc<3) {
		errno=; //no error code for too few arguments btw
		fprintf(stderr, "Too few arguments (2 needed)");
		return errno;
	}

	const char* pathToTaskfile = argv[1]; //this needs to be read from arguments (eg ./minicron path)
	const char* pathToOutfile = argv[2];

	struct TASKFILE_LINE *tasks = getTaskArray(pathToTaskfile);
	const int numberOfTasks = sizeof(tasks)/sizeof(char*)+1;
	
	for(int i = 0; i<numberOfTasks; i++)
		printTask(tasks[i]);
	
	
	
	
	//memory cleanup
	free(tasks);
	return 0;
}
