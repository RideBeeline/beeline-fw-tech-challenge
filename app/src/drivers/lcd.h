#ifndef DRIVERS_LCD_H_
#define DRIVERS_LCD_H_

#include <stdint.h>
#include <stdbool.h>
#include "definitions.h" 
#include "lcd_config.h"

// Screen Centre X = Horizontal centre of the screen
#define SCX (LCD_WIDTH / 2)

// Screen Centre Y = Vertical centre of the screen
#define SCY (LCD_HEIGHT / 2)

#ifdef __cplusplus
extern "C" {
#endif



    typedef enum {
        Black = HEX_TO_DISPLAY(0x000000),
        White = HEX_TO_DISPLAY(0xFFFFFF),
        VeryLightGrey = HEX_TO_DISPLAY(0xDDDDDD),
        LightGrey = HEX_TO_DISPLAY(0xBBBBBB),
        MediumGrey = HEX_TO_DISPLAY(0x888888),
        DarkGrey = HEX_TO_DISPLAY(0x444444),
        VeryDarkGrey = HEX_TO_DISPLAY(0x222222),
        Red = HEX_TO_DISPLAY(0xF2594F),
        Orange = HEX_TO_DISPLAY(0xF6882F),
        Yellow = HEX_TO_DISPLAY(0xFFCF00),
        Green = HEX_TO_DISPLAY(0x239B55),
        Blue = HEX_TO_DISPLAY(0x0000FF),
        DarkRed = HEX_TO_DISPLAY(0x590202),
        DarkGreen = HEX_TO_DISPLAY(0x013220),
        DarkYellow = HEX_TO_DISPLAY(0x63521a),
        Transparent = HEX_TO_DISPLAY(0xFF00FF),
        PureRed = HEX_TO_DISPLAY(0xFF0000),
        PureGreen = HEX_TO_DISPLAY(0x00FF00),
        PureBlue = HEX_TO_DISPLAY(0x0000FF),
        Magenta = HEX_TO_DISPLAY(0xFF00FF),
    } Colour;



/**
 * Set a coloured pixel to a grey value between 0 and 255 lightness
 */
void lcd_write_greyscale(uint16_t x, uint16_t y, uint8_t lightness);


/**
 * Initialise the LCD driver and the required peripherals
 */
void lcd_init(void);


typedef void (*render_func_t)();

/**
 * Start a refresh of the LCD - start rendering and transfer to LCD.
 *
 */
void lcd_refresh(render_func_t render_func);

/**
 * Set or clear the pixel at the given position.
 */
void lcd_write(uint16_t x, uint16_t y, Colour colour);

/**
 * @brief Draw a horizontal line to the screen, checking bounds.
 *
 * Draws a line from (x,y), extending in the +x, with length len.
 *
 * @param x X-coordinate of start pixel.
 * @param y Y-coordinate of start pixel.
 * @param len How long to make the line, in pixels.
 * @param set The colour to write
 */
void lcd_hline(int x, int y, int len, Colour colour);


/**
 * @brief Quickly, without checking bounds, draw a horizontal line.
 *
 * Effectively, a line from (x,y) to (x+len, y)
 *
 * @param x X-coordinate of start pixel.
 * @param y Y-coordinate of start pixel.
 * @param len How long to make the line, in pixels.
 * @param set The colour to write
 */
void lcd_hline_unsafe(uint16_t x, uint16_t y, uint16_t len, Colour colour);


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* DRIVERS_LCD_H_ */
