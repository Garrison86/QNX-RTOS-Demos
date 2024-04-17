#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>

/* This Program uses a named semaphores to handle
 * multiple threads accessing resources.
 *
 * - Semaphores are asynchronous safe. This means you can access them in a signal handler.
 * - For a named semaphores:
 * 				sem_open() creates a named semaphore
 * 				sem_unlink() destroys the named semaphore
 *
 * Sigaction is used to handle the signals for this program.
 * - Sigaction struct: is a data structure that defines signal handling behavior
 * - Sigaction function: is used to register this behavior with the operating system.
 *
 *
 * */

volatile sig_atomic_t calledHandler = 0;
void* child_thread(void *args);
void handler(int sig);
sem_t *semaphore;

//===================================
//=               MAIN              =
//===================================
int main() {
    int total_iterations = 10;					// Loading Bar, not required
    int current_iteration = 0;					// Loading Bar, not required


    int numberOfThreads = 0;
    pthread_attr_t threadAttributes;


	//===================================
	//=        SIGNAL HANDLING          =
	//===================================
    struct sigaction sa;
    sa.sa_handler = handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask); // Remove all signals from `sa.sa_mask`
    sigaddset(&sa.sa_mask, SIGUSR1);// Add the signal in `SIGUSR1` to the mask.


/* Sigaction Function: specifies how a process handles signals, allowing the setup of a
 * signal handler function, blocking signals during handler execution, and defining behavior for
 * specific signals
 */if (sigaction(SIGUSR1, &sa, NULL) != 0) {
        perror("ERROR, handler function could not intercept SIGUSR1");
        exit(1);
    }


    printf("Enter the number of children: \n");
    scanf("%d", &numberOfThreads);

    //===================================
    //=       ARRAY OF THREADS          =
    //===================================
    pthread_t threadArray[numberOfThreads];


    //===================================
    //=   CREATE THE NAMED SEMAPHORE    =
    //===================================
    semaphore = sem_open("namedSemaphore", O_CREAT, S_IWOTH, 0);

	 if (semaphore == SEM_FAILED) {
		perror("sem_init");
		exit(1);
	 }


	//start by initializing the default Thread options into the `threadAttributes` object
	pthread_attr_init(&threadAttributes);
    //===================================
    //=      CREATING THE THREADS       =
    //===================================
	for (int i = 0; i < numberOfThreads; i++) {
		pthread_create(&threadArray[i], &threadAttributes, &child_thread, NULL);
	}

	/* After you have started your thread there is still one more thing you
	 * should always do. You should clean up the data structures you used
	 * in the thread setup process.
	 */
	pthread_attr_destroy(&threadAttributes);


    while (!calledHandler) {
        printf("[");									// Loading Bar, not required
        for (int i = 0; i < current_iteration; i++) {	// Loading Bar, not required
            printf("=");								// Loading Bar, not required
        }printf(">\r");									// Loading Bar, not required
        fflush(stdout);									// Loading Bar, not required
    	sleep(3);
        current_iteration++;							// Loading Bar, not required
        if (current_iteration >= total_iterations) {	// Loading Bar, not required
            current_iteration = 0;						// Loading Bar, not required
        }												// Loading Bar, not required
    }

    printf("Exiting Thread factory...");
    sleep(2);

    sem_close(semaphore);//close the named semaphore
    sem_unlink("namedSemaphore");//destroy the named semaphore
    return EXIT_SUCCESS;
}

//================================================
//=                 FUNCTIONS                    =
//================================================

void* child_thread(void *args) {
    printf("Child thread %d created.\n", pthread_self());
    do {
        sem_wait(semaphore);
        printf("Child Thread %d unblocked.\n", pthread_self());
        sleep(5);
    } while (!calledHandler);
    return NULL;
}

void handler(int sig) {
    calledHandler = 1;
}
