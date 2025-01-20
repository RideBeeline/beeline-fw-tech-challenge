#ifndef FONTRENDERER_H
#define FONTRENDERER_H

#include <stdint.h>
#include "fonts/fonts.h"
#include "graphics.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    JUSTIFY_CENTRE,
    JUSTIFY_LEFT,
    JUSTIFY_RIGHT,
} Justify;


void draw_string_centred(const char *string, uint16_t x, uint16_t y, FontSize fs, Colour fill);
void draw_string_justified(const char *string, uint16_t x, uint16_t y, Justify justify, FontSize fs, Colour fill);
uint16_t get_line_width(const char *string, FontSize fs);


#ifdef __cplusplus
}
#endif


#endif //FONTRENDERER_H
