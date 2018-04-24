#include "logger.h"

void enableLogging() {
    openlog("mini-cron", LOG_PID, LOG_DAEMON);
}

void closeLogging() {
    closelog();
}