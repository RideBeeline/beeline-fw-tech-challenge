
// AUTOMATICALLY GENERATED. DO NOT MODIFY DIRECTLY.
// See tools/fonts/build_fonts.py for more information.

#include <stdint.h>

// Must be consistent in multiple places:
// - tools/build_fonts.py 
// - tools/fonts/bdf_to_c_data.py 
// - app/src/graphics/fonts/Fonts.h
// - boot_nrf52/bootloader_secure/fonts/Fonts.h
                    
typedef struct __attribute__((packed))
{
    int8_t bmp_width;   // width in pixels of the bitmap data
    int8_t bmp_height;  // height in pixels of the bitmap data
    int8_t offset_x;    // starting x coordinate of the bitmap 
    int8_t offset_y;    // starting y coordinate of the bitmap
    int8_t shift_x;     // width in pixels of the overall character (DWIDTH X)
    uint16_t offset;    // offset of the bitmap data in the array
} character_info;
                                      

extern const uint8_t bariol_36px_data[];
const character_info get_bariol_36px_descriptor(int c);
extern const uint8_t bariol_64px_data[];
const character_info get_bariol_64px_descriptor(int c);


// Map to get the correct descriptor lookup function
const character_info (*const descriptor_map[])(int) = {
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    get_bariol_36px_descriptor,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    get_bariol_64px_descriptor,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
};

         
// Map to get the correct font data
const uint8_t* const data_map[] = {
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    bariol_36px_data,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    bariol_64px_data,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
};
