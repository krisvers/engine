CC = clang
LIBS = -lglfw -lm
FLAGS = -std=c99 -Wall -Wextra -Wpedantic -Wno-newline-eof -Wno-unused-parameter -Wno-gnu-pointer-arith -Wno-gnu-zero-variadic-macro-arguments -Iinclude -fPIC -shared -DKEXPORT -Wno-gnu-empty-initializer -Wno-unused-variable -Wno-sign-compare -Wno-missing-field-initializers # -ferror-limit=100
DEBUG = -DDEBUG_FLAG -g
SRCDIR = ./src
FILES = $(shell find $(SRCDIR) -type f -name "*.c")
BINDIR = ./bin
BINNAME = libengine.so
BIN = $(BINDIR)/$(BINNAME)

OPENGLFLAGS = -lGL -Iopengl-include
WINOPENGLFLAGS = -lopengl32 -Iopengl-include

WINCC = x86_64-w64-mingw32-gcc
WINLIBS = -Lwinlib -lglfw3 -lgdi32 -lkernel32 -lm
WINFLAGS = -std=c99 -Wall -Wextra -Wno-newline-eof -Wno-unused-parameter -Iinclude -Iwinclude -mwindows -Wl,-subsystem,windows -DKEXPORT -DKWINDOWS_DLL -DGLFW_DLL -shared -DVK_PROTOTYPES -DVK_USE_PLATFORM_WIN32_KHR
WINBIN = $(BINDIR)/libengine.dll
WINDEBUG = -DDEBUG_FLAG -D_DEBUG

MACFLAGS = -std=c99 -Wall -Wextra -Wpedantic -Wno-newline-eof -Wno-unused-parameter -Wno-gnu-pointer-arith -Wno-gnu-zero-variadic-macro-arguments -Imacinclude -Iinclude -shared -undefined dynamic_lookup -ferror-limit=1000 -DKEXPORT
MACLIBS = -Lmaclib -lglfw3 -framework Cocoa -framework IOKit -lm
MACBIN = $(BINDIR)/libengine.dylib

VERSION = version-0.1_0
PKGDIR = package/

INSTALLDIR = /usr/lib

.PHONY = clean build

build:
	$(CC) $(OPENGLFLAGS) $(FLAGS) $(LIBS) $(FILES) -o $(BIN)

debug:
	$(CC) $(OPENGLFLAGS) $(DEBUG) $(FLAGS) $(LIBS) $(FILES) -o $(BIN)

install:
	@cp $(BIN) $(INSTALLDIR)/$(BINNAME)

mac:
	$(CC) $(OPENGLFLAGS) $(MACFLAGS) $(MACLIBS) $(FILES) -o $(MACBIN)

win:
	$(WINCC) $(WINOPENGLFLAGS) $(WINFLAGS) $(WINLIBS) $(FILES) -o $(WINBIN)

debug-win:
	$(WINCC) $(WINOPENGLFLAGS) $(WINDEBUG) $(WINFLAGS) $(WINLIBS) $(FILES) -o $(WINBIN)

pkg: clean build
	@rm -f $(PKGDIR)/engine-linux-$(VERSION).tar.gz
	@tar -czvf $(PKGDIR)/engine-linux-$(VERSION).tar.gz bin/

pkg-win: clean_ win
	@cp ./glfw3.dll $(BINDIR)
	@cp -R $(BINDIR) ./$(PKGDIR)
	@cd $(PKGDIR) &&		mv ./bin engine-win-$(VERSION) &&		zip engine-win-$(VERSION).zip engine-win-$(VERSION) -r &&		rm -rf engine-win-$(VERSION)

pkg-all: pkg pkg-win

clean clean_:
	@rm -rf $(BINDIR)/*
