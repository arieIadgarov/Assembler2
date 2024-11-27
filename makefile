# Compiler and compiler flags
CC := gcc
CFLAGS := -Wall -ansi -pedantic -c
LFLAGS := -Wall -ansi -pedantic

# Find all .c files in the directory
SOURCES := $(wildcard *.c)

# Convert the .c files to .o files
OBJECTS := $(SOURCES:.c=.o)

# Name of the executable
EXECUTABLE := assembler

# Default target
all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LFLAGS) $^ -o $@

# Rule to compile each .c to an .o
%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

# Clean target
clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

# Phony targets
.PHONY: all clean
