#include "fonts.h"

extern const uint8_t* data_map[];
extern character_info (*descriptor_map[])(int c);

typedef enum {
    BariolBold = 0,
    MPlus,
    SourceHanSans,
    NumFonts
} FontName;

static inline int offset_of_entry(int c, FontSize font_size) {

    // For each (font size + font name) combination, two things are automatically generated:
    // - The descriptor lookup function, which returns the metadata for a given character
    // - The bitmap data
    
    // These are indexed in two arrays, called descriptor_map and data_map respectively.
    // The arrays are sorted first by font size, then by font name.
    // [0]: Bariol 18px, [1]: MPlus 18px, [2]: Source Han Sans 18px,
    // [3]: Bariol 24px, [4]: MPlus 24px, [5]: Source Han Sans 24px,
    // [6]: Bariol 32px, [7]: MPlus 32px, [8]: Source Han Sans 32px,
    // ...
    
    // For Taiwanese and Japanese characters outside the Latin codepages, use the special fonts.
    // Otherwise default to Bariol
    int font_name = BariolBold;

    return (font_size * NumFonts) + font_name;
}

character_info font_character_info_get(int c, FontSize fs) {

    const int offset = offset_of_entry(c, fs);
    
    if(descriptor_map[offset]) {
        return descriptor_map[offset](c);
    }

    return (character_info) {-1, -1, -1, -1, -1, 0};
}

const uint8_t* font_bitmap_data_get(int c, FontSize fs) {

    return data_map[offset_of_entry(c, fs)];
}

int get_font_size(FontSize fs) {

    switch(fs) {
        case Size_18px:
            return 18;
        case Size_24px:
            return 24;
        case Size_32px:
            return 32;
        case Size_36px:
            return 36;
        case Size_40px:
            return 40;
        case Size_48px:
            return 48;
        case Size_64px:
            return 64;
        case Size_72px:
            return 72;
        case Size_96px:
            return 96;
        default: 
            return 0;
    }
}

// Vertical size of the O character
int get_font_height(FontSize fs) {

    switch(fs) {
        case Size_18px:
            return 13;
        case Size_24px:
            return 17;
        case Size_32px:
            return 22;
        case Size_36px:
            return 25;
        case Size_40px:
            return 28;
        case Size_48px:
            return 34;
        case Size_64px:
            return 45;
        case Size_72px:
            return 50;
        case Size_96px:
            return 66;
        default: 
            return 0;
    }
}


int get_font_line_spacing(FontSize fs) 
{
    // This is a design-led choice.
    // 1 + 1/3 line height (so 1/3 line spacing) looks decent.
    return get_font_height(fs) / 3; 
}

// defines midpoint for font-centering
int get_font_midpoint(FontSize fs)
{
    // This is a design-led choice.
    // A little less would look more balanced when vertically 
    // centering but 50% makes it easier to define arbitrary positions.
    return 50 * get_font_height(fs) / 100;
}
