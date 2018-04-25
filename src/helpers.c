#include "helpers.h"
#include "taskfileparse.h"

int currentHour() {//return current hour
	time_t now = time(NULL);
	struct tm *tm_struct = localtime(&now);
	return tm_struct->tm_hour;
}

int currentMinute() {//return current minute
	time_t now = time(NULL);
	struct tm *tm_struct = localtime(&now);
	return tm_struct->tm_min;
}

int currentSecond() { //return current second
	time_t now = time(NULL);
	struct tm *tm_struct = localtime(&now);
	return tm_struct->tm_sec;
}

int str2int(char* str) { //turn a "str"ing representing a number (eg "24") and turn it into an int ("24"->24).
	int stringLength = strlen(str);
	int multiply = pow(10.0,(float) stringLength-1);
	int result=0, i;
	
	for(i = 0; i < stringLength; i++) {
		int translatedNumberFromASCII = str[i]-48;
		int charIsDigit = translatedNumberFromASCII >= 0 && translatedNumberFromASCII <=9;
		
		assert(charIsDigit); //something something put errors here later
		
		//printf("Adding %d\n", translatedNumberFromASCII*multiply);
		result+=translatedNumberFromASCII*multiply;
		multiply /= 10;
	}

	return result;
}

char* strcut(char* start, char* end) { //return characters between pointers "start" and "end" as string
	const int stringLength = end-start;
	char* tab = malloc(sizeof(char)*100); //memory leak call the cops

	int i;
	for(i = 0; i < stringLength; i++) {
		tab[i]=*(start+i);
	}
	tab[stringLength]='\0'; //add string terminator to the end of our newly created string

	return tab;
}

struct TASKFILE_LINE parseTaskfileLine(char* line) {//takes a "line" (a string) from taskfile and prases it into a TASKFILE_LINE struct
	
	const int stringLength = strlen(line);
	int colonPositionsInString[] = {-1, -1, -1};
	int colonCounter=0, i;

