
#include "time.h"

static uint32_t ms_since_boot;

// overflows after 2^32 ms / (1000 ms/s * 60 * 60 * 24) = 49.7 days
uint32_t get_ms_since_boot(){
    return ms_since_boot;
}


void increment_ms_since_boot(uint32_t ms){
    ms_since_boot += ms;
}

// assume we booted at 13:00:00
clock_time get_time(){

    clock_time ct;
    uint32_t seconds_since_boot = get_ms_since_boot() / 1000;
    ct.hours = (13 + seconds_since_boot / 3600) % 24;
    ct.minutes = (seconds_since_boot / 60) % 60;
    ct.seconds = seconds_since_boot % 60;

    return ct;
}