#include "logger.h"

void enableLogging(int* logdesc) {
    *logdesc = open("var/log/minicron", O_CREAT | O_EXCL | WRONLY);
    if( *logdesc < 0 ) {
	if( errno == EEXIST ) {
	    // exists logic
	    return;
	}
	// dunno access denied?
    }
}

void closeLogging(int* logdesc) {
    close(*logdesc);
}

void logCritical(char* msg) {
    // generate timestamp
    printf("%s", msg);
}

void logBlockStart(char* msg {
    // timestamp of block
    printf("%s", msg);
}

void logBlockEnd(char* msg) {
    printf("%s", msg);
    // timestamp of block end
}

void logNoDate(char* msg) {
    printf("%s", msg);
}