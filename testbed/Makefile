CC = clang
LIBS = -lglfw -lvulkan
FLAGS = -std=c99 -Wall -Wextra -Wpedantic -Wno-gnu-zero-variadic-macro-arguments -Iinclude -I../engine/include
DEBUG = -DDEBUG_FLAG
SRCDIR = ./src
FILES = $(shell find $(SRCDIR) -type f -name "*.c")
BINDIR = ./bin
BIN = $(BINDIR)/testbed

.PHONY = clean build

build:
	$(CC) $(FLAGS) $(LIBS) $(FILES) -o $(BIN)

debug:
	$(CC) $(DEBUG) $(FLAGS) $(LIBS) $(FILES) -o $(BIN)

clean:
	@rm -rf $(BINDIR)/*
