#include "taskfileparse.h"
#include <stdio.h>

int main() {
	
	char *tasks = _getTaskfileContents("taskfile.test");
//	printf("%s",tasks);
	char **tasktable=_convertLineStringIntoLineArray(tasks);

	printf("%s",tasktable[0]);
	return 0;
}
