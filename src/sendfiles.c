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

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <termios.h>
#include <unistd.h>

#include "sendfiles.h"
#include "serial.h"
#include "options.h"

#define READBUF_SIZE 200
#define STX 0x2
#define ETX 0x3

static void replacechars(char * str, char from, char to) {
	while ((str = strchr(str, from)))
		*str = to;
}

void sendfiles(int filesc, const char * filesv[], int serialfd) {
	int filesfd;
	int i;
	ssize_t n_read;
	char readbuf[READBUF_SIZE];

	for (i = 0; i < filesc; ++i) {
		filesfd = open(filesv[i], O_RDONLY);
		if (-1 == filesfd) {
			perror(filesv[i]);
			continue;
		}
		while ((n_read = read(filesfd, readbuf, READBUF_SIZE)) > 0) {
			replacechars(readbuf, '\n', '\r');
			replacechars(readbuf, '\t', ' ');
			serial_write(serialfd, readbuf, n_read);
		}
	}
	return;
}

static void sendfiles_deprecated(int amount, char *files[], int serial_fd) {
  FILE *currentFile = NULL;
  char readbuf[READBUF_SIZE + 2];
  int cin;
  int cwritten;
  int i, j;

  /* loop over all files*/
  for (i=0; i < amount; ++i) {
    currentFile = fopen(files[i], "r");

    /* error when opening file has failed, and try next file */
    if (!currentFile) {
      fprintf(stderr, "error opening %s\n", files[i]);
      return;
    }

		j = 0;
		if (settings.stxetx) {
    /* fill readbuf with file contents, starting with the STX ASCII char */
    readbuf[j++] = STX;
		}

    while (EOF != (cin = getc(currentFile))) {
      /* store character and send buffer to device when full */
      readbuf[j] = cin;
      if (READBUF_SIZE == j) {
        //serial_flush(serial_fd);
        cwritten = serial_write(serial_fd, readbuf, j + 1);
        if (cwritten < 0) {
          perror("sending file failed");
          return;
        }
        j = 0;
      }
      else {
        ++j;
      }
    }

		if (settings.stxetx) {
    /* EOF was reached, write remainder (if present) to device
     * and include the terminating character '\n' */
    readbuf[j++] = ETX;
		}

    cwritten = serial_write(serial_fd, readbuf, j);
    if (cwritten < 0) {
      perror("marking end of file failed");
      return;
    }

    /* wait for other end to finish reading */
    //serial_flush(serial_fd);

    /* done processing the current file */
    fclose(currentFile);
  }
	/* finalize input by sending the appropriate EOL */
	//serial_write(serial_fd, options.eol, 1); 
}
