# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -O2

# Target executable name
TARGET = ConnectOpen

# Source files
SRCS = ConnectOpen.c

# Build target
all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

# Clean target
clean:
	rm -f $(TARGET)
