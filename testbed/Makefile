CC = clang
LIBS = -lglfw -lvulkan
FLAGS = -std=c99 -Wall -Wextra -Wpedantic -Iinclude
DEBUG = -DDEBUG_FLAG
SRCDIR = ./src
FILES = $(shell find $(SRCDIR) -type f -name "*.c")
BINDIR = ./bin
BIN = $(BINDIR)/testbed

.PHONY = clean build

build:
	$(CC) $(FLAGS) $(LIBS) $(FILES) -o $(BIN)

debug:
	$(CC) $(DEBUG) $(FLAGS) $(LIBS) $(FILES) -o $(BIN)-debug

clean:
	@rm -rf $(BINDIR)/*
