#include "helpers/helpers.h"
#include <stdio.h>

int main() {
	
	//parser
	char tab[]="25:56:command:0";
	struct TASKFILE_LINE line = parseTaskfileLine(tab);
	printf("%d %d %s %d", line.hour, line.minute, line.command, line.info);

	//str2int
	//printf("%d",str2int("3000"));

	//strcut
	//char tab[] = "XD";
	//char* result = strcut(tab, tab+3);
	//printf("%s", result);

	return 0;
}
