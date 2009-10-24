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

#include <stdio.h>
#include <stdlib.h>

#include <readline/readline.h> /* gnu readline */
#include <readline/history.h> /* gnu readline history */

#define DEFAULT_PROMPT ""


/* read a string, return a pointer to it
 * Returns NULL on EOF */
char * rl_gets (void) {

	/* static variable to store read lines */
	static char * line_read = (char *)NULL;

	/* if the buffer has already been allocated,
	 * return the memory to the free pool */
	if (line_read != NULL) {
		free (line_read);
		line_read = (char *)NULL;
	}

	/* get a line */
	line_read = readline ( DEFAULT_PROMPT );

	/* save non-empty lines in the history */
	if (line_read && *line_read)
		add_history (line_read);

	return (line_read);
}
