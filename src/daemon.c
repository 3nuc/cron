#include "logger.h"
#include "daemon.h"

void forkDaemon() {
	pid_t pid, sid;
	pid = fork();

	if( pid < 0 ) { 
		printf("Error forking a daemon process");
		exit(EXIT_FAILURE);
	}
	if( pid > 0 ) {
		exit(EXIT_SUCCESS);
	}

	umask(0);

	enableLogging();

	sid = setsid();
	if( sid < 0 ) {
		syslog(LOG_ERR, "Session set failed");
		exit(EXIT_FAILURE);
	}

	if( (chdir("/")) < 0 ) {
		syslog(LOG_ERR, "Changing working directory failed");
		exit(EXIT_FAILURE);
	}

	//close(STDIN_FILENO);
	//close(STDOUT_FILENO);
	//close(STDERR_FILENO);

	syslog(LOG_NOTICE, "Mini-cron properly forked");
}

void executeCommand(char* command) {
	pid_t pid;
	pid = fork();

	if( pid < 0 ) {
		syslog(LOG_ERR, "Failed creating a process for the command %s", command);
		exit(EXIT_FAILURE);
	}
	if( pid > 0 ) {
		exit(EXIT_SUCCESS);
	}

	char* test[2];
	test[0] = "/bin/mkdir";
	test[1] = "/home/maxim/pls";
	test[2] = 0;

	syslog(LOG_NOTICE, "Forked command %s", command);
	execv("/bin/mkdir", test);
}
