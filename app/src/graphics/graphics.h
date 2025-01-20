#ifndef SRC_GRAPHICS_GRAPHICS_H_
#define SRC_GRAPHICS_GRAPHICS_H_

#include "definitions.h"
#include "drivers/lcd.h"


void graphics_triangle_fill(int, int, int, int, int, int, Colour);

void graphics_quadrangle_fill(int x0, int y0, int x1, int y1,int x2, int y2, int x3, int y3, Colour colour);


void graphics_circle_fill_fine(int x0, int y0, int diameter, Colour colour);

void graphics_rectangle_fill(int x0, int y0, int x1, int y1, Colour colour);
void graphics_line(int x, int y, int x1, int y1, Colour colour);


#endif /* SRC_GRAPHICS_GRAPHICS_H_ */
