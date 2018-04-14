#include<syslog.h>

void enableLogging();
void closeLogging();
void logCritical(char* msg);
void logNoDate(char* msg);