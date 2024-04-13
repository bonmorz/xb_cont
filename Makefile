# Makefile

CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LDFLAGS = -lusb-1.0

# Target executable
TARGET = xboxcont

all: $(TARGET)

$(TARGET): xboxcont.c
	$(CC) $(CFLAGS) -o $(TARGET) xboxcont.c $(LDFLAGS)

clean:
	rm -f $(TARGET)