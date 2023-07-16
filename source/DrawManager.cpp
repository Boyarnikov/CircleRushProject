#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE
#define _USE_MATH_DEFINES

#include "DrawManager.h"
#include <math.h>
#include <memory.h>
#include <stdlib.h>
#include <iostream>

double eps = 0.0001;

color::color(unsigned char r = 255, unsigned char g = 255, unsigned char b = 255, unsigned char a = 255)
    : r(r), g(g), b(b), a(a) {};


color Colors::bgc = color(100, 164, 225, 255);
color Colors::dark = color(80, 124, 180, 255);
color Colors::green = color(100, 225, 164, 255);
color Colors::blue = color(100, 225, 225, 255);
color Colors::white = color(150, 225, 225, 255);
color Colors::defoult = color(225, 0, 0, 255);


uint32_t uint32_from_color(color c) {
    uint32_t color;
    uint32_t* point = &color;
    unsigned char* r_ptr = (unsigned char*)point + 2;
    unsigned char* g_ptr = (unsigned char*)point + 1;
    unsigned char* b_ptr = (unsigned char*)point + 0;
    unsigned char* a_ptr = (unsigned char*)point + 3;
    *r_ptr = c.r;
    *g_ptr = c.g;
    *b_ptr = c.b;
    *a_ptr = c.a;
    return color;
}



color color_from_uint32(uint32_t pixel_color) {
    color c;
    uint32_t* point = &pixel_color;
    unsigned char* r_ptr = (unsigned char*)point + 2;
    unsigned char* g_ptr = (unsigned char*)point + 1;
    unsigned char* b_ptr = (unsigned char*)point + 0;
    unsigned char* a_ptr = (unsigned char*)point + 3;
    c.r = *r_ptr;
    c.g = *g_ptr;
    c.b = *b_ptr;
    c.a = *a_ptr;
    return c;
}

// mesh between back_color to front_color using t value where t=0 means back_color and t=1 means front_color 
color mesh_colors(color start_color, color end_color, double t) {
    if (t <= 0) {
        return start_color;
    }
    if (t >= 1) {
        return end_color;
    }
    color c(
        unsigned char((double(start_color.r) * (1 - t)) + (double(end_color.r) * t)),
        unsigned char((double(start_color.g) * (1 - t)) + (double(end_color.g) * t)),
        unsigned char((double(start_color.b) * (1 - t)) + (double(end_color.b) * t)),
        unsigned char((double(start_color.a) * (1 - t)) + (double(end_color.a) * t)));
    return c;
}


// add back_color to front_color using alpha channel
color add_colors(color back_color, color front_color) {
    // adding colors just uses alpha of the 
    color c = mesh_colors(front_color, back_color, double(front_color.a) / 255.0);
    c.a = unsigned char(double(255 - back_color.a) * double(255 - back_color.a) / 255.0);
    return c;
}

// sets pixel on the buffer to the chosen color
void set_pixel_color(uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH], int x, int y,
    color c = Colors::defoult) {
    uint32_t pixel_color = uint32_from_color(c);

    if (y >= 0 && x >= 0 && y < SCREEN_HEIGHT && x < SCREEN_WIDTH) {
        if (c.a == 255) {
            buffer[y][x] = pixel_color;
        } 
        else {
            color new_color = add_colors(color_from_uint32(buffer[y][x]), c);
            new_color.a = 255;
            buffer[y][x] = uint32_from_color(new_color);
        }
    }  
}

