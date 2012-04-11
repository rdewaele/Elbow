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

#ifndef BOWSHELL_H
#define BOWHSELL_H

#include <stdbool.h>

/* print the buffer pointed to by text to stdout
 * will keep user input on the bottom line when present */
int bowshell_print (char *text);

/* initialize bowshell must be called before any other bowshell
 * functionality is used
 * fd is where user input should be sent to */
void bowshell_init(int fd);

/* indicates whether /last/ line handled by bowhsell was EOF */
bool bowshell_eof(void);

/* restore terminal to original state and clean up memory
 * this function effectively unloads bowshell */
void bowshell_cleanup(void);

/* bowshell_notify should be called every time user input
 * becomes vailable, so bowshell can handle it */
void bowshell_notify(void);

#endif
