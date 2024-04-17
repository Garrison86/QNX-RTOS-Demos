/*
 * This program executes the program and arguments
 * specified by argv[1..argc].  The standard input
 * of the executed program is converted to upper
 * case.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <process.h>
#include <sys/wait.h>

int main( int argc, char **argv )
  {
    pid_t pid;
    pid_t wpid;
    int   fd[2];
    char  buffer[80];
    int   i, len;
    int   status;

    if( pipe( fd ) == -1 ) {
       perror( "pipe" );
       return EXIT_FAILURE;
    }

    if( ( pid = fork() ) == -1 ) {
       perror( "fork" );
       return EXIT_FAILURE;
    }

    if( pid == 0 ) {
      /* This is the child process.
       * Move read end of the pipe to stdin ( 0 ),
       * close any extraneous file descriptors,
       * then use exec to 'become' the command.
       */
      dup2( fd[0], 0 );
      close( fd[1] );
      execvp( argv[1], argv+1 );

  /* This can only happen if exec fails; print message
   * and exit.
   */
      perror( argv[1] );
      return EXIT_FAILURE;
    } else {
      /* This is the parent process.
       * Remove extraneous file descriptors,
       * read descriptor 0, write into pipe,
       * close pipe, and wait for child to die.
       */
      close( fd[0] );
      while( ( len = read( 0, buffer, sizeof( buffer ) )
          ) > 0 ) {
        for( i = 0; i < len; i++ ) {
          if( isupper( buffer[i] ) )
            buffer[i] = tolower( buffer[i] );
        }
        write( fd[1], buffer, len );
      }
      close( fd[1] );
      do {
        wpid = waitpid( pid, &status, 0 );
      } while( WIFEXITED( status ) == 0 );
      return WEXITSTATUS( status );
    }
  }
