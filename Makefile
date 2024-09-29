CC       = gcc
CFLAGS   = -Wall -std=c99 -g -O0
LDFLAGS  = 
CPPFLAGS = -I include

SOURCES = src/http_server.c 
OBJECTS = $(SOURCES:.c=.o)

TARGET = http_server

.PHONY: all
all: lib$(TARGET).a lib$(TARGET).so 

.PHONY: clean
clean:
	rm -f $(OBJECTS) lib$(TARGET).a lib$(TARGET).so

lib$(TARGET).so: $(OBJECTS)
	$(CC) $(CFLAGS) -shared -o $@ $^ $(LDFLAGS)

lib$(TARGET).a: $(OBJECTS)
	ar rcs $@ $^
