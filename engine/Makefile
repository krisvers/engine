CC = clang
LIBS = -lglfw -lvulkan
FLAGS = -std=c99 -Wall -Wextra -Wpedantic -Wno-unused-parameter -Wno-gnu-zero-variadic-macro-arguments -Iinclude -fPIC -shared -ferror-limit=100 -DKEXPORT
DEBUG = -DDEBUG_FLAG
SRCDIR = ./src
FILES = $(shell find $(SRCDIR) -type f -name "*.c")
BINDIR = ./bin
BIN = $(BINDIR)/libengine.so

WINCC = x86_64-w64-mingw32-gcc
WINLIBS = -Lwinlib -lglfw3 -lgdi32 -lkernel32
WINFLAGS = -std=c99 -Wall -Wextra -Wno-unused-parameter -Iinclude -Iwinclude -mwindows -Wl,-subsystem,windows -DKEXPORT -DKWINDOWS_DLL -DGLFW_DLL -shared
WINBIN = $(BINDIR)/libengine.dll

VERSION = version-0.1_0
PKGDIR = package/

.PHONY = clean build

build:
	$(CC) $(FLAGS) $(LIBS) $(FILES) -o $(BIN)

debug:
	$(CC) $(DEBUG) $(FLAGS) $(LIBS) $(FILES) -o $(BIN)

win:
	$(WINCC) $(WINFLAGS) $(WINLIBS) $(FILES) -o $(WINBIN)

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