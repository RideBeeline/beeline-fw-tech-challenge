
#include "graphics/font_renderer.h"
#include "graphics/graphics.h"
#include "drivers/time.h"
#include "drivers/ble.h"
#include "drivers/lcd.h"

// A pill is a button like object with text on top.
void draw_pill(const char *string, int y, Colour bg_clr, Colour fg_clr)
{

    constexpr FontSize fs = Size_36px;
    uint16_t lw = get_line_width(string, fs);
    int height = get_font_height(fs) * 1292 / 850;

    constexpr int padding_x = -5;
    int width = padding_x + lw + padding_x;

    int x0 = SCX - width / 2;
    int x1 = x0 + width;
    int y0 = y + height / 2;
    int y1 = y - height / 2;

    graphics_rectangle_fill(x0, y0, x1, y1, bg_clr);

    graphics_circle_fill_fine(x0, y, height, bg_clr);
    graphics_circle_fill_fine(x1, y, height, bg_clr);

    x0 = SCX;
    draw_string_centred(string, x0, y, fs, fg_clr);
    return;
}

void format_time(char *string, int h, int m)
{

    // Generate string with format hh:mm. Don't show first h if it's zero.
    h >= 10 ? (string[0] = '0' + h / 10) : (string[0] = ' ');
    string[1] = '0' + h % 10;
    string[2] = ':';
    string[3] = '0' + m / 10;
    string[4] = '0' + m % 10;
    string[5] = '\0';
}

void draw_time_string(int x, int y, int hours, int minutes, FontSize font_size, Colour colour)
{
    char string[6];
    format_time(string, hours, minutes);
    draw_string_centred(string, x, y, font_size, colour);
}

void draw_velo2_home()
{

    clock_time ct = get_time();

    bool is_connected = ble_is_connected();

    constexpr int time_y_pos_connected = 91;
    constexpr int time_y_pos_disconnected = 104;
    int time_y_pos = is_connected ? time_y_pos_connected : time_y_pos_disconnected;

    constexpr int battery_y_offset = -40;

    // pretend battery icon (actually just a circle)
    graphics_circle_fill_fine(SCX, time_y_pos + battery_y_offset, 10, White);

    draw_time_string(SCX, time_y_pos, ct.hours, ct.minutes, Size_64px, White);
    draw_pill("Let's Ride!", time_y_pos + 55, is_connected ? Yellow : White, Black);

    if (is_connected)
    {

        constexpr int phone_connected_y_offset = 95;
        // pretend phone icon (actually just a circle too)
        graphics_circle_fill_fine(SCX, time_y_pos + phone_connected_y_offset, 30, Yellow);
    }
}