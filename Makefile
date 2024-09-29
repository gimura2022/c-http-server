CC       = gcc
CFLAGS   = -Wall -std=c99 -g -O0
LDFLAGS  = 
CPPFLAGS = -I include

SOURCES = src/http_server.c 
OBJECTS = $(SOURCES:.c=.o)

TEST_SOURCES = test/main.c
TEST_OBJECTS = $(TEST_SOURCES:.c=.o)

TARGET = http_server

.PHONY: all
all: lib$(TARGET).a lib$(TARGET).so main 

.PHONY: clean
clean:
	rm -f $(OBJECTS) $(TEST_OBJECTS) lib$(TARGET).a lib$(TARGET).so main

lib$(TARGET).so: $(OBJECTS)
	$(CC) $(CFLAGS) -shared -o $@ $^ $(LDFLAGS)

lib$(TARGET).a: $(OBJECTS)
	ar rcs $@ $^

main: $(TEST_OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) ./libhttp_server.so
