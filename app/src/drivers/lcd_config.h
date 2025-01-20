
/*
 * Definitions below are not intended to be edited, except when working on the internals of the display driver.
 */


//Colour LCD using partial framebuffering
#define COLOUR

#define BYTES_PER_PIXEL 2

#define LCD_BUFSIZE_UNPADDED ((LCD_WIDTH * LCD_HEIGHT * BYTES_PER_PIXEL))

// Pad the in-memory buffer size up to the nearest 64 bytes, to make CPU clearing of buffer very fast (unrolled loop)
#define ROUND_UP(N, S) ((((N) + (S) - 1) / (S)) * (S))
#define LCD_BUFSIZE ROUND_UP(LCD_BUFSIZE_UNPADDED, 64)



#define LCD_RED_BITS 5
#define LCD_GREEN_BITS 6
#define LCD_BLUE_BITS 5
#define BITS_PER_PIXEL (LCD_RED_BITS + LCD_GREEN_BITS + LCD_BLUE_BITS)
#define LCD_RED_POSITION (5+6)
#define LCD_GREEN_POSITION (5)
#define LCD_BLUE_POSITION (0)

// Pack an RGB888 colour value into a RGB565 value understood by the display
#define RGB888_TO_DISPLAY(r, g, b) END_SWAP_16(( \
								(((r & 0xFF) >> (8-LCD_RED_BITS)) << LCD_RED_POSITION) | \
								(((g & 0xFF) >> (8-LCD_GREEN_BITS)) << LCD_GREEN_POSITION) | \
								(((b & 0xFF) >> (8-LCD_BLUE_BITS)) << LCD_BLUE_POSITION) \
								))

#define LINE_OFFSET_BYTES 0
#define LINE_SIZE_BYTES (LCD_WIDTH * BITS_PER_PIXEL + LINE_OFFSET_BYTES)

#define DISPLAY_COMPONENT_RED(x) ((END_SWAP_16(x)>>LCD_RED_POSITION) & (0xFF >> (8-LCD_RED_BITS)))
#define DISPLAY_COMPONENT_GREEN(x) ((END_SWAP_16(x)>>LCD_GREEN_POSITION) & (0xFF >> (8-LCD_GREEN_BITS)))
#define DISPLAY_COMPONENT_BLUE(x) ((END_SWAP_16(x)>>LCD_BLUE_POSITION) & (0xFF >> (8-LCD_BLUE_BITS)))

#define END_SWAP_16(x) (((x & 0xFF) << 8) | ((x & 0xFF00) >> 8))

#define HEX_TO_DISPLAY(hex) (RGB888_TO_DISPLAY((hex >> 16), (hex >> 8) & 0xFF, hex & 0xFF))
