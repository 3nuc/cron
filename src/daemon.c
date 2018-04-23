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

void handleCommand(char* commandString, const char* outPath, int outtype) {

	//if(stringContainsCharacter(commandString,'|')) { //if command is like 'program1 | program2'
	int* numberOfPipedCommands=malloc(sizeof *numberOfPipedCommands);
	char** commands = splitByCharacter(commandString, numberOfPipedCommands, '|');

	int fds[(*numberOfPipedCommands) * 2];	// pipes
	for(int i = 0; i < *numberOfPipedCommands; i++) {
		if(pipe(fds + i*2) < 0) {
			syslog(LOG_ERR, "Failed creating a pipe");
			exit(EXIT_FAILURE);
		}
	}

	// error pipe
	int errp[2];
	if(pipe(errp) < 0) {
		syslog(LOG_ERR, "Failed creating an error pipe");
		exit(EXIT_FAILURE);
	}

	char buff[4096];
	char bufferr[1024];// bufferr[0] = '\0';
	int nbyteserr = 0;
	
	int* commandArgc;
	char** commandArgv;

	syslog(LOG_NOTICE, "Forking command %s", commandString);

	for(int j = 0; j<*numberOfPipedCommands; j++) {

		commandArgc=malloc(sizeof *commandArgc);
		commandArgv = splitByCharacter(removeEdgeSpaces(commands[j]), commandArgc, ' ');
		
		for(int k = 0; k < *commandArgc; k++) {
			printf("	|%s|\n", commandArgv[k]);
			//the above printf prints arguments (the same format as argv[]) of a command.
			//so "cd -d -xd" will print "cd", "-d" "-xd".
		}

		pid_t pid = fork();

		if( pid < 0 ) {
			syslog(LOG_ERR, "Failed creating a process for the command %s", commandArgv[0]);
			exit(EXIT_FAILURE);
		}
		if( pid > 0 ) {	// parent
			//waitpid(pid,0,0);
		}
		else {	//child

			if(j != 0) {
				dup2(fds[(j-1)*2], STDIN_FILENO);
			}
			
			dup2(fds[j*2+1], STDOUT_FILENO);
			dup2(errp[1], STDERR_FILENO); close(errp[0]); close(errp[1]);

			commandArgv[*commandArgc] = 0; // null termination is required at the end of argv[] in exec

			for(int i = 0; i < (*numberOfPipedCommands)*2; i++) {
				close(fds[i]);
			}

			execvp(commandArgv[0], commandArgv);

			//exec error
			syslog(LOG_ERR, "EXEC error of command %s", commandArgv[0]);
			exit(EXIT_FAILURE);
		}
	}

	close(errp[1]);
	nbyteserr = read(errp[0], bufferr, sizeof(bufferr));
	close(errp[0]);

	for(int i = 0; i < (*numberOfPipedCommands)*2; i++) {
		if(((*numberOfPipedCommands)-1)*2 != i)
			close(fds[i]);
	}

	int out = open(outPath, O_WRONLY | O_APPEND);

	if(nbyteserr != 0)
	{
		bufferr[nbyteserr] = '\0';
		syslog(LOG_ERR, "ERROR: %s", bufferr);
		if(outtype != 0) { // 0 - tylko stdout
			write(out, bufferr, nbyteserr);
		}
	}
	else {
		if(outtype != 1) { // 1 - tylko stderr
			int nbytes = read(fds[((*numberOfPipedCommands)-1)*2], buff, sizeof(buff));
			write(out, buff, nbytes);
			syslog(LOG_NOTICE, "Fork successful: %s", commandString);
		}
	}

	close(out);
	close(fds[((*numberOfPipedCommands)-1)*2]);

	free(commandArgc);
	free(commandArgv);
	free(commands);
	free(numberOfPipedCommands);
	// }
	
	// else {  //for normal commands (without pipe)
	// 	printf("%s\n", commandString);
	// 	/*createWholesomeFork(commandString)*/
	// };
}