#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void) {

    pid_t child_pid;
    int exit_status;


    // The fork() function creates a duplicate of
    // the currently running Process.
    // fork() is called here.
    switch (child_pid = fork()) {

        case -1:
        	/* Something went wrong */
            perror("fork");
            exit(1); /* Parent exits */

        case 0:
        	// New “Child” Process inherits all the same
        	// resources as the “Parent” Process.
            printf(" CHILD: This is the child process!\n");
            printf(" CHILD: My PID is \t\t%d\n", getpid());
            printf(" CHILD: My parent's PID is \t%d\n", getppid());
            printf(" CHILD: Enter `0` to exit: \n");

            // entering any int value will exit the child process
            scanf(" %d", &exit_status);

            printf(" CHILD: I'm outta here!\n");
            exit(exit_status);

        default:
            // The “Parent” Process will receive the PID of the newly
            // created Process as the return value from fork().
            printf("PARENT: This is the parent process!\n");
            printf("PARENT: My PID is \t\t%d\n", getpid());
            printf("PARENT: My child's PID is \t%d\n", child_pid);
            printf("PARENT: I'm now waiting for my child to exit()...\n");

            wait(&exit_status);

            printf("PARENT: My child's exit status is: %d\n", WEXITSTATUS(exit_status));
            printf("PARENT: I'm outta here!\n");
    }
    return 0;
}
