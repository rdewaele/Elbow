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

#ifndef OPTIONS_H
#define OPTIONS_H

#include <stdbool.h>
#include <termios.h>

#ifndef __linux__
#define B57600  57600
#define B115200 115200
#define B230400 230400
#endif /* #ifndef __linux__ */

#define VERSION "0.1"
#define YEAR "2009"
#define AUTHOR "Robrecht Dewaele"
#define LICENSE "GNU GPLv3"
#define DISCLAIMER \
	"Elbow is distributed in the hope that it will be useful,\n"\
"but WITHOUT ANY WARRANTY; without even the implied warranty of\n"\
"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the\n"\
"GNU General Public License for more details."

/* this struct is initialized with default options, which will be
 * overridden by command line options when present */
struct settings {
	int rate;
	const char * device;
	const char * eol;
	const char * file;
	bool stxetx;
} settings;

/* parse the command line options and set them in the options struct */
void setOptions(int argc, const char * argv[]);

#endif /* #ifndef OPTIONS_H */
