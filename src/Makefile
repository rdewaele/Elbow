# Copyright (C) 2009 Robrecht Dewaele
#
# This file is part of Elbow.
#
# Elbow is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Elbow is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Elbow.  If not, see <http://www.gnu.org/licenses/>.

#reminders
# $@ = The file name of the target of the rule.
# $< = The name of the first dependency.
# $^ = The names of all the dependencies, with spaces between them.
# $? = The names of all the dependencies that are newer than the target, with spaces between them.
# $* = The stem with which an implicit rule matches. If the target is 'dir/a.foo.b' and the target pattern is 'a.%.b' then the stem is 'dir/foo'. The stem is useful for constructing names of related files.

CCWARNINGS= -W -Wall -Wextra -Wundef -Wendif-labels -Wshadow -Wpointer-arith -Wbad-function-cast -Wcast-align -Wwrite-strings -Wstrict-prototypes -Wmissing-prototypes -Wnested-externs -Winline -Wdisabled-optimization
CCFLAGS= -O3 -pipe -pedantic -std=c99 -D_POSIX_C_SOURCE $(CCWARNINGS)
LDFLAGS= -Wall -lreadline
SOURCES= main.c bowshell.c serial.c sendfiles.c getopt.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE= elbow
DATE=`date +%F_%H-%M-%S`

all: .ctags $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) make.depend
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

%.o: %.c
	$(CC) $(CCFLAGS) -c $< -o $@

.ctags: $(OBJECTS)
	ctags *.c *.h
	touch .ctags

test: $(EXECUTABLE) input.lst
	./$< < input.lst

memleak: $(EXECUTABLE)
	valgrind ./$<

clean:
	@rm -f $(OBJECTS) $(EXECUTABLE)

tar: $(SOURCES)
	tar cvf $(EXECUTABLE)-$(DATE).tar $(SOURCES) *.h Makefile

tarclean:
	@rm -f $(EXECUTABLE)*.tar

mrproper: clean tarclean
	@rm -f .ctags tags make.depend

make.depend: $(SOURCES)
	$(CC) -E -MM $^ > $@

-include make.depend
