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
#include "options.h"

#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* returns a filedescriptor to the selected serial port */
int serial_init(const char * device) {
  int fd;
  struct termios newtio;

  fd = open(device, O_RDWR | O_NOCTTY | O_NONBLOCK);
  if (fd < 0) {
    perror(device);
    exit(-1);
  }

  tcgetattr(fd,&newtio); /* save current serial port settings */

  newtio.c_cflag &= ~(CSIZE | PARENB | CSTOPB);
  newtio.c_cflag |= CS8 | CREAD | CLOCAL;

  newtio.c_iflag &= ~(INPCK | IXON | IXOFF | IXANY);
	newtio.c_iflag |= ICRNL;

  newtio.c_oflag &= ~(OCRNL | OPOST);

  newtio.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

  newtio.c_cc[VTIME]    = 0;     /* inter-character timer unused */
  newtio.c_cc[VMIN]     = 1;     /* blocking read until 1 character arrives */

  cfsetispeed(&newtio, settings.rate);
  cfsetospeed(&newtio, settings.rate);

  /* when we are _initialising_ a device, we generally don't care what was
   * left on its output buffer (maybe from previous operations or whatever
   *   TCIFLUSH flushes data received but not read */
  tcflush(fd, TCSAFLUSH);

  /* apply settings */
  if (-1 == tcsetattr(fd,TCSANOW,&newtio)) {
    perror("tcsetattr");
  }

	/* TODO: add call to tcgetattr(fd,oldtio) to check whether all changes
	 * were successful */

  return fd;
}

ssize_t serial_write(int serial_fd, const char *buf, size_t len) {
	ssize_t cwritten;
	cwritten = write(serial_fd, buf, len);
	tcdrain(serial_fd);
	return cwritten;
}
