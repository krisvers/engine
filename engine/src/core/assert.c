#include <core/assert.h>
#include <core/logger.h>
#include <defines.h>
#include <stdlib.h>

void report_assertion_failure(const char * expression, const char * message, const char * file, u32 line) {
	KFATAL("Assertion failure: (%s), message: \"%s\", file: \"%s\", line: %u\n", expression, message, file, line);
	abort();
}
