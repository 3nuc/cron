#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "taskfileparse.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

void forkDaemon();
void handleCommand(char* commandString, const char* outPath, int outtype);