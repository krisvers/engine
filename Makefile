OS = $(shell uname -sm)

all: build-engine build-testbed

build-engine:
	@echo "> Building engine for $(OS)"
	@make build -C engine/ --no-print-directory

debug-engine:
	@echo "> Building engine debug for $(OS)"
	@make debug -C engine/ --no-print-directory

clean-engine:
	@echo "> Cleaning engine"
	@make clean -C engine/ --no-print-directory

build-testbed:
	@echo "> Building testbed for $(OS)"
	@make build -C engine/testbed/ --no-print-directory

debug-testbed:
	@echo "> Building testbed debug for $(OS)"
	@make debug -C engine/testbed/ --no-print-directory

clean-testbed:
	@echo "> Cleaning testbed"
	@make clean -C engine/testbed/ --no-print-directory

run-testbed:
	@echo "> Running engine testbed for $(OS)"
	@./engine/testbed/bin/testbed

build-engine-win:
	@echo "> Building engine for Windows x64"
	@make win -C engine/ --no-print-directory

build-testbed-win:
	@echo "> Building testbed for Windows x64"
	@make win -C engine/testbed/ --no-print-directory

run-testbed-win:
	@echo "> Running engine testbed.exe"
	@./engine/testbed/bin/testbed.exe

build-engine-mac:
	@echo "> Building engine for $(OS)"
	@make mac -C engine/ --no-print-directory

win: build-engine-win build-testbed-win
mac: build-engine-mac
debug: debug-engine debug-testbed
clean: clean-engine clean-testbed
run: run-testbed
run-win: run-testbed-win
