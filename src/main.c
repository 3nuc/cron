#include "helpers/helpers.h"
#include <stdio.h>

int main() {
	
	//parser
	char tab[]="25:56:command:0";
	struct TASKFILE_LINE line = parseTaskfileLine(tab);
	printf("%d %d %s %d \n", line.hour, line.minute, line.command, line.info);


	//strcut
	//char tab[] = "XD";
	//char* result = strcut(tab, tab+3);
	//printf("%s", result);

	return 0;
}
