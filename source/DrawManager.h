#pragma once
#include "Engine.h"

struct pixel {
    int x = 0;
    int y = 0;
};

// color class with RGBA
struct color {
    char b = 0;
    char g = 0;
    char r = 0;
    char a = 0;

    color(char, char, char, char);
};


class Colors {
public:
    static color bgc;
    static color dark;
    static color green;
    static color blue;
    static color white;
    static color defoult;
};

uint32_t uint32_from_color(color c);
color mesh_colors(color back_color, color front_color, double t);
void set_pixel_color(uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH], int x, int y,
    color color);
void draw_line(uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH], int x0, int y0,
    int x1, int y1, color color);
void draw_line(uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH], pixel a, pixel b,
    color color);
void draw_circle(uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH], int x, int y,
    int r, color);
void draw_circle(uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH], pixel p, int r,
    color color);

