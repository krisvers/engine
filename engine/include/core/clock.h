#ifndef KENGINE_CORE_CLOCK_H
#define KENGINE_CORE_CLOCK_H

#include <defines.h>

typedef struct clock {
    f64 start;
    f64 elapsed;
} clock_t;

void clock_update(clock_t * clock);
void clock_start(clock_t * clock);
void clock_stop(clock_t * clock);

#endif
