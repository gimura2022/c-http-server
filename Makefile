CC       = gcc
CFLAGS   = -Wall -std=c99 -g -O0
LDFLAGS  = 
CPPFLAGS = -I include

SOURCES = src/main.c src/http.c src/gif.c
OBJECTS = $(SOURCES:.c=.o)

.PHONY: all
all: main

.PHONY: clean
clean:
	rm -f $(OBJECTS) main

main: $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
