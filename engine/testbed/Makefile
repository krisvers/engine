CC = clang
LIBS = -L../bin -lglfw -lGL -lengine -lm
FLAGS = -std=c99 -Wall -Wextra -Wpedantic -Wno-newline-eof -Wno-gnu-zero-variadic-macro-arguments -Iinclude -I../include -DKIMPORT -fPIC -g -Wl,-rpath,. -Wno-unused-parameter -O3
DEBUG = -DDEBUG_FLAG
SRCDIR = ./src
FILES = $(shell find $(SRCDIR) -type f -name "*.c")
BINDIR = ./bin
BIN = $(BINDIR)/testbed

WINCC = x86_64-w64-mingw32-gcc
WINLIBS = -L../bin -L../winlib -lengine -lm
WINFLAGS = -std=c99 -Wall -Wextra -Wno-unused-parameter -Iinclude -I../include -DKIMPORT -fPIC -g -Wl,-rpath,. -Wno-unused-parameter -O3
WINRELEASEFLAGS = -mwindows -Wl,-subsystem,windows
WINBIN = $(BINDIR)/testbed.exe
WINDEBUG = -DDEBUG_FLAG -D_DEBUG

MACLIBS = -L../bin -L../maclib -lengine -lglfw3 -lm
MACFLAGS = -std=c99 -Wall -Wextra -Wpedantic -Wno-gnu-zero-variadic-macro-arguments -Iinclude -I../include -DKIMPORT -shared -undefined dynamic_lookup -Wno-unused-parameter -O3
MACBIN = $(BINDIR)/testbed.dylib

.PHONY = clean build

build:
	$(CC) $(FLAGS) $(LIBS) $(FILES) -o $(BIN)

debug:
	$(CC) $(DEBUG) $(FLAGS) $(LIBS) $(FILES) -o $(BIN)

win:
	$(WINCC) $(WINFLAGS) $(WINRELEASEFLAGS) $(WINLIBS) $(FILES) -o $(WINBIN)

debug-win:
	$(WINCC) $(WINDEBUG) $(WINFLAGS) $(WINLIBS) $(FILES) -o $(WINBIN)

mac:
	$(CC) $(MACFLAGS) $(MACLIBS) $(FILES) -o $(MACBIN)

clean:
	@rm -rf $(BINDIR)/*
