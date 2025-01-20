#include "font_renderer.h"

// Used for debugging and to ensure that we have all the glyphs required by the UI
#include <stdio.h>
bool font_error_occurred = false;

typedef struct
{
    const char *string;
    int byteAt;
} string_iterator;

// Get the next printable character from a (possibly UTF-8) array of bytes
int iterator_get(string_iterator *it)
{

    int firstCharacter = it->string[it->byteAt];

    // If the character starts 0b0_______ then it's 1 byte wide (ASCII)
    if (!(firstCharacter & 0b10000000))
    {
        int character = firstCharacter;
        it->byteAt++;
        return character;
    }

    // If the character starts 0b110_____ then it's 2 bytes wide
    if ((firstCharacter & 0b11100000) == 0b11000000)
    {
        int character = (((it->string[it->byteAt] & 0b00011111) << 6) | (it->string[it->byteAt + 1] & 0b00111111));
        it->byteAt += 2;
        return character;
    }

    // If the character starts 0b1110____ then it's 3 bytes wide
    if ((firstCharacter & 0b11110000) == 0b11100000)
    {
        int character = (((it->string[it->byteAt] & 0b00011111) << 12) | ((it->string[it->byteAt + 1] & 0b00111111) << 6) | (it->string[it->byteAt + 2] & 0b00111111));
        it->byteAt += 3;
        return character;
    }

    // If the character starts 0b11110___ then it's 4 bytes wide
    if ((firstCharacter & 0b11111000) == 0b11110000)
    {
        int character = (((it->string[it->byteAt] & 0b00011111) << 18) | ((it->string[it->byteAt + 1] & 0b00111111) << 12) | ((it->string[it->byteAt + 2] & 0b00111111) << 6) | (it->string[it->byteAt + 3] & 0b00111111));
        it->byteAt += 4;
        return character;
    }

    fprintf(stderr, "Bad UTF8 data! Got %i when processing %s\n", firstCharacter, it->string);
    font_error_occurred = true;
    // Terminate the string processing
    return '\0';
}

string_iterator iterator_create(const char *str)
{
    string_iterator s;
    s.byteAt = 0;
    s.string = str;
    return s;
}

// Helper Functions
uint8_t draw_char(int c, uint16_t x, uint16_t y, FontSize fs, Colour fill);
uint16_t get_character_width(int c, FontSize fs);
uint16_t get_line_width(const char *string, FontSize fs);

uint16_t get_character_width(int c, FontSize fs)
{

    character_info ci = font_character_info_get(c, fs);

    if (ci.bmp_width < 0)
    {

        fprintf(stderr, "Character %i is not in font for size %i\n", c, get_font_size(fs));

        font_error_occurred = true;

        return 0;
    }

    return ci.shift_x;
}

// Draw a single character to the screen and return the width of the character
uint8_t draw_char(int c, uint16_t x, uint16_t y, FontSize fs, Colour fill)
{

    const character_info ci = font_character_info_get(c, fs);

    if (ci.bmp_width < 0)
    {

        fprintf(stderr, "Character %i is not in font for size %i\n", c, get_font_size(fs));

        font_error_occurred = true;

        return 0;
    }

    // Bitmap data for the given character
    // Consists of horizontal rows ("scan lines") containing pixel data, left aligned, and rounded up to the nearest byte.
    // This means, for example, a 17-pixel wide bitmap is 24 bits wide, with the final 7 bits on each row being 0.
    const uint8_t *characterBitmapData = (uint8_t *)(font_bitmap_data_get(c, fs) + ci.offset);

    const int rowWidthBytes = (ci.bmp_width + 7) / 8;
    const int y_off = (-ci.bmp_height - ci.offset_y);
    const int x_off = ci.offset_x;

    // We rarely have any text horizontally offscreen, so checking for it is a waste of time.


    const int y_top_offset = 0;
    const int y_bottom_offset = ci.bmp_height;

    // For each horizontal scan line containing pixel data
    for (int ay = y_top_offset; ay < y_bottom_offset; ay++)
    {
        const int y_coord = y + ay + y_off;

        // For each pixel along the scan line
        for (int ax = 0; ax < ci.bmp_width; ax++)
        {

            const int x_coord = x + ax + x_off;

            // We should fill the current pixel if the corresponding bit in the character data bitmap is set.
            bool fillPixel = characterBitmapData[ay * rowWidthBytes + (ax >> 3)] & ((1 << 7) >> (ax & 7));

            if (fillPixel)
            {
                lcd_write(x_coord, y_coord, fill);
            }
        }
    }

    return ci.shift_x;
}

// Draw a single line of text, returning the number of bytes consumed
int draw_single_line_string(const char *string, unsigned short x, unsigned short y, FontSize fs, Colour fill)
{

    string_iterator it = iterator_create(string);

    for (int nextChar = iterator_get(&it); nextChar != '\0' && nextChar != '\n'; nextChar = iterator_get(&it))
    {

        x += draw_char(nextChar, x, y, fs, fill);

        if (font_error_occurred)
        {
            fprintf(stderr, "A font error occurred whilst drawing string %s in size %i. \n", string, get_font_size(fs));
            return it.byteAt;
        }
    }

    return it.byteAt;
}

/**
 * Draw a string, possibly multi-lined, with the centre at (X, Y)
 * This is just short-hand for draw_string_justified in JUSTIFY_CENTRE mode.
 */
void draw_string_centred(const char *string, uint16_t x, uint16_t y, FontSize fs, Colour fill)
{
    draw_string_justified(string, x, y, JUSTIFY_CENTRE, fs, fill);
}

void draw_string_justified(const char *string, uint16_t x, uint16_t y, Justify justify, FontSize fs, Colour fill)
{

    int height = get_font_height(fs);

    // If the entire string doesn't fit, shift everything to compensate
    // TODO

    // Start at the top, we don't check for screen overflowing here!
    int sy = ((int)y - height / 2) + get_font_midpoint(fs) + get_font_height(fs) / 2;

    int sx = x; // Compiler complains if this isn't initialised, even though all cases are covered in the switch

    switch (justify)
    {
    case JUSTIFY_LEFT:
        sx = x;
        break;
    case JUSTIFY_RIGHT:
        sx = x - get_line_width(string, fs);
        ;

        break;
    case JUSTIFY_CENTRE:
        sx = x - (get_line_width(string, fs) / 2);
        break;
    }

    draw_single_line_string(string, sx, sy, fs, fill);
}

/**
 * @brief Get the width of one specific line of text (ie width between the start and the first \n or \0)
 */
uint16_t get_line_width(const char *string, FontSize fs)
{

    uint16_t w = 0;

    string_iterator it = iterator_create(string);

    for (int nextChar = iterator_get(&it); nextChar != '\0' && nextChar != '\n'; nextChar = iterator_get(&it))
    {

        w += get_character_width(nextChar, fs);

#if defined(BEELINE_EMULATOR)
        if (font_error_occurred && stop_on_error)
        {
            fprintf(stderr, "A font error occurred whilst calculating line width for string %s in size %i. Stopping at byte %i\n", string, get_font_size(fs), it.byteAt);
            return it.byteAt;
        }
#endif
    }
    return w;
}
