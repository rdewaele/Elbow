/*
 * Copyright (C) 2009 2010 Robrecht Dewaele
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

#ifndef SENDFILES_H
#define SENDFILES_H

void sendfiles(int filesc, const char * filesv[], int serialfd);

void replacechars(char * str, char from, char to);

#endif
