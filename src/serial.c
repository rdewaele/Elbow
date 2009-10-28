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

#include "serial.h"
#include "getopt.h"

#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* returns a filedescirptor to the selected serial port */
int serial_init(const char * device) {
  int fd;
  struct termios newtio;

  fd = open(device, O_RDWR | O_NOCTTY);
  if (fd < 0) {
    perror(device);
    exit(-1);
  }

  tcgetattr(fd,&newtio); /* save current serial port settings */

  newtio.c_cflag &= ~(CSIZE | PARENB | CSTOPB);
  newtio.c_cflag |= CS8 | CREAD | CLOCAL;

  newtio.c_iflag &= ~(INPCK | IXON | IXOFF | IXANY);

  newtio.c_oflag &= ~(OPOST);

  newtio.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
  newtio.c_lflag |= ICRNL;

  newtio.c_cc[VTIME]    = 0;     /* inter-character timer unused */
  newtio.c_cc[VMIN]     = 0;     /* blocking read until 1 character arrives */

  cfsetispeed(&newtio, options.rate);
  cfsetospeed(&newtio, options.rate);

  /* when we are _initialising_ a device, we generally don't care what was
   * left on its output buffer (maybe from previous operations or whatever
   *   TCIFLUSH flushes data received but not read */
  tcflush(fd, TCSAFLUSH);

  /* apply settings */
  if (-1 == tcsetattr(fd,TCSANOW,&newtio)) {
    perror("tcsetattr");
  }

  return fd;
}

char * serial_read(int fd) {
  int res;
  static char buf[256];
  res = read(fd,buf,255);

  /* EOF or ERROR: return NULL */
  switch (res) {
    case -1:
      perror("serial_read()");
    case 0:
      return NULL;
  }

  /* successful read: mark end of string and return it */
  buf[res] = '\0';

#ifdef DEBUG
	puts("\nDEBUG: serial read buffer on next line:");
	for(int i = 0; i < res; ++i)
		printf("%d ", buf[i]);
	printf("\n");
#endif /* #ifdef DEBUG */

  return buf;
}

/* empty program's output buffer */
void serial_flush(int fd) {
  tcdrain(fd);
}
