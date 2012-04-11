/*
 * Copyright (C) 2009-2012 Robrecht Dewaele
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

#define PROGRESS_INTERVAL 200
#define READBUF_SIZE 200
#define STX 0x2
#define ETX 0x3

void replacechars(char * str, char from, char to) {
	while ((str = strchr(str, from)))
		*str = to;
}

void sendfiles(int filesc, const char * filesv[], int serialfd) {
	int filesfd;
	int i;
	ssize_t n_read, n_swritten;
	off_t n_current, n_total;
	unsigned int progress;
	char readbuf[READBUF_SIZE];

	/* loop through files and send them */
	for (i = 0; i < filesc; ++i) {
		filesfd = open(filesv[i], O_RDONLY);
		if (-1 == filesfd) {
			perror(filesv[i]);
			continue;
		}
		n_total = lseek(filesfd, 0, SEEK_END);
		progress = 0;
		fprintf(stderr, "%s, %lu bytes\n", filesv[i], n_total);
		lseek(filesfd, 0, SEEK_SET);
		/* send file until no more bytes are read */
		while ((n_read = read(filesfd, readbuf, READBUF_SIZE)) > 0) {
			/* TODO hackery has to go */
			replacechars(readbuf, '\n', '\r');
			replacechars(readbuf, '\t', ' ');
			n_swritten = serial_write(serialfd, readbuf, n_read);
			if ((progress += n_swritten) > PROGRESS_INTERVAL) {
				n_current = lseek(filesfd, 0, SEEK_CUR);
				fprintf(stderr, "\b\b\b\b    \b\b\b\b%lu%%", 100 * n_current / n_total);
			}
		}
		fprintf(stderr, "\n");
	}
	return;
}
