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
    sem_t *semaphore;


    // The named semaphore from `thread-factory`
    semaphore = sem_open("namedSemaphore", O_CREAT, S_IWOTH, 1);
    if (semaphore == SEM_FAILED) {
       printf("Error in sem_open method");
       exit(1);
   }

    printf("Thread-waker PID = %d\n", getpid());


    //========================================
    //= PROCESS UNTIL ALL THREADS ARE AWAKE  =
    //========================================
    do {
        printf("Enter the number of threads to wake up (enter 0 to exit): \n");
        scanf("%d", &numWakeup);

        for (int i = 0; i < numWakeup; i++) {
            sem_post(semaphore); // increment a named or unnamed semaphore
        }

    } while (numWakeup > 0);



    sem_close(semaphore);
    return EXIT_SUCCESS;
}
