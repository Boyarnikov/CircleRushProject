#include "DrawManager.h"
#include <math.h>
#include <memory.h>
#include <stdlib.h>

double pi = 3.1415;
double eps = 0.00001;

color::color(char b = 255, char g = 255, char r = 255, char a = 255)
    : b(b), g(g), r(r), a(a) {};


color Colors::bgc = color(100, 164, 225, 255);
color Colors::dark = color(80, 124, 180, 255);
color Colors::green = color(100, 225, 164, 255);
color Colors::blue = color(100, 225, 225, 255);
color Colors::white = color(150, 225, 225, 255);
color Colors::defoult = color(225, 0, 0, 255);


uint32_t uint32_from_color(color c) {
    uint32_t color;
    uint32_t* point = &color;
    char* r_ptr = (char*)point;
    char* g_ptr = (char*)point + 1;
    char* b_ptr = (char*)point + 2;
    char* a_ptr = (char*)point + 3;
    //   char *b_ptr = static_cast<char*>(point);
    *r_ptr = c.r;
    *g_ptr = c.g;
    *b_ptr = c.b;
    *a_ptr = c.a;
    return color;
}


char add_channel_with_alpha(char back, char front, char alpha) {
    return front * (float(alpha) / 255) + back * (1.0f - float(alpha) / 255);
}


// add back_color to front_color using alpha channel
color mesh_colors(color back_color, color front_color, double t) {
    color color(
        add_channel_with_alpha(back_color.r, front_color.r, back_color.a),
        add_channel_with_alpha(back_color.g, front_color.g, back_color.a),
        add_channel_with_alpha(back_color.b, front_color.b, back_color.a),
        back_color.a);
    return color;
}


void set_pixel_color(uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH], int x, int y,
    color color = Colors::defoult) {
    uint32_t pixel_color = uint32_from_color(color);
    // char* a_ptr = (char*)&color + 3;
    if (y >= 0 && x >= 0 && y < SCREEN_HEIGHT && x < SCREEN_WIDTH)
        buffer[y][x] = pixel_color;
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
    for (double a = 0; a <= 2 * pi; a += 1 / (2 * pi * double(r))) {
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
