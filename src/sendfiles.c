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

#include <stdio.h>
#include <unistd.h>
#include <termios.h>

#include "sendfiles.h"
#include "serial.h"

#define READBUF_SIZE 200
#define STX 0x2
#define ETX 0x3

void sendfiles(int amount, char *files[], int serial_fd) {
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

    /* fill readbuf with file contents, starting with the STX ASCII char */
    readbuf[0] = STX;
    j = 1;
    while (EOF != (cin = getc(currentFile))) {
      /* store character and send buffer to device when full */
      readbuf[j] = cin;
      if (READBUF_SIZE == j) {
        serial_flush(serial_fd);
        cwritten = write(serial_fd, readbuf, j + 1);
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

    /* EOF was reached, write remainder (if present) to device
     * and include the terminating character '\n' */
    readbuf[j++] = ETX;
    cwritten = write(serial_fd, readbuf, j);
    if (cwritten < 0) {
      perror("marking end of file failed");
      return;
    }

    /* wait for other end to finish reading */
    serial_flush(serial_fd);

    /* done processing the current file */
    fclose(currentFile);
  }
}
