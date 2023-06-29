build-engine:
	@echo "> Building engine"
	@make build -C engine/ --no-print-directory

debug-engine:
	@echo "> Building engine debug"
	@make debug -C engine/ --no-print-directory

clean-engine:
	@echo "> Cleaning engine"
	@make clean -C engine/ --no-print-directory

build-testbed:
	@echo "> Building testbed"
	@make build -C testbed/ --no-print-directory

debug-testbed:
	@echo "> Building testbed debug"
	@make debug -C testbed/ --no-print-directory

clean-testbed:
	@echo "> Cleaning testbed"
	@make clean -C testbed/ --no-print-directory

run-testbed:
	@echo "> Running engine testbed"
	@./testbed/bin/testbed

all: build-engine build-testbed
