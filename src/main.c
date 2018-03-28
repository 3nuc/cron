#include "taskfileparse.h"
#include <stdio.h>

int main() {
	
	const char* pathToTaskfile = "taskfile.test"; //this needs to be read from arguments (eg ./minicron path)
	struct TASKFILE_LINE *tasks = getTaskArray(pathToTaskfile);
	const int numberTasks = sizeof(tasks)/sizeof(char*)+1;
	
	for(int i = 0; i<numberTasks; i++)
		printTask(tasks[i]);
	
	
	
	
	//memory cleanup
	free(tasks);
	return 0;
}
