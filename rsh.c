#include <stdio.h>
#include <stdlib.h>
#include <spawn.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define N 12

extern char **environ;

char *allowed[N] = {"cp","touch","mkdir","ls","pwd","cat","grep","chmod","diff","cd","exit","help"};

int isAllowed(const char*cmd) {
	// TODO
	// return 1 if cmd is one of the allowed commands
	// return 0 otherwise
	for(int i = 0; i < N; i++){
		if(strcmp(cmd, allowed[i]) == 0){
			return 1;
		}
	}
	return 0;
}

int main() {

    // TODO
    // Add variables as needed

    char line[256];

    while (1) {

	fprintf(stderr,"rsh>");

	if (fgets(line,256,stdin)==NULL) continue;

	if (strcmp(line,"\n")==0) continue;

	line[strlen(line)-1]='\0';

	// TODO
	// Add code to spawn processes for the first 9 commands
	// And add code to execute cd, exit, help commands
	// Use the example provided in myspawn.c

	pid_t pid;

	
	char* cmd = strtok(line, " ");
	char *argv[20] = {};
	argv[0] = cmd;
	int count = 1;
	if(isAllowed(cmd) == 0){
		printf("NOT ALLOWED!\n");
		continue;
	}
	if(strcmp(cmd, "cd") == 0){
		argv[1] = strtok(NULL, " ");
		if(strtok(NULL, " ") != NULL){
			printf("-rsh: cd: too many arguments");			
		}else{
			chdir(argv[1]);
		}

	} else if(strcmp(cmd, "help") == 0){
		printf("The allowed commands are:\n");
		printf("1: cp\n2: touch\n3: mkdir\n4: ls\n5: pwd\n6: cat\n7: grep\n8: chmod\n9: diff\n10: cd\n11: exit\n12: help\n");
	} else if(strcmp(cmd, "exit") == 0){
		exit(0);
	} else {
			char* token = strtok(NULL, " ");
			argv[1] = token;
			count++;
		while(token != NULL){	
			token = strtok(NULL, " ");
			argv[count] = token;
			count++;
		
		}

		int status;
		posix_spawnattr_t attr;

		// Initialize spawn attributes
		posix_spawnattr_init(&attr);

		// Set flags if needed, for example, to specify the scheduling policy
		// posix_spawnattr_setflags(&attr, POSIX_SPAWN_SETSCHEDULER);

		// Spawn a new process
		if (posix_spawnp(&pid, argv[0], NULL, &attr, argv, environ) != 0) {
			perror("spawn failed");
			exit(EXIT_FAILURE);
		}

		// Wait for the spawned process to terminate
		if (waitpid(pid, &status, 0) == -1) {
			perror("waitpid failed");
			exit(EXIT_FAILURE);
		}

		// Destroy spawn attributes
		posix_spawnattr_destroy(&attr);

		}

    }
    return 0;
}
