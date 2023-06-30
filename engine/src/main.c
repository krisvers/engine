#include <stdio.h>
#include <defines.h>
#include <core/logger.h>
#include <core/assert.h>

int main(void) {
	log_init();
	log_set_logfile("log.txt");
	KLOG("test test test %s", "123");
	log_deinit();

	KASSERT_MSG(1 != 1, "im a goofy goober yeah");

	return 0;
}
