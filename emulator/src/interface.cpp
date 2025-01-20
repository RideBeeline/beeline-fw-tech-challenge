#include "interface_common.h"

#include <string.h>

#include "definitions.h"
#include "drivers/lcd_config.h"
#include "drivers/time.h"
#include "main.h"
#include <math.h>
#include "drivers/ble.h"



extern uint16_t display_ram[LCD_WIDTH * LCD_HEIGHT];

uint8_t screen[LCD_HEIGHT][LCD_WIDTH][3];

EXPORT_TO_LIB
uint8_t *ei_lcd_get_screen(int *lcd_height, int *lcd_width)
{
    *lcd_height = LCD_HEIGHT;
    *lcd_width = LCD_WIDTH;

    uint16_t *pixels = (uint16_t *)display_ram;

    for (int x = 0; x < LCD_WIDTH; x++)
    {
        for (int y = 0; y < LCD_HEIGHT; y++)
        {

            // Need to tweak this logic for other colour depths
            static_assert(LCD_RED_BITS == 5, "LCD_RED_BITS must be 5");
            static_assert(LCD_GREEN_BITS == 6, "LCD_GREEN_BITS must be 6");
            static_assert(LCD_BLUE_BITS == 5, "LCD_BLUE_BITS must be 5");

            uint16_t colour = END_SWAP_16(pixels[y * LCD_WIDTH + x]);
            uint8_t r_5bit = ((colour >> LCD_RED_POSITION) & ((0xff) >> (8 - LCD_RED_BITS)));
            uint8_t g_6bit = ((colour >> LCD_GREEN_POSITION) & ((0xff) >> (8 - LCD_GREEN_BITS)));
            uint8_t b_5bit = ((colour >> LCD_BLUE_POSITION) & ((0xff) >> (8 - LCD_BLUE_BITS)));

            screen[y][x][0] = round(r_5bit * 255.0 / 31.0);
            screen[y][x][1] = round(g_6bit * 255.0 / 63.0);
            screen[y][x][2] = round(b_5bit * 255.0 / 31.0);
        }
    }

    return &screen[0][0][0];
}

EXPORT_TO_LIB
void ei_loop()
{
    loop();
}

EXPORT_TO_LIB
void ei_increment_ms_since_boot(uint32_t ms)
{
    increment_ms_since_boot(ms);
}

EXPORT_TO_LIB
uint32_t ei_get_ms_since_boot()
{
    return get_ms_since_boot();
}

EXPORT_TO_LIB
void ei_set_connected(bool connected)
{
    ble_set_connected(connected);
}