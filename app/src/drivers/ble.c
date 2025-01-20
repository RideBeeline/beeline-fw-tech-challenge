#include "ble.h"

bool connected = false;

bool ble_is_connected()
{
    return connected;
}

void ble_set_connected(bool c)
{
    connected = c;
}

