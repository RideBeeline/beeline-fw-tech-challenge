#include <stdint.h>
#include <stdbool.h>
#include "definitions.h"

#include "lcd.h"
#include "lcd_config.h"
#include <string.h>
#include "drivers/time.h"

#include "stdio.h"

/**
 * Send a single slice to the screen.
*/
void lcd_send_slice(uint8_t *buf, int y0, int y1, uint8_t *buf_clr);

/*
 * Manually wipe both the front and the back buffer. Not normally needed,
 * only used in error-handling code to ensure buffers are in a clean state.
 */
void lcd_clear_framebuffers(void);


/*
 * Framebuffer(s)
 */
//Peripherals that are using EasyDMA (for example, SPIS) require the transfer buffers to be placed in the data RAM region.
//if this is NOT met then a NRF_ERROR_INVALID_ADDR will be returned and caught by APP_ERROR_CHECK (locking up)
uint8_t display_buffer_a[LCD_BUFSIZE] __attribute__ ((aligned (4)));
uint8_t display_buffer_b[LCD_BUFSIZE] __attribute__ ((aligned (4)));
uint8_t* draw_buffer = display_buffer_a;
uint8_t* refresh_buffer = display_buffer_b;


void lcd_refresh(render_func_t render_func)
{

        if (render_func != NULL) {
            render_func();
        }
		lcd_send_slice(draw_buffer, 0, LCD_HEIGHT-1, refresh_buffer);
		uint8_t* temp = draw_buffer;
		draw_buffer = refresh_buffer;
		refresh_buffer = temp;

}

void lcd_clear_framebuffers(void) {

	memset(display_buffer_a, 0, LCD_BUFSIZE);
	memset(display_buffer_b, 0, LCD_BUFSIZE);

}

void lcd_init(void)
{
	lcd_clear_framebuffers();
}

void lcd_write(uint16_t x, uint16_t y, Colour colour) {

	if ((y < 0) || (y >= LCD_HEIGHT))
		return;

	if(x >= LCD_WIDTH)
		return;

	y = y % LCD_HEIGHT;

	uint16_t *pixels = (uint16_t*) draw_buffer;

	pixels[y * LCD_WIDTH + x] = colour;

}

// Turn a 0-255 lightness value to palette Colour value.
// 0 -> Black
// 255 -> White
void lcd_write_greyscale(uint16_t x, uint16_t y, uint8_t lightness){    
    lcd_write(x, y, RGB888_TO_DISPLAY(lightness, lightness, lightness));
}

void lcd_hline(int x, int y, int len, Colour colour) {

	if ((y < 0) || (y >= LCD_HEIGHT))
		return;

	//This logic prevents things that are off-screen from breaking everything,
	//at the expense of a small amount of performance

	if(x >= LCD_WIDTH)
		return;

	int x1 = x + len;
	
	//Line doesn't appear on screen at all
	if(x1 < 0)
		return;

	if(x < 0)
		x = 0;
		

	if(x1 > LCD_WIDTH)
		x1 = LCD_WIDTH;

	// Clip line to display bounds
	len = x1 - x;

	if(len > 0)
		lcd_hline_unsafe(x, y, len, colour);

}

void lcd_hline_unsafe(uint16_t x, uint16_t y, uint16_t len, Colour colour) { 

	// This code should have the same effect as doing:
	// for(int i = 0; i < len; i++) {
	// 	lcd_write(x+i, y, colour);
	// }
	// but this should be more efficient as we only do the bounds checks / colour shifting / wrapping once.

	if ((y < 0) || (y >= LCD_HEIGHT))
		return;

	y = y % LCD_HEIGHT;


	uint16_t *pixels = (uint16_t*) draw_buffer;

	// It may be possible to write two pixels in one operation by writing (colour | (colour << 16))
	// but this is easier to read/understand and doesn't need a special case for when start/end isn't word-aligned.
	for(int i = (y * LCD_WIDTH + x); i < (y * LCD_WIDTH + x + len); i++) {
		pixels[i] = colour;
	}

}

// This display_ram is to simulate the display buffer in the actual display driver
uint16_t display_ram[LCD_WIDTH * LCD_HEIGHT];

static int sim_spi_transfer_bytes = 0;
// in the real driver we start the SPI transfer and use DMA to clear the buffer. 
void lcd_send_slice(uint8_t* buf, int y0, int y1, uint8_t* buf_clr)
{

	// Assume SPIM transfer takes 10ms
	increment_ms_since_boot(10); 
#if BYTES_PER_PIXEL != 2
#error This code assumes 16bpp
#endif
    
    // copy that part into the display ram buffer
    uint16_t* pixels = (uint16_t*)buf;
    // NB: y1 is given as the last line to be sent!
    for (int y = y0; y <= y1; y++) {
        for (int x = 0; x < LCD_WIDTH; x++) {
            display_ram[y * LCD_WIDTH + x] = pixels[(y % LCD_HEIGHT) * LCD_WIDTH + x];
        }
    }

    // clear the previous buffer 
    uint16_t* pixels_clr = (uint16_t*)buf_clr;
    for (int y = y0; y <= y1; y++) {
        for (int x = 0; x < LCD_WIDTH; x++) {
            pixels_clr[(y % LCD_HEIGHT) * LCD_WIDTH + x] = 0;
        }
    }

    sim_spi_transfer_bytes = LCD_WIDTH * LCD_HEIGHT * BYTES_PER_PIXEL;
}

