#pragma once

#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;
} clock_time;

uint32_t get_ms_since_boot();

void increment_ms_since_boot(uint32_t ms);

clock_time get_time();

#ifdef __cplusplus
}
#endif