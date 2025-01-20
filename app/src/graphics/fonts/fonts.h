#ifndef SRC_GRAPHICS_FONTS_FONTSCOMMON_H_
#define SRC_GRAPHICS_FONTS_FONTSCOMMON_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif
// Must be consistent in multiple places:
// - tools/build_fonts.py 
// - tools/fonts/bdf_to_c_data.py 
// - app/src/graphics/fonts/Fonts.h
// - boot_nrf52/bootloader_secure/fonts/Fonts.h

typedef enum {
    Size_18px = 0,
    Size_24px,
    Size_32px,
    Size_36px,
    Size_40px,
    Size_48px,
    Size_64px,
    Size_72px,
    Size_96px,
} FontSize;

typedef struct __attribute__((packed))
{
    int8_t bmp_width;   // width in pixels of the bitmap data
    int8_t bmp_height;  // height in pixels of the bitmap data
    int8_t offset_x;    // starting x coordinate of the bitmap 
    int8_t offset_y;    // starting y coordinate of the bitmap
    int8_t shift_x;     // width in pixels of the overall character (DWIDTH X)
    uint16_t offset;    // offset of the bitmap data in the array
} character_info;

character_info font_character_info_get(int c, FontSize fs);
const uint8_t* font_bitmap_data_get(int c, FontSize fs);
int get_font_size(FontSize fs);
int get_font_height(FontSize fs);
int get_font_line_spacing(FontSize fs);
int get_font_midpoint(FontSize fs);

#ifdef __cplusplus
}
#endif

#endif /* SRC_GRAPHICS_FONTS_FONTSCOMMON_H_ */
