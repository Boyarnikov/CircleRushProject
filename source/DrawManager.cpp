#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE
#define _USE_MATH_DEFINES

#include "DrawManager.h"
#include "SpriteManager.h"
#include <math.h>
#include <memory.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <algorithm>


namespace draw_manager {
    double eps = 0.0001;
    
    pixel::pixel(int x, int y) : x(x), y(y) {};
    pixel::pixel() : x(0), y(0) {};

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
        // NOTE: this function really begs for some optimisation. it's really bad performance-vise especialy when called many times for each pixel
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
        color c = mesh_colors(back_color, front_color, double(front_color.a) / 255.0);
        c.a = unsigned char(double(255 - back_color.a) * double(255 - back_color.a) / 255.0);
        return c;
    }

    // set pixel on the buffer to the chosen color. forces it on top of existing colors
    void set_pixel_color_forced(uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH], int x, int y,
        color c = Colors::defoult) {
        uint32_t pixel_color = uint32_from_color(c);

        if (y >= 0 && x >= 0 && y < SCREEN_HEIGHT && x < SCREEN_WIDTH) {
            buffer[y][x] = pixel_color;
        }
    }

    // set pixel on the buffer to the chosen color
    void set_pixel_color(uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH], int x, int y,
        color c = Colors::defoult) {
        if (c.a > 250) {
            set_pixel_color_forced(buffer, x, y, c);
            return;
        }
        
        uint32_t pixel_color = uint32_from_color(c);

        if (y >= 0 && x >= 0 && y < SCREEN_HEIGHT && x < SCREEN_WIDTH) {
            color new_color = add_colors(color_from_uint32(buffer[y][x]), c);
            new_color.a = 255;
            buffer[y][x] = uint32_from_color(new_color);
        }  
    }

    // fill rectangle from (x0, y0) to (x1, y1) with plane color
    void fill_with_color(uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH], int x0, int y0,
        int x1, int y1, color color = Colors::defoult) {
        if (x0 > x1) {
            int swap = x0;
            x0 = x1;
            x1 = swap;
        }
        if (y0 > y1) {
            int swap = y0;
            y0 = y1;
            y1 = swap;
        }
        for (size_t i = x0; i < x1; i++)
        {
            for (size_t j = y0; j < y1; j++)
            {
                set_pixel_color(buffer, i, j, color);
            }
        }
    }

    // NOTE: it really looks like this kind of function could benefit from batch-filling the values instead of filling it pixel by pixel
    //       however it feels like alpha channel cannot be optimised taht way. Maybe create a function just to fill background?
    // NOTE: changing set_pixel_color to set_pixel_color_forced improves fps to flat +20, witch is just kinda sad. 
    // NOTE: ok, in hindsight its really obvious that to fill background with one color we should just assign the value
    // fill whole buffer with plane color 
    void fill_with_color(uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH], color color = Colors::defoult) {
        std::fill_n(*buffer, SCREEN_HEIGHT * SCREEN_WIDTH, uint32_from_color(color));
    }

    // fill rectangle from pixel a to pixel b
    void fill_with_color(uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH], pixel a, pixel b,
        color color = Colors::defoult) {
        fill_with_color(buffer, a.x, a.y, b.x, b.y, color);
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
    void draw_circle_fill(uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH],
        int x, int y, int r, color color = Colors::defoult) {

        for (int dx = x - r; dx <= x + r; dx++) {
            for (int dy = y - r; dy <= y + r; dy++) {
                if ((dx - x) * (dx - x) + (dy - y) * (dy - y) <= r * r) {
                    set_pixel_color(buffer, dx, dy, color);
                }
            }
        }
    }

    // draw filled circle at pixel p with radius r and color
    void draw_circle_fill(uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH],
        pixel p, int r, color color = Colors::defoult) {
        draw_circle_fill(buffer, p.x, p.y, r, color);
    }

    // draw circle segment at (x, y) with radius r and color that starts at angle from and ends at to.
    void draw_circle_segment(uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH],
        int x, int y, int r, double from = 0, double to = 2 * M_PI,
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
    void draw_circle_segment(uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH],
        pixel p, int r, double from = 0, double to = 2 * M_PI,
        color color = Colors::defoult) {
        draw_circle_segment(buffer, p.x, p.y, r, from, to, color);
    }

    // draw polygon frame from vetrices in points
    void draw_polygon(uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH],
        pixel* points, int point_coint, color color) {
        for (size_t i = 0; i < point_coint - 1; i++) {
            draw_line(buffer, points[i], points[i + 1], color);
        }
        draw_line(buffer, points[point_coint - 1], points[0], color);
    }


    spr::sprite font = spr::sprite();

    void init_font() {
        font = spr::sprite("numbers.bmp", 64, 64);
    }

    // draws number on screen. uses sprite as mask, and draw color where the mask is full white
    void draw_number(uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH],
        int x, int y, int n, color color) {
        if (n < 0 || n > 9) return;
        int dx = n * font.spr_width;
        int dy = 0;

        for (size_t i = 0; i < font.spr_width; i++)
        {
            for (size_t j = 0; j < font.spr_height; j++)
            {
                int p = font.pixels[j + dy][i + dx];
                unsigned char* point = (unsigned char*)&p;
                //set_pixel_color(buffer, i + x - 32, j + y - 32, get_color_from_RGBA(point[0] == 255, point[1] == 255, point[2] == 255, point[3] == 255));
                if (!(point[0] == 255 && point[1] == 255 && point[2] == 255))
                    set_pixel_color(buffer, i + x - font.spr_width / 2., j + y - font.spr_height / 2., color);
            }
        }
    }

    // draws positive integer on screen, scale value gives padding between numbers
    void draw_int(uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH],
        int x, int y, int n, color color = Colors::defoult) {
        if (font.height == 0 || font.width == 0) return;

        double scale = 1.4;
        int num_size = 0;
        int num = n;

        while (num > 0) {
            num_size += 1;
            num /= 10;
        }
        if (n == 0) {
            num_size = 1;
        }

        num = n;
        for (size_t i = 0; i < num_size; i++)
        {
            // NOTE: probably should make a sprite-mask function and just draw a sprite frame
            draw_number(buffer, x + ((num_size - 1) * font.spr_width * scale / 2. - double(i * font.spr_width * scale)) / 2.0,
                y - font.spr_height / 2., num % 10, color);
            num /= 10;
        }
    }
}