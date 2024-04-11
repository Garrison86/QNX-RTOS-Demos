#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>

///////// GLOBAL VARIABLES ///////////
volatile bool usr1Happened = false;
void sigint_handler();


 ////////////// main( ) //////////////
int main(void) {

	/////// LOCAL VARIABLES ///////
	int numbOfChildren = 0;
	printf("Enter the number of childern: ");
    scanf("%d", &numbOfChildren);


    // Parent Process PID
	printf("PID = %d:\t\tParent running...\n", getpid());


    int childCounter = 0;

	for(int i = 0; i < numbOfChildren; ++i){
		usr1Happened = false;
		pid_t pid = fork();

		if(pid == -1){
			perror("fork");
			exit(EXIT_FAILURE);

		} else if(pid == 0){
			printf("CHILD PID = %d:\t\t Running...\n", getpid());

			while (!usr1Happened) {
			}
			printf("PID = %d: Child exiting...\n", getpid());
			++childCounter;
			printf("Child:\t\t%d\n", childCounter);

		}
	}

	childCounter = 0;

    while(childCounter < numbOfChildren){
    	pid_t terminatedChild = wait(NULL);
    	if(terminatedChild != -1){
    		printf("PID = %d:\t\t Recived SIGUSR1\n", getpid());
			printf("PID = %d: \t\tChild %d exiting...\n", getpid(), terminatedChild);
			++childCounter;
    	}
    }
    printf("PID = %d: Children finished, parent exiting.\n", getpid());
    exit(0);
	return 0;
}


////////// SIGINT HANDLER ////////////////
void sigint_handler() {
	usr1Happened = true;
}
