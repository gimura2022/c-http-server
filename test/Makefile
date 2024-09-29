CC       = gcc
CFLAGS   = -Wall -std=c99 -g -O0
LDFLAGS  = 
CPPFLAGS = -I include

SOURCES = main.c 
OBJECTS = $(SOURCES:.c=.o)

.PHONY: all
all: main

.PHONY: clean
clean:
	rm -f $(OBJECTS) main

main: $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
