#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>

// Global variable
volatile sig_atomic_t calledHandler = 0;

// Function Declarations
void* child_thread(void *args);// Function for child threads
void handler(int sig);// Signal handler for SIGUSR1

// The Semaphore
sem_t *semaphore;

//===================================
//=               MAIN              =
//===================================
int main() {

    int nThreads = 0;
    pthread_attr_t attr;

    // Setting up signal handler for SIGUSR1
    struct sigaction sa;
    sa.sa_handler = handler;
    // Special flag to affect signal behavior
    sa.sa_flags = 0;
    // Remove all signals from sa.sa_mask.
    sigemptyset(&sa.sa_mask);
    // Add the signal in SIGUSR1 to the mask.
    sigaddset(&sa.sa_mask, SIGUSR1);

    //checks if the setup of the signal handler for SIGUSR1 was successful.
    //If it fails, an error message is printed, and the program exits with a failure status.
    if (sigaction(SIGUSR1, &sa, NULL) != 0) {
        perror("ERROR, handler function could not intercept SIGUSR1");
        exit(1);
    }

    // Prompt user for number of threads
    printf("Enter the number of children: \n");
    scanf("%d", &nThreads);
    pthread_t threadArray[nThreads];


    // Creating the named semaphore
    if ((semaphore = sem_open("namedSemaphore", O_CREAT, S_IWOTH, 0)) == SEM_FAILED) {
        perror("sem_init");
        exit(1);
    }

        pthread_attr_init(&attr);

        // Create threads in this for loop
		for (int i = 0; i < nThreads; i++) {
			// Create a new child thread
			pthread_create(&threadArray[i], &attr, &child_thread, NULL);
		}

	    pthread_attr_destroy(&attr);

    // Wait for handler to be called
    while (!calledHandler) {
    	sleep(3);
    }

    printf("Exiting Thread factory...");
    sleep(2);

    // Clean up semaphore
    sem_close(semaphore);//close a named semaphore
    sem_unlink("namedSemaphore");//destroy the named semaphore

    return EXIT_SUCCESS;
}

//================================================
//=                 FUNCTIONS                    =
//================================================

// Thread handler (semaphore) - displays output to console
void* child_thread(void *args) {
    printf("Child thread %d created.\n", pthread_self());
    do {
    	// Wait on the semaphore
        sem_wait(semaphore);
        // Print message when unblocked
        printf("Child Thread %d unblocked.\n", pthread_self());
        // 5 second sleep
        sleep(5);
    } while (!calledHandler);
    return NULL;
}

// Handler to set 'calledHandler' to 1 when SIGUSR1 is received
void handler(int sig) {
    calledHandler = 1;
}
