#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include "helpers.h"

struct TASKFILE_LINE *getTaskArray(char* pathToTaskfile, int* lineCountArg);
char* _getTaskfileContentsAsLineString(char* pathToTaskfile);
char** _convertLineStringIntoLineArray(char* reallyLongString, int* lineCountArg );
