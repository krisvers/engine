#include <core/clock.h>
#include <core/logger.h>
#include <platform/platform.h>

void clock_update(clock_t * clock) {
    if (clock->start != 0) {
        clock->elapsed = platform_get_absolute_time() - clock->start;
    }
}

void clock_start(clock_t * clock) {
    clock->start = platform_get_absolute_time();
    clock->elapsed = 0;
}

void clock_stop(clock_t * clock) {
    clock->start = 0;
}
