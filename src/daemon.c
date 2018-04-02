#include "daemon.h"

void forkDaemon() {
	pid_t pid, sid;
	pid = fork();

	if( pid < 0 ) {
		exit(EXIT_FAILURE);
	}
	if( pid > 0 ) {
		exit(EXIT_SUCCESS);
	}

	umask(0);

	// enable logging

	sid = setsid();
	if( sid < 0 ) {
		logCritical("Session creation failure\n");
		exit(EXIT_FAILURE);
	}

	if( (chdir("/")) < 0 ) {
		logCritical("Changing working directory failed\n");
		exit(EXIT_FAILURE);
	}

	//close(STDIN_FILENO);
	//close(STDOUT_FILENO);
	//close(STDERR_FILENO);

	logNoDate("Process forked\n");
}

void executeCommand(char** list) {
	pid_t pid;
	pid = fork();

	if( pid < 0 ) {
		logCritical("Failed creating a process for the command\n");
		exit(EXIT_FAILURE);
	}
	// w/e
}
