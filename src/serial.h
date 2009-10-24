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

#ifndef SERIAL_H
#define SERIAL_H

/* initialize a serial device and return file descriptor to it */
int serial_init(const char * device);
/* read from serial device and return a pointer to a newly malloc'd string */
char * serial_read(int fd);
/* empty program's output buffer
 * and terminate it with the device's favorite newline character */
void serial_flush(int fd);

#endif
