#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include "helpers.h"

#ifndef TASKFILEPARSE_H
#define TASKFILEPARSE_H

struct TASKFILE_LINE *getTaskArray(char* pathToTaskfile, int* lineCountArg);
char* _getTaskfileContentsAsLineString(char* pathToTaskfile);
char** _convertLineStringIntoLineArray(char* reallyLongString, int* lineCountArg );
int _compareForQsort(const void* a, const void*  b);
#endif