// draw staight line from (x0, y0) to (x1, y1)
void draw_line(uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH], int x0, int y0,
    int x1, int y1, color color = Colors::defoult) {
    // this algorithm uses the Bresenham's line algorithm, so some adjustments
    // need to be made 1) line should be drawn from left to right 2) line should
    // be drawn from up to down 3) slope shoul be less then 1 becouse of that,
    // some transformations needs to done

    // delta position
    int dx = x0;
    int dy = y0;
    // end position from delta
    int x_end = x1 - dx;
    int y_end = y1 - dy;
    // is x and y reversed for slope adjustment
    int x_reverce = 1;
    int y_reverce = 1;
    bool is_diagonal_reverced = false;
    // fixing slope
    if (abs(x_end) < abs(y_end)) {
        is_diagonal_reverced = true;
        int swap = x_end;
        x_end = y_end;
        y_end = swap;
    }
    // revercing to go from left to right
    if (x_end < 0) {
        x_reverce = -1;
        x_end *= -1;
    }
    // revercing to go from up to down
    if (y_end < 0) {
        y_reverce = -1;
        y_end *= -1;
    }

    int slope = 2 * (y_end);
    int slope_error = slope - (x_end);
    for (int x = 0, y = 0; x <= x_end; x++) {
        if (!is_diagonal_reverced)
            set_pixel_color(buffer, x * x_reverce + dx, y * y_reverce + dy,
                color);
        else
            set_pixel_color(buffer, y * y_reverce + dx, x * x_reverce + dy,
                color);

        slope_error += slope;
        if (slope_error >= 0) {
            y++;
            slope_error -= 2 * (x_end);
        }
    }
}

// draw staight line from pixel a to pixel b
void draw_line(uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH], pixel a, pixel b,
    color color = Colors::defoult) {
    draw_line(buffer, a.x, a.y, b.x, b.y, color);
}

// draw circle at (x, y) with radius r and color
void draw_circle(uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH], int x, int y,
    int r, color color = Colors::defoult) {
    for (double a = 0; a <= 2 * M_PI; a += 1 / (2 * M_PI * double(r))) {
        int endx = int(double(r) * cos(a));
        int endy = int(double(r) * sin(a));
        set_pixel_color(buffer, x + endx, y + endy, color);
    }
}

// draw circle at pixel p with radius r and color
void draw_circle(uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH], pixel p, int r,
    color color = Colors::defoult) {
    draw_circle(buffer, p.x, p.y, r, color);
}

// draw filled circle at (x, y) p with radius r and color
void draw_circle_fill(uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH], int x,
    int y, int r, color color = Colors::defoult) {

    for (int dx = x - r; dx <= x + r; dx++) {
        for (int dy = y - r; dy <= y + r; dy++) {
            if ((dx - x) * (dx - x) + (dy - y) * (dy - y) <= r * r) {
                set_pixel_color(buffer, dx, dy, color);
            }
        }
    }
}

// draw filled circle at pixel p with radius r and color
void draw_circle_fill(uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH], pixel p,
    int r, color color = Colors::defoult) {
    draw_circle_fill(buffer, p.x, p.y, r, color);
}

// draw circle segment at (x, y) with radius r and color that starts at angle from and ends at to.
void draw_circle_segment(uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH], int x,
    int y, int r, double from = 0, double to = 2 * M_PI,
    color color = Colors::defoult) {
    // this algorithm iterates through angles from from to to
    // in angles such iteration only makes sence if we are going from from
    // clockwise until we reach first from at 2pi period because of that angles
    // are downscaled to 0 to 2pi and 0 to 4 pi respectively

    // TODO: get rid of while's
    while (to < from) {
        to += M_PI * 2;
    }

    while (to > from + M_PI * 2 + eps) {
        to -= M_PI * 2;
    }

    if (to - from < eps) {
        return;
    }

    for (double a = from; a <= to; a += 1 / (2 * M_PI * double(r))) {
        int endx = int(double(r) * cos(a));
        int endy = int(double(r) * sin(a));
        
        set_pixel_color(buffer, x + endx, y + endy, color);
    }
}

// draw circle segment at pixel p with radius r and color that starts at angle
// from and ends at to
void draw_circle_segment(uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH], pixel p,
    int r, double from = 0, double to = 2 * M_PI,
    color color = Colors::defoult) {
    draw_circle_segment(buffer, p.x, p.y, r, from, to, color);
}