CC = clang
LIBS = -L../bin -lglfw -lvulkan -lengine
FLAGS = -std=c99 -Wall -Wextra -Wpedantic -Wno-gnu-zero-variadic-macro-arguments -Iinclude -I../include -DKIMPORT -fPIC -g -Wl,-rpath,.
DEBUG = -DDEBUG_FLAG
SRCDIR = ./src
FILES = $(shell find $(SRCDIR) -type f -name "*.c")
BINDIR = ./bin
BIN = $(BINDIR)/testbed

WINCC = x86_64-w64-mingw32-gcc
WINLIBS = -L../bin -lengine
WINFLAGS = -std=c99 -Wall -Wextra -Wno-unused-parameter -Iinclude -I../include -mwindows -Wl,-subsystem,windows -DKIMPORT -fPIC -g -Wl,-rpath,.
WINBIN = $(BINDIR)/testbed.exe

.PHONY = clean build

build:
	$(CC) $(FLAGS) $(LIBS) $(FILES) -o $(BIN)

debug:
	$(CC) $(DEBUG) $(FLAGS) $(LIBS) $(FILES) -o $(BIN)

win:
	$(WINCC) $(WINFLAGS) $(WINLIBS) $(FILES) -o $(WINBIN)

clean:
	@rm -rf $(BINDIR)/*