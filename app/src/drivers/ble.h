
#pragma once

#include "stdbool.h"

#ifdef __cplusplus
extern "C" {
#endif

bool ble_is_connected();

void ble_set_connected(bool c);

#ifdef __cplusplus
}
#endif

