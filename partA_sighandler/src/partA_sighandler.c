#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <stdbool.h>

///////// GLOBAL VARIABLES ///////////
volatile bool usr1Happened = false;
void sigint_handler();


 ////////////// main( ) //////////////
int main(void) {

/* //////////////////////////////// ANALOGY //////////////////////////////////////
 * // Imagine your computer's operating system (OS) as a bustling city.         //
 * // In this city, various events are constantly happening                     //
 * // (e.g., construction work, parades, emergencies). Signals in an OS are     //
 * // like notifications about these events. Just as city events might require  //
 * // certain responses (like rerouting traffic or sending emergency services), //
 * // signals in an OS notify a program that an event has occurred, prompting   //
 * // a specific response.                                                      //
 * ///////////////////////////////////////////////////////////////////////////////
 */
	char string[140];

	///////// SIG SETUP ///////////
    /*
	To receive a Signal from within your application you need to
	follow a few steps.

	- 1. Create a sigaction struct
	- 	Populate it with the desired signal handler
	- 	Setup any additional signal handling options.

	- 2. Setup the Signal Mask
	-  	This is a list of Signals to block while within the signal handler.
	*/

	// The sigaction structure is like a detailed
	// instruction manual for how to handle specific
	// events (signals) in our metaphorical city. It contains:
	// Signal Handler Functions (`sa_handler`, `sa_sigaction`).
	struct sigaction sa;


	// If the event doesn't require a queue (immediate action is possible), sa_handler is used
	// It's like having a quick response team for immediate issues
	sa.sa_handler = sigint_handler;


	// sa_flags – Special flags to affect signal behaviour
	// These flags adjust how to respond to events.
	sa.sa_flags = 0;

	// sa_mask – A set of signals to mask (block) during a signal handler.
	// This is a helper function to do the bitwise work for us
	sigemptyset(&sa.sa_mask); // – Remove all signals from &sa.sa_mask (Clears all roadblocks, starting fresh.)


	// in this case `SIGUSR1` is the signal you will be attaching a handler.
	int sigactfunc = sigaction(SIGUSR1, &sa, NULL); // returns an int

	if ( sigactfunc == -1) {
		perror("sigaction");
		exit(1);
	}
	////////////////////////////////


	///////// ENDLESS PROGRAM FOR USER INPUT ////////////
	while(!usr1Happened){
		printf("PID = %d:\t\t Running...\n", getpid());
		printf("Enter a string:  ");
		if(fgets(string, sizeof string, stdin) == NULL){
			printf("\n");
		} else {
			printf("You entered:\t%s\n", string);
		}
	}
	/////////////////////////////////////////////////////


	///////////////// EXITING ////////////////////
	printf("PID = %d:\t\t Recived SIGUSR1\n", getpid());
	printf("PID = %d:\t\t Exiting...\n", getpid());

	kill( getpid(), SIGUSR1);
	return EXIT_SUCCESS;
}


////////// SIGINT HANDLER ////////////////
void sigint_handler() {
	usr1Happened = true;
}
