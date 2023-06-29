COMPILER = clang
LIBS = -lglfw -lvulkan
FLAGS = -std=c99 -Wall -Wextra -Wpedantic -Iinclude -DNDEBUG
FILES = $(shell find ./src -type f -name "*.c")
BIN = ./bin
EXE = game

all:
	$(COMPILER) $(FLAGS) $(LIBS) $(FILES) -o $(BIN)/$(EXE)