	for(int i = 0; i < stringLength; i++) { //save the respective colon (":") positions from the string in an array
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
	result.hour=str2int(strcut(hourStart, hourEnd)); //save the values we've just determined to the object we're going to return
	result.minute=str2int(strcut(minuteStart, minuteEnd));
	result.command=strcut(commandStart,commandEnd);
	result.info=str2int(strcut(infoStart,infoEnd));

	return result;
		
}

int stringContainsCharacter(char* string, char character) { //checks if argument "string" contains given "character"
	int stringLength = strlen(string);

	for(int i = 0; i < stringLength; i++)
		if(string[i]==character) 
			return 1;
	
	return 0;
}

void getTaskCurrentHourIndexRange(int* start, int* end, struct TASKFILE_LINE* tasks, int numberOfTasks, int* daemonSleepFor) {
	const struct TASKFILE_LINE* currentTime = createPlaceholderTask(currentHour(), currentMinute(), "", 0);
	//_compareForQsort compares only hours for tasks, so I create a dummy task with the current time above
	
	static int rememberLastEnd = 0; //this will remember where to star the search from between function calls (the tasks[] array is sorted)
	*start = rememberLastEnd;
	printf("	all good friend, start:%d\n",*start);
	
	fflush(stdout);
	printf("check first comparison: %d\n", _compareForQsort(currentTime, &tasks[*start]));
	printf("check while condition: %d\n", _compareForQsort(currentTime, &tasks[*start]) < 0);
	printTask(*currentTime);
	printTask(tasks[*start]);
	while(_compareForQsort(currentTime, &tasks[*start]) != 0 ) { //seek until you find a task whose hour&minute is "now" or later than currenttime
		printf("	start:%d\n",*start);
	       if(*start == numberOfTasks) {
		printf("All scheduled tasks already completed OR cron started after they were scheduled. Nothing to do. Closing.\n");
		       return;
	       }
		(*start)++;
	}
		*end=*start;

	while(_compareForQsort(currentTime, &tasks[*end])==0) { //find the last place where there's a task for the current time
		printf("	adding end: %d\n", *end);
		if(*end==numberOfTasks) break;
		(*end)++;
	}

	printf("Finally, end is: %d\n", *end);

	printf("Sleep formula debug:\n Supposedly the next task is:\n");
	printTask(tasks[*end]);
	printf("The current time is: %d:%d:%d\n", currentHour(), currentMinute(), currentSecond());	
	if(*end<numberOfTasks) { //check the next task and tell the daemon when to wake up next  
 		*daemonSleepFor = ((tasks[*end].hour-currentTime->hour)*60*60+(tasks[*end].minute-currentTime->minute)*60-currentSecond()+1); //this is in seconds		
	}

	rememberLastEnd = *end;
	free(currentTime);
}

char** splitByCharacter(char* string, int* numberOfCommandsArg,char splittingCharacter) {
	
	//memory alloc stuff
	int numberOfPipes = 0;
	
	for(int i = 0; i < strlen(string);i++)
		if(string[i]==splittingCharacter)
			numberOfPipes++;
	
	char** result;
	
	if ((result = malloc(sizeof(*result)*(numberOfPipes+1)))==NULL) {	
			fflush(stdout);
			printf("\nmemory alloc failed on result\n");
			return result;
	}

	for(int i = 0; i < numberOfPipes+1; i++) {
		if((result[i]=malloc(sizeof (*result[i])*100))==NULL) {
			fflush(stdout);
			printf("\nmemory alloc failed on result[i]\n");
			return result;
		}
	}
	
	//end of memory alloc stuff
	int lineCount = 0;
	int characterCount = 0;
	int stringLength=strlen(string);

	//printf("\nconv strlen %d\n", stringLength);
	
	for(int i = 0; i < stringLength; i++) {
		char currentCharacter = string[i];
		if(currentCharacter==splittingCharacter) {
			//printf("	conv %d FOUND SPLITCHAR lc: %d char: %c------- \n", i, lineCount,currentCharacter);
			result[lineCount][characterCount]='\0';
			lineCount++;
			characterCount=0;
		}
		else {
			result[lineCount][characterCount] = currentCharacter;
			//printf("	conv %d lc: %d char: %c \n", i, lineCount,currentCharacter);
			//printf("	but in memory char: %c\n", result[lineCount][characterCount]);
			characterCount++;
		}
	}
	
	result[lineCount][characterCount]='\0';
/*
	for(int i = 0; i<=lineCount; i++) {
		printf("%d -> %s\n",i, result[i]);
	}
*/	
	*numberOfCommandsArg=numberOfPipes+1;
	return result;	
}


void printTask(struct TASKFILE_LINE task) { //takes a "task" and prints all it's fields (hour, minute, command, info)
	printf("%d %d %s %d\n", task.hour, task.minute, task.command, task.info);
}

struct TASKFILE_LINE* createPlaceholderTask(int hour, int minute, char* command, int info) { //create a task manually with set commands
	struct TASKFILE_LINE* task = malloc(sizeof(*task));
	
	task->hour=hour;
	task->minute=minute;
	task->command=command;
	task->info=info;

	return task;
}

char* removeEdgeSpaces(char* string) {	//removes chains of spaces from the left and the right ("   string   " -> "string")
	char* start = string;
	char* end = start+strlen(string)-1;// printf("END: %c\n", *end);

	while(*(start++)==' '); //you think Linus would hire me?	
	while(*(end--)==' ');
	//*(end+2) = '\0';

	return strcut(--start, end+2); //this func will have weird behavior if the given string is all spaces
}


int checkArgs(int argc, char* argv[]) { //checks if the user provided a correct amount of arguments
	if(argc > 3) { //argument count needs to be >3 because the first argument is the program name ("./minicron");
		errno=E2BIG;
		perror("Too many arguments (2 needed)\n");
		return errno;
	}

	else if (argc<3) {
		errno=0; //no error code for too few arguments btw
		fprintf(stderr, "Too few arguments (2 needed)\n");
		return errno;
	}

	return 0; //return 0 if everything is ok
}
