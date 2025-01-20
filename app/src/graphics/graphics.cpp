/*
 *
 * Draw commands for any primitives that are more complex than horizontal lines or individual pixels
 * This keeps the LCD driver as simple and clean as possible
 */

#include "graphics.h"

#include <stdio.h>

int abs(int a)
{
	if (a < 0)
		return -a;
	return a;
}

#define swap(a, b) \
	{              \
		int t = a; \
		a = b;     \
		b = t;     \
	}

// Fill a triangle
void graphics_triangle_fill(int x0, int y0,
							int x1, int y1,
							int x2, int y2, Colour set)
{

	int a, b, y, last;

	// Sort coordinates by Y order (y2 >= y1 >= y0)
	if (y0 > y1)
	{
		swap(y0, y1);
		swap(x0, x1);
	}
	if (y1 > y2)
	{
		swap(y2, y1);
		swap(x2, x1);
	}
	if (y0 > y1)
	{
		swap(y0, y1);
		swap(x0, x1);
	}

	if (y0 == y2)
	{ // Handle awkward all-on-same-line case as its own thing
		a = b = x0;
		if (x1 < a)
			a = x1;
		else if (x1 > b)
			b = x1;
		if (x2 < a)
			a = x2;
		else if (x2 > b)
			b = x2;
		lcd_hline(a, y0, b - a + 1, set);
		return;
	}

	int16_t
		dx01 = x1 - x0,
		dy01 = y1 - y0,
		dx02 = x2 - x0,
		dy02 = y2 - y0,
		dx12 = x2 - x1,
		dy12 = y2 - y1;
	int32_t
		sa = 0,
		sb = 0;

	// For upper part of triangle, find scanline crossings for segments
	// 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
	// is included here (and second loop will be skipped, avoiding a /0
	// error there), otherwise scanline y1 is skipped here and handled
	// in the second loop...which also avoids a /0 error here if y0=y1
	// (flat-topped triangle).
	if (y1 == y2)
		last = y1; // Include y1 scanline
	else
		last = y1 - 1; // Skip it

	for (y = y0; y <= last; y++)
	{
		a = x0 + sa / dy01;
		b = x0 + sb / dy02;
		sa += dx01;
		sb += dx02;
		/* longhand:
	a = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
	b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
		 */
		if (a > b)
			swap(a, b);
		lcd_hline(a, y, b - a + 1, set);
	}

	// For lower part of triangle, find scanline crossings for segments
	// 0-2 and 1-2.  This loop is skipped if y1=y2.
	sa = dx12 * (y - y1);
	sb = dx02 * (y - y0);
	for (; y <= y2; y++)
	{
		a = x1 + sa / dy12;
		b = x0 + sb / dy02;
		sa += dx12;
		sb += dx02;
		/* longhand:
	a = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
	b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
		 */
		if (a > b)
			swap(a, b);
		lcd_hline(a, y, b - a + 1, set);
	}
}

void graphics_quadrangle_fill(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, Colour set)
{
	graphics_triangle_fill(x0, y0, x1, y1, x2, y2, set);
	graphics_triangle_fill(x0, y0, x3, y3, x2, y2, set);
}

// Implementation of bresenham’s circle drawing algorithm with variable perimeter value
// Allows more precise sizes of circles to be drawn by specifiying the diameter instead of the radius
void graphics_circle_fill_fine(int x0, int y0, int diameter, Colour set)
{

	// diameter one is just a point
	if (diameter == 1)
	{
		lcd_write(x0, y0, set);
		return;
	}

	// max_P is the maximum perimeter. By deviating from 0 one can set circles to be slightly smaller or larger
	int P = 0, max_P = -4 * 8;

	// max_P can be decreased to make fewer pixels be inside the perimeter
	// P and max_P are both scaled by 8 to avoid floating point calculations

	// if the diameter is odd, so that the radius would be e.g 3.5,
	// draw at radius 3 but with a looser P value
	if (diameter % 2)
	{
		diameter--;
		max_P = 0;
	}

	// initialize
	int x = diameter / 2, y = 0;

	while (x >= y)
	{

		// on the first pass, only intialise P
		if (!y)
			P = (2 - diameter) * 8 / 2;

		// on passes other than the first, check if midpoint is inside the perimeter.
		else if (P <= max_P)
			P = P + (2 * y + 1) * 8;

		// Mid-point is outside the perimeter
		else
		{
			x--;
			P = P + (2 * y - 2 * x + 1) * 8;
		}

		// All the perimeter points have already been printed
		if (x < y)
			break;

		// Printing the generated point and its reflection
		// in the other octants after translation

		lcd_hline(x0 - x, y0 + y, 2 * x + 1, set);
		lcd_hline(x0 - x, y0 - y, 2 * x + 1, set);

		if (x != y)
		{ // has just been printed if x==y

			lcd_hline(x0 - y, y0 + x, 2 * y + 1, set);
			lcd_hline(x0 - y, y0 - x, 2 * y + 1, set);
		}
		y++;
	}
}

/*
 * Fill a rectangle with opposing corners at (x0,y0) and (x1, y1)
 */
void graphics_rectangle_fill(int x0, int y0, int x1, int y1, Colour colour)
{

	// Ensure x0 <= x1 and y0 <= y1
	if (x0 > x1)
	{
		int temp = x1;
		x1 = x0;
		x0 = temp;
	}
	if (y0 > y1)
	{
		int temp = y1;
		y1 = y0;
		y0 = temp;
	}

	// If the lower of our coordinates is above the upper bound, the entire shape is off-screen.
	if (y0 >= LCD_HEIGHT)
		return;

	if (x0 >= LCD_WIDTH)
		return;

	// If we start or finish off the screen, clip to the screen bounds
	if (x1 > (LCD_WIDTH - 1))
		x1 = (LCD_WIDTH - 1);

	if (x0 < 0)
		x0 = 0;

	if (y1 > LCD_HEIGHT - 1)
		y1 = LCD_HEIGHT - 1;

	if (y0 < 0)
		y0 = 0;

	// Width of each stroke
	int len = x1 - x0 + 1;

	for (int aty = y0; aty <= y1; aty++)
	{
		lcd_hline_unsafe(x0, aty, len, colour);
	}
}

/*
 * Implementation of Bresenham's algorithm
 */
void graphics_line(int x0, int y0,
				   int x1, int y1,
				   Colour color)
{
	int steep = abs(y1 - y0) > abs(x1 - x0);
	if (steep)
	{
		swap(x0, y0);
		swap(x1, y1);
	}

	if (x0 > x1)
	{
		swap(x0, x1);
		swap(y0, y1);
	}

	int dx, dy;
	dx = x1 - x0;
	dy = abs(y1 - y0);

	int err = dx / 2;
	int ystep;

	if (y0 < y1)
	{
		ystep = 1;
	}
	else
	{
		ystep = -1;
	}

	for (; x0 <= x1; x0++)
	{
		if (steep)
		{
			lcd_write(y0, x0, color);
		}
		else
		{
			lcd_write(x0, y0, color);
		}
		err -= dy;
		if (err < 0)
		{
			y0 += ystep;
			err += dx;
		}
	}
}
