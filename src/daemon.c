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

void executeCommand(char** command, int outtype) {
	pid_t pid;
	int fd[2];
	char buff[4096];

	if(pipe(fd) < 0) {
		syslog(LOG_ERR, "Failed creating a pipe for %s", command);
		exit(EXIT_FAILURE);
	}

	pid = fork();

	if( pid < 0 ) {
		syslog(LOG_ERR, "Failed creating a process for the command %s", command);
		exit(EXIT_FAILURE);
	}
	if( pid > 0 ) {	// parent

		close(fd[1]);

		int out = open("/home/maxim/studia/out", O_WRONLY | O_APPEND);
		int nbytes = read(fd[0], buff, sizeof(buff));
		write(out, buff, nbytes);

		close(out);
		close(fd[0]);
		waitpid(pid, 0, 0);
	}
	else {	//child

		close(fd[0]);

		dup2(fd[1], STDOUT_FILENO);
		syslog(LOG_NOTICE, "Forked command %s", command[0]);

		char* test[2];
		test[0] = "cat";
		test[1] = "/home/maxim/studia/cron/README.md";
		test[2] = 0;
		
		execv(command[0], command);

		close(fd[1]);
	}
}
