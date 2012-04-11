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

/* TODO
 * Bowshell is currently only able to use the callback interface from
 * readline. When READLINE_CALLBACKS is not defined, this interface is
 * unavailable. A possible solution is to provide an alternative
 * implementation, using fork() and pipes.
 */

#include "bowshell.h"
#include "serial.h"
#include "options.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <readline/readline.h> /* gnu readline */
#include <readline/history.h> /* gnu readline history */

#define DEFAULT_PROMPT ""

/* indicates whether /last/ line handled by bowhsell was EOF */
static bool read_eof = false;

/* file descriptor bowshell will write to */
static int target_fd;

/* forward declarations */
static size_t collapse_char_cont(char *, const size_t, const char, char);
/* XXX currently unused */
//static size_t collapse_char(char *, const size_t, const char);

/* collapse a sequence of a given char into a single occurrence of that char
 * <*> This function is intended for use on chunks of a bigger string. The
 *     'previous' parameter indicates what the character was before the
 *     first character of the current string.
 * <*> returns new buffer length */
size_t collapse_char_cont(char *buffer, const size_t buflen, const char seq, char previous) {
	size_t i;
	unsigned int count = 0;

	/* a buffer with fewer than two characters cannot contain duplicates */
	if (buflen < 2)
		return 1;

	for (i = 0; i < buflen; ++i) {
		if ((seq == previous) && (previous == buffer[i]))
			++count;
		previous = buffer[i - count] = buffer[i];
	}
	return i - count;
}

/* XXX currently unused */
/* collapse a sequence of a given char into a single occurrence of that char
 * <*> returns new buffer length */
//size_t collapse_char(char *buffer, const size_t buflen, const char seq) {
//	size_t i;
//	char previous = buffer[0];
//	unsigned int count = 0;
//
//	/* a buffer with fewer than two characters cannot contain duplicates */
//	if (buflen < 2)
//		return 1;
//
//	for (i = 1; i < buflen; ++i) {
//		if ((seq == previous) && (previous == buffer[i]))
//			++count;
//		previous = buffer[i - count] = buffer[i];
//	}
//	return i - count;
//}

/* GNU readline callback function, to process user input. */
static void bowshell_callback(char *line) {
	/* EOF is indicated by line == NULL (see GNU readline documentation) */
	if (line) {
		serial_write(target_fd, line, strlen(line));
		serial_write(target_fd, settings.eol, 1);
		if (*line) /* non empty lines go in to history */
			add_history (line);
		rl_free(line);
		read_eof = false;
	}
	else
		read_eof = true;
}

/* set fd for serial output and install the bowshell callback function */
void bowshell_init(int new_target_fd) {
	target_fd = new_target_fd;
	rl_callback_handler_install(DEFAULT_PROMPT, bowshell_callback);
}

/* return read_eof status */
bool bowshell_eof(void) {
	return read_eof;
}

/* restore terminal to original state and clean up memory
 * this function effectively unloads bowshell */
void bowshell_cleanup(void) {
	rl_free_line_state();
	rl_free_undo_list();
	clear_history();
	rl_callback_handler_remove();
}

/* feed next character on the input to readline
 * the bowhsell callback will be called when this character completes the
 * line (see GNU readline documentation) */
void bowshell_notify() {
	rl_callback_read_char();
}

/* print the buffer pointed to by text to stdout (will keep user input on
 * bottom line when present */
int bowshell_print (char *text) {
	int ret;
	unsigned int i;
	static char last = '\n';
	rl_save_prompt();
	
	/* TODO allow collapsing of characters other than \n (notably \r)*/
	if (settings.nodup_crnl)
		collapse_char_cont(text, strlen(text) + 1 /* +1: copy '\0' */, '\n', last);

	/* erase current line */
	for (i = 0; i < strlen(rl_line_buffer); ++i)
		printf("\b \b");

	/* XXX */
#ifdef DEBUG
	printf("\n\\n=%d \\r=%d -- |", '\n', '\r');
	for (i = 0; i < strlen(text); ++i)
		printf(" %d |", text[i]);
	puts("\n");
#endif /* DEBUG */

	/* print line and return number of characters printed */
	ret = printf("%s", text);
	rl_forced_update_display();
	last = text[ret - 1];
	return ret;
}
