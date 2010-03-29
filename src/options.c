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
#include <unistd.h>
#include <termios.h>
#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <popt.h>

#include "options.h"

/* default settings */
struct settings settings = {
	B9600,
	"/dev/ttyUSB0",
	"\r",
	NULL,
	false
};

#define BPAIR(x) {x, B ## x}
#define BPAIR_SIZE (sizeof((int[])BPAIR(50)) / sizeof(int))
#define BAUDRATES_SIZE (sizeof(baudrates) / (BPAIR_SIZE * sizeof(int *)))

/* array of valid (baudrate number, baudrate constant) pairs */
static int baudrates[][BPAIR_SIZE] =
{
	BPAIR(50), BPAIR(75), BPAIR(110), BPAIR(134),	BPAIR(150),
	BPAIR(200), BPAIR(300), BPAIR(600), BPAIR(1200), BPAIR(1800),
	BPAIR(2400), BPAIR(4800), BPAIR(9600), BPAIR(19200), BPAIR(38400),
	BPAIR(57600), BPAIR(115200), BPAIR(230400)
};

#define ENDLINES_SIZE (sizeof(endlines) / ((sizeof(*endlines) / sizeof(**endlines)) * sizeof(char *)))

/* array maps commandline 'cr', 'lf' and 'crlf' to the real ascii characters */
static const char * endlines[][2] =
{
	{ "cr", "\r" },
	{ "lf", "\n" },
	{ "crlf", "\r\n" }
};

/* print all valid baudrate numbers */
static void printBaudrates(void) {
	unsigned int i;
	printf("Invalid baudrate!\nValid baudrates are: ");
	for (i = 0; i < BAUDRATES_SIZE - 1; ++i)
		printf("%d, ", baudrates[i][0]);
	printf("%d\n", baudrates[i][0]);
}

/* lookup baudrate number and return baudrate constant
 * exits when baudrate number is invalid */
static int getBaudrate(int baudrate) {
	unsigned int i;
	for (i = 0; i < BAUDRATES_SIZE; ++i) {
		if (baudrates[i][0] == baudrate)
			return baudrates[i][1];
	}

	printBaudrates();
	exit(EXIT_FAILURE);
}

static const char * getEndOfLine(const char * description) {
	unsigned int i;

	for (i = 0; i < ENDLINES_SIZE; ++i)
		if (0 == strcmp(description, endlines[i][0]))
			return endlines[i][1];

	puts("invalid end of line, see help for valid options");
	exit(EXIT_FAILURE);
}

/*********************************************************/
/* libpopt structures defining all the options for Elbow */
/*********************************************************/

/*
 * b = baudrate
 * d = device
 * e = end of line
 * f = file
 */

static struct poptOption elbowOptionsTable[] = {
	{ "baudrate", 'b', POPT_ARG_INT, &settings.rate, 2,
		"baudrate to use for communication",
		"baudrate" },

	{ "device", 'd', POPT_ARG_STRING, &settings.device, 0,
		"device node to connect to",
		"device" },

	{ "eol", 'e', POPT_ARG_STRING, &settings.eol, 1,
		"end of line character to use - valid: cr, lf, crlf",
		"end of line" },

	{ "file", 'f', POPT_ARG_STRING, &settings.file, 0,
		"file to send before starting interactive session",
		"file" },

	{ "stxetx", 'S', POPT_ARG_NONE, &settings.stxetx, 0,
		"use STX/ETX ascii codes to mark start/end of a file transmission",
		NULL },

	POPT_TABLEEND
};

static struct poptOption options[] = {
	{ NULL, '\0', POPT_ARG_INCLUDE_TABLE, NULL, 0,
		"Elbow options",
		NULL },

	POPT_AUTOHELP	POPT_TABLEEND
};

void setOptions(int argc, const char * argv[]) {
	int ret;

	poptContext optCon = poptGetContext(NULL, argc, argv, options, 0);

	options[0].arg = elbowOptionsTable;

	while ((ret = poptGetNextOpt(optCon))) {
		if (-1 == ret) /* parsing done */
			break;

		switch (ret) {
			case 1: /* eol */
				settings.eol = getEndOfLine(settings.eol);
				break;
			case 2: /* baudrate */
				settings.rate = getBaudrate(settings.rate);
				break;
		}
	}

	optCon = poptFreeContext(optCon);
}
