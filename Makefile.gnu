# Copyright (C) 2010 Robrecht Dewaele
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

EXECUTABLE=elbow
SOURCEDIR=src
TARFILES=COPYING INSTALL Makefile README src

EXECPATH=$(SOURCEDIR)/$(EXECUTABLE)
DATE=$(shell date +%F_%H-%M-%S)
TARNAME=$(EXECUTABLE)-$(DATE)
TARDIRNAME=$(TARNAME)


$(EXECUTABLE): $(EXECPATH)
	cp $(SOURCEDIR)/$(EXECUTABLE) $(EXECUTABLE)

src/%:
	$(MAKE) -C src $*

%::
	$(MAKE) -C $(SOURCEDIR) $@

clean:
	rm -f $(EXECUTABLE)
	$(MAKE) -C $(SOURCEDIR) $@

mrproper: clean
	rm -f $(EXECUTABLE)-*.tar.bz2
	$(MAKE) -C $(SOURCEDIR) $@

tar: $(TARDIRNAME) mrproper
	cp -r $(TARFILES) $<
	tar cvjf $(TARNAME).tar.bz2 $<
	rm -rf $<

$(TARDIRNAME):
	rm -rf $@
	mkdir $@

.PHONY: clean mrproper tar $(TARDIRNAME)
