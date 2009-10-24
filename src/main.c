/*
 * Copyright (C) 2009 Robrecht Dewaele
 *
 * This file is part of Elbow.
 *
 * Elbow is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Elbow is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Elbow.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "bowshell.h"
#include "serial.h"
#include "sendfiles.h"
#include "getopt.h"

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/wait.h>

#define DEFAULT_BUFFER_SIZE 256
#define EXIT_MSG "\nEOF Reached, quitting.."

/* exit codes */
typedef enum {
	exit_success = 0,
	exit_failedpipe,
	exit_failedfork,
	exit_lostconnection
} exit_codes;

/* ---- */
/* MAIN */
/* ---- */
int main(int argc, char * argv[]) {
  /* temp variables for read() and write() return values */
  int cread, cwritten;

  setOptions(argc, argv);

  /* initialize the serial port */
  int serial_fd = serial_init(options.device);

  /* bowshell will be piped here, so select can be used */
  /* the shell has to run in a seperate process because readline is blocking */
  int bowshellpipe[2];
  pid_t cpid; /* current process id will be stored here*/
  char * input = NULL; /* common place to store input */
  char readbuf[DEFAULT_BUFFER_SIZE]; /* static(!) read buffer */

  int piper = pipe(bowshellpipe);
  /* failed opening pipe */
  if (piper == -1) {
    perror("pipe");
    exit(exit_failedpipe);
  }

  cpid = fork();

  /* failed fork */
  if (cpid == -1) {
    perror("fork");
    exit(exit_failedfork);
  }
  /* fork succeeded, start two 'main' loops depending on child or parent */

  if (cpid == 0) {
    /* ----------------------------------------- */
    /* This is the child process, writes to pipe */
    /* ----------------------------------------- */
    close( bowshellpipe[0] ); /* close unused read end */

    /* check whether there are files to be sent first */
    if (options.file)
      sendfiles(1, &options.file, serial_fd);

    /* read forever from stdin with readline */
    for(;;) {
      input = (char *)rl_gets();
      if (!input) { /* if EOF is reached, user wants to stop putting things in */
        puts("EOF from user: quitting");
        close( bowshellpipe[1] );
        return exit_success;
      }
      cwritten = write( bowshellpipe[1], input, strlen(input) );
      if (cwritten < 0)
        perror("child writing to parent error");
    }
  }
  else {
    /* ------------------------------------------- */
    /* This is the parent process, reads from pipe */
    /* ------------------------------------------- */
    close( bowshellpipe[1] ); /* close unused write end */

    /* store the highest filedescriptor + 1, to pass to select() */
    int ndfs = ((serial_fd > bowshellpipe[0])? serial_fd : bowshellpipe[0]) + 1;

    /* define & clear "fd_set"s for select */
    int fds_ready; /* will store amount of filedesriptors that are ready after select() */
    fd_set rd, wr, er; /* read, write, and error file descriptor sets*/
    FD_ZERO(&wr); /* won't be used, so one-time zero */
    FD_ZERO(&er); /* won't be used, so one-time zero */

    /* start receiving from the pipe to the child process and from the serial device */
    for (;;) {
      /* reset "fd_set"s each loop because select() modifies it */
      FD_ZERO( &rd );
      FD_SET( bowshellpipe[0], &rd ); /* watch the reading end from the pipe to the child process */
      FD_SET( serial_fd, &rd ); /* watch the serial port for reading */

      /* select filedescriptors that are ready for reading */
      fds_ready = select( ndfs, &rd, &wr, &er, NULL );

      /* check for errors on select */
      if ( fds_ready == -1 )
        perror("select()");

      /* ------------------------------------------ */
      /* pipe to child process is ready for reading */
      /* ------------------------------------------ */
      if ( FD_ISSET( bowshellpipe[0], &rd ) ) {
        /* read in to readbuf, preserve last place in the array for '\0' */
        cread = read( bowshellpipe[0], readbuf, ( DEFAULT_BUFFER_SIZE - 1 ) );
        if (cread > 0) {
          readbuf[cread] = '\0'; /* make readbuf suitable for printing */
          cwritten = write(serial_fd, readbuf, cread); /* pass on to the serial port */
          if (cwritten < 0)
            perror("writing to serial port error");
          serial_flush(serial_fd);
          cwritten = write(serial_fd, options.eol, strlen(options.eol)); /* end the line properly */
          if (cwritten < 0)
            perror("writing to serial port error");
        }
        else {
          /* reading from child failed so it must have quit */
          return 0;
        }
      }

      /* -------------------------------- */
      /* serial port is ready for reading */
      /* -------------------------------- */
      if ( FD_ISSET( serial_fd, &rd ) ) {
        char *output;
        if ((output = serial_read(serial_fd))) {
          printf("%s", output);
          fflush(stdout);
        }
        else {
          /* serial port was ready to read from, but nothing was read */
          kill(cpid, SIGINT);
          waitpid(cpid, NULL, 0);
          puts("Connection lost to device: quitting");
          return exit_lostconnection;
        }
      }
    }
  }
  puts(EXIT_MSG);
  return exit_success;
}
