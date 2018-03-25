#define MAX_CRON_TASKS 100 //how many lines there can be in the taskfile
#include "taskfileparse.h"
struct TASKFILE_LINE *getTaskArray(char** linesFromTaskfile) {
	struct TASKFILE_LINE result[MAX_CRON_TASKS];
	
	static const struct TASKFILE_LINE nullLine;
	
	for(int i = 0; i < MAX_CRON_TASKS; i++) {
		result[i]=nullLine;
	}
	
	int numberOfLines = sizeof(linesFromTaskfile)/sizeof(char)*MAX_CRON_TASKS;

	int parsingLessLinesThanMAXCRONTASKSAllows=numberOfLines<MAX_CRON_TASKS;
	assert(parsingLessLinesThanMAXCRONTASKSAllows);
	
	for(int i = 0; i < numberOfLines;i++) {
		result[i]=parseTaskfileLine(linesFromTaskfile[i]);
	}
	return result;
}


char* _getTaskfileContents(char* pathToTaskfile) {
	int fileDescriptor = open(pathToTaskfile, O_RDONLY);

	//add error handling later (for opening the file)

	
	int fileLengthInBytes=lseek(fileDescriptor, 0L, SEEK_END); //go to end of file
	lseek(fileDescriptor,0L,SEEK_SET); //cursor back to beginning of file

	char *fileContents = malloc(fileLengthInBytes+1); //memory leak call the cops
	read(fileDescriptor, fileContents, fileLengthInBytes); //no idea what the 1,f is
	close(fileDescriptor);

//	fileContents[fileLengthInBytes-1]='/0';


	return fileContents; 
	
	
}

char** _convertLineStringIntoLineArray(char* reallyLongString) { //splits a long string by \n
	char result[MAX_CRON_TASKS][100];

	
	for(int i = 0; i < MAX_CRON_TASKS; i++) {
		strcpy(result[i], "");
	}


	const int stringLength = strlen(reallyLongString);

	/*int numberOfLines = 1;
	for(int i = 0; i<stringLength; i++) 
		if(reallyLongString[i]=='\n') 
			numberOfLines++;*/

	int lineCount = 0;
	int characterCount = 0;

	
	
	for(int i = 0; i < stringLength; i++) {
		char currentCharacter = reallyLongString[i];
		if(currentCharacter=='\n') {
			lineCount++;
			result[lineCount][characterCount]='\0';
			characterCount=0;
		}
		else {
			result[lineCount][characterCount] = currentCharacter;
			characterCount++;
		}
	}

	return result;
}
