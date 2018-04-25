#define MAX_CRON_TASKS 100 //how many lines there can be in the taskfile
#include "taskfileparse.h"
#include "logger.h"
#include <syslog.h>

struct TASKFILE_LINE *getTaskArray(char* pathToTaskfile, int* lineCountArg) {
	struct TASKFILE_LINE *result;
	int* linecountptr=malloc(sizeof(*linecountptr));
	
	char* taskfileContentsAsOneString = _getTaskfileContentsAsLineString(pathToTaskfile);
	char** linesFromTaskfile = _convertLineStringIntoLineArray(taskfileContentsAsOneString,linecountptr);
	
	free(taskfileContentsAsOneString);
	
	int numberOfLines = *linecountptr;
	
	result = malloc(numberOfLines * sizeof(struct TASKFILE_LINE));

	int parsingLessLinesThanMAXCRONTASKSAllows=numberOfLines<MAX_CRON_TASKS;
	assert(parsingLessLinesThanMAXCRONTASKSAllows);
	
	for(int i = 0; i <= numberOfLines;i++) {
		result[i]=parseTaskfileLine(linesFromTaskfile[i]);
	}
	for(int i =0; i < MAX_CRON_TASKS; i++) {
		free(linesFromTaskfile[i]);
	}
	*lineCountArg=numberOfLines;


	qsort(result,numberOfLines,sizeof(struct TASKFILE_LINE), _compareForQsort);
	return result;
}


char* _getTaskfileContentsAsLineString(char* pathToTaskfile) {
	int fileDescriptor = open(pathToTaskfile, O_RDONLY);
	enableLogging();
	if(fileDescriptor < 0) {
		syslog(LOG_ERR, "Cannot access/Missing taskfile");
		exit(EXIT_FAILURE);
		}
	closeLogging();

	//add error handling later (for opening the file)

	
	int fileLengthInBytes=lseek(fileDescriptor, 0L, SEEK_END); //go to end of file
	lseek(fileDescriptor,0L,SEEK_SET); //cursor back to beginning of file

	char *fileContents = malloc(fileLengthInBytes+1); //memory leak call the cops
	read(fileDescriptor, fileContents, fileLengthInBytes);
	close(fileDescriptor);



	//printf("%s", fileContents);
	return fileContents; 
	
	
}

char** _convertLineStringIntoLineArray(char* reallyLongString, int* lineCountArg) { //splits a long string by \n
	
	
	const int maxStringSize=100;
	
	char** result=malloc( MAX_CRON_TASKS*sizeof( char* ));
	
	for (int i = 0; i < MAX_CRON_TASKS; i++) {
		result[i]=malloc(maxStringSize);
	}
	
	//basically, char** result now is char result[MAX_CRON_TASKS][maxStringSize];
	
	for(int i = 0; i < MAX_CRON_TASKS; i++) {
		strcpy(result[i], "");
	}


	const int stringLength = strlen(reallyLongString);

	int lineCount = 0;
	int characterCount = 0;

	//printf("\nconv strlen %d\n", stringLength);
	
	for(int i = 0; i < stringLength; i++) {
	//	printf("conv %d\n", i);
		char currentCharacter = reallyLongString[i];
		if(currentCharacter=='\n') {
	//		printf("	found a newline lc: %d char: %c \n", lineCount,currentCharacter);
			result[lineCount][characterCount]='\0';
			lineCount++;
			characterCount=0;
		}
		else {
			result[lineCount][characterCount] = currentCharacter;
	//		printf("	lc: %d char: %c \n", lineCount,currentCharacter);
			characterCount++;
		}
	}
	
	result[lineCount][characterCount-1]='\0';
	
	
	*lineCountArg=lineCount;
	return result;
}

int _compareForQsort(const void * a, const void *  b) {
	struct TASKFILE_LINE *elemA = (struct TASKFILE_LINE*) a;
	struct TASKFILE_LINE *elemB = (struct TASKFILE_LINE*) b;
	int ahour=elemA->hour;
	int bhour=elemB->hour;
	int aminute=elemA->minute;
	int bminute=elemB->minute;
	
	if(ahour>bhour) return 1;
	else if(ahour<bhour) return -1;
	
	else
		if(aminute>bminute) return 1;
		else if(aminute<bminute) return -1;
		else return 0;
}

