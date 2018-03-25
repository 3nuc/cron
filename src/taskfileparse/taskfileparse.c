#define MAX_CRON_TASKS 100 //how many lines there can be in the taskfile

struct TASKFILE_LINE *getTaskArray(char** linesFromTaskfile) {
	struct TASKFILE_LINE result[MAX_CRON_TASKS];
	for(int i = 0; i < MAX_CRON_TASKS; i++) {
		result[i]=NULL;
	}
	
	int numberOfLines = sizeof(linesFromTaskfile)/sizeof(char*MAX_CRON_TASKS);

	bool parsingLessLinesThanMAXCRONTASKSAllows=numberOfLines<MAX_CRON_TASKS;
	assert(parsingLessLinesThanMAXCRONTASKSAllows);
	
	for(int i = 0; i < numberOfLines) {
		result[i]=parseTaskfileLine(linesFromTaskfile[i]);
	}
	return result;
}


char** _getTaskfileContents(char* pathToTaskfile) {
	int fileDescriptor = open(pathToTaskfile, O_RDONLY);

	//add error handling later (for opening the file)

	fseek(fileDescriptor, 0L, SEEK_END); //go to end of file
	int fileLengthInBytes=ftell(fileDescriptor);
	
	rewind(fileDescriptor); //cursor back to beginning of file

	char *fileContents = malloc(fileLengthInBytes+1); //memory leak call the cops
	fread(fileContents, fileLengthInBytes, 1, f); //no idea what the 1,f is
	fclose(fileDescriptor);

	fileContents[fileLengthInBytes]=0;

	return fileContents; 
	
	
}

char** _convertLineStringIntoLineArray(char* reallyLongString) { //splits a long string by \n
	char* result[MAX_CRON_TASKS];

	for(int i = 0; i < MAX_CRON_TASKS; i++) {
		result[i]=NULL;
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
