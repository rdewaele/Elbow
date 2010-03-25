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
#include "options.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <readline/readline.h>
#include <readline/history.h>

#define DEFAULT_BUFFER_SIZE 256

/* ---- */
/* MAIN */
/* ---- */
int main(int argc, const char * argv[]) {
	int cread, serial_fd, stdin_fd, nfds, fds_ready;
	char readbuf[DEFAULT_BUFFER_SIZE];

	/* set options from commandline */
	setOptions(argc, argv);

	/* initialize the serial port */
	serial_fd = serial_init(settings.device);

	/* check whether there are files to be sent first */
	if (settings.file)
		sendfiles(1, &(settings.file), serial_fd);

	/* initialize bowshell */
	bowshell_init(serial_fd);

	/* define & clear "fd_set"s for select */
	fd_set rd, wr, er; /* read, write, and error file descriptor sets*/
	FD_ZERO(&wr); /* won't be used, so one-time zero */
	FD_ZERO(&er); /* won't be used, so one-time zero */

	stdin_fd = fileno(stdin);

	nfds = 1 + (serial_fd > stdin_fd ? serial_fd : stdin_fd);

	/* start receiving from the pipe to the child process and from the
	 * serial device */
	for (;;) {
		/* reset "fd_set"s each loop because select() modifies it */
		FD_ZERO( &rd );
		FD_SET( stdin_fd, &rd );
		FD_SET( serial_fd, &rd ); /* watch the serial port for reading */

		/* select filedescriptors that are ready for reading */
		fds_ready = select( nfds, &rd, &wr, &er, NULL );
		if ( fds_ready == -1 ) { perror("select()"); }

		/* stdin ready */
		if (FD_ISSET(stdin_fd, &rd))
			bowshell_notify();

		/* serial ready */
		if (FD_ISSET(serial_fd, &rd)) {
			cread = read(serial_fd, readbuf, (DEFAULT_BUFFER_SIZE - 1));
			if (cread < 0)
				perror("serial read");
			else {
				if (cread == 0) {
					puts("serial disconnect");
					rl_callback_handler_remove();
					exit(1);
				}
				else {
					readbuf[cread] = '\0'; /* make readbuf suitable for printing */
					bowshell_print(readbuf);
				}
			}
		}
	}
	return EXIT_SUCCESS;
}
