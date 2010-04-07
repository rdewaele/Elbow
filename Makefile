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
DATE=`date +%F_%H-%M-%S`
TARNAME=$(EXECUTABLE)-$(DATE)

all: $(EXECUTABLE)

$(EXECUTABLE): src/main
	@cp $< $(EXECUTABLE)

cleanhere:
	@rm -f $(EXECUTABLE)

mrproperhere: cleanhere
	@rm -f $(EXECUTABLE)-*.tar.bz2

clean: src/clean cleanhere

mrproper: src/mrproper mrproperhere

tar: tmp mrproper
	@tar cvjf $</$(TARNAME).tar.bz2\
		--exclude .git --exclude $< --exclude armpit_tests .\
		&& mv $</$(TARNAME).tar.bz2 .\
		&& rmdir $<

tmp:
	@mkdir tmp

src/%:
	$(MAKE) -C src $*

%::
	$(MAKE) -C src $@

.PHONY: all cleanhere mrproperhere clean mrproper tar
