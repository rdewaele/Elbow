CFLAGS=-O3 $(shell pkg-config --cflags gtk+-2.0)
LDLIBS=$(shell pkg-config --libs gtk+-2.0)

EXECUTABLE=elbow-gui
MAIN=main
SOURCES=main.c
OBJECTS=$(SOURCES:%.c=%.o)

all: $(EXECUTABLE)

$(EXECUTABLE): $(MAIN)
	mv $< $@

$(MAIN): $(OBJECTS)

clean:
	rm -f $(OBJECTS) $(EXECUTABLE) $(MAIN) $(EXECUTABLE) make.depend

.PHONY: all clean

make.depend: $(SOURCES)
	  $(CC) -E -MM $^ > $@

-include make.depend
