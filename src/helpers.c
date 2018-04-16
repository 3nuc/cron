#include "helpers.h"

int currentHour() {
	time_t now = time(NULL);
	struct tm *tm_struct = localtime(&now);
	return tm_struct->tm_hour;
}

int currentMinute() {
	time_t now = time(NULL);
	struct tm *tm_struct = localtime(&now);
	return tm_struct->tm_min;
}

int currentSecond() { //probably wont be needed but just in case
	time_t now = time(NULL);
	struct tm *tm_struct = localtime(&now);
	return tm_struct->tm_sec;
}

int str2int(char* str) { //probably works
	int stringLength = strlen(str);
	int multiply = pow(10.0,(float) stringLength-1);
	int result=0, i;
	
	for(i = 0; i < stringLength; i++) {
		int translatedNumberFromASCII = str[i]-48;
		//int charIsDigit = translatedNumberFromASCII >= 0 && translatedNumberFromASCII <=9;
		
		//assert(charIsDigit); //something something put errors here later
		
		//printf("Adding %d\n", translatedNumberFromASCII*multiply);
		result+=translatedNumberFromASCII*multiply;
		multiply /= 10;
	}

	return result;
}

char* strcut(char* start, char* end) {
	const int stringLength = end-start;
	char* tab = malloc(sizeof(char)*100); //memory leak call the cops

	int i;
	for(i = 0; i < stringLength; i++) {
		tab[i]=*(start+i);
	}
	tab[stringLength+1]='\0';

	//printf("%s",tab);

	//printf("strcut: %s\n", tab);
	return tab;
}

struct TASKFILE_LINE parseTaskfileLine(char* line) {
	
	const int stringLength = strlen(line);
	int colonPositionsInString[] = {-1, -1, -1};
	int colonCounter=0, i;

	for(i = 0; i < stringLength; i++) {
		if(line[i] == ':') {
			colonPositionsInString[colonCounter] = i;
			
			colonCounter++;
		}

		assert(colonCounter<=3); //cant have more than 3 colons TODO: error handling, later (actually never lmao)
		
	}

	//printf("%d %d %d", colonPositionsInString[0], colonPositionsInString[1],colonPositionsInString[2]);
	char* hourStart = line;
	char* hourEnd= line+colonPositionsInString[0];

	char* minuteStart = hourEnd+1; //+1 because we're skipping the colon
	char* minuteEnd= line+colonPositionsInString[1];

	char* commandStart = minuteEnd+1;
	char* commandEnd= line+colonPositionsInString[2];

	char* infoStart = commandEnd+1; //info should only be a single digit, but later cron commands could handle errors if you eg. pass two digits
	char* infoEnd= line+stringLength;



	struct TASKFILE_LINE result;
	result.hour=str2int(strcut(hourStart, hourEnd));
	result.minute=str2int(strcut(minuteStart, minuteEnd));
	result.command=strcut(commandStart,commandEnd);
	result.info=str2int(strcut(infoStart,infoEnd));

	return result;
		
}

int stringContainsCharacter(char* string, char character) {
	int stringLength = sizeof(string)+1;

	for(int i = 0; i < stringLength; i++)
		if(string[i]==character) 
			return 1;
	
	return 0;
}

char** splitByPipe(char* string, int* numberOfCommandsArg) {
	
	//memory alloc stuff
	int numberOfPipes = 0;
	
	for(int i = 0; i < strlen(string);i++)
		if(string[i]=='|')
			numberOfPipes++;

	char** result = malloc(sizeof(char*)*(numberOfPipes+1));

	for(int i = 0; i < numberOfPipes+1; i++) {
		result[i]=malloc(100);
	}
	//end of memory alloc stuff
	
	int lineCount = 0;
	int characterCount = 0;
	int stringLength=strlen(string);

	//printf("\nconv strlen %d\n", stringLength);
	
	for(int i = 0; i < stringLength; i++) {
//	printf("conv %d\n", i);
		char currentCharacter = string[i];
		if(currentCharacter=='|') {
//			printf("	conv %d found a newline lc: %d char: %c \n", i, lineCount,currentCharacter);
			result[lineCount][characterCount]='\0';
			lineCount++;
			characterCount=0;
		}
		else {
			result[lineCount][characterCount] = currentCharacter;
//			printf("	conv %d lc: %d char: %c \n", i, lineCount,currentCharacter);
			characterCount++;
		}
	}
	
	result[lineCount][characterCount-1]='\0';
	
	*numberOfCommandsArg=numberOfPipes+1;
	return result;	
}


void printTask(struct TASKFILE_LINE task) {
	printf("%d %d %s %d\n", task.hour, task.minute, task.command, task.info);
}

int checkArgs(int argc, char* argv[]) {
	if(argc > 3) { 
		errno=E2BIG;
		perror("Too many arguments (2 needed)\n");
		return errno;
	}

	else if (argc<3) {
		errno=0; //no error code for too few arguments btw
		fprintf(stderr, "Too few arguments (2 needed)\n");
		return errno;
	}

	return 0;
}
