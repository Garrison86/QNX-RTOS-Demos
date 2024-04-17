#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>


int main() {
    int numWakeup = 0;
    sem_t *sem;


    // Initialize semaphore
    if ((sem = sem_open("namedSemaphore", O_CREAT, S_IWOTH, 1))
           == SEM_FAILED) {
       printf("Error in sem_open method");
       exit(1);
   }

    printf("Thread-waker PID = %d\n", getpid());

    do {
        // Prompt user for the number of threads to wake up
        printf("Enter the number of threads to wake up (enter 0 to exit): \n");
        scanf("%d", &numWakeup);

        // Increment semaphore by the specified number
        for (int i = 0; i < numWakeup; i++) {
            sem_post(sem);
        }

    } while (numWakeup > 0);

    // Close the semaphore
    sem_close(sem);
    return EXIT_SUCCESS;
}
