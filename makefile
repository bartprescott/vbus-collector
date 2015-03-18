CC=gcc
CFLAGS=-c -Wall
LDFLAGS=
SOURCES=kbhit.c checksum.c serial.c vbus.c main.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=vbus-collector

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm *.o
	rm $(EXECUTABLE)

