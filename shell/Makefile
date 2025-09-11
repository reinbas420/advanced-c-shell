## LLM Generated Starts here
# Compiler and Flags
CC      := gcc
CFLAGS  := -std=c99 -D_POSIX_C_SOURCE=200809L -D_XOPEN_SOURCE=700 \
            -Wall -Wextra -Werror -Wno-unused-parameter -fno-asm
INCLUDES := -Iinclude

# Directories
SRCDIR  := src
BUILDDIR:= build

# Target binary
TARGET  := shell.out

# Files
SOURCES := $(wildcard $(SRCDIR)/*.c)
OBJECTS := $(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/%.o,$(SOURCES))

# Default rule
all: $(TARGET)

# Link object files into final binary
$(TARGET): $(OBJECTS) | $(BUILDDIR)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS)

# Compile .c into .o
$(BUILDDIR)/%.o: $(SRCDIR)/%.c | $(BUILDDIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Run the shell
run: $(TARGET)
	./$(TARGET)

# Debug build (adds -g)
debug: CFLAGS += -g
debug: clean all

# Memory check with Valgrind
valgrind: $(TARGET)
	valgrind --leak-check=full --track-origins=yes ./$(TARGET)

# Clean rules
clean:
	rm -f $(BUILDDIR)/*.o

distclean: clean
	rm -f $(TARGET)

# Ensure dirs exist
$(BUILDDIR):
	mkdir -p $@

# Phony targets
.PHONY: all clean distclean run debug valgrind
#LLM Generated ends here