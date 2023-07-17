#pragma once
#include "Engine.h"
#include "SpriteManager.h"

namespace draw_manager {
    struct pixel {
        int x = 0;
        int y = 0;
        pixel(int, int);
        pixel();
    };

    // color class with RGBA
    struct color {
        unsigned char b = 0;
        unsigned char g = 0;
        unsigned char r = 0;
        unsigned char a = 0;

        color(unsigned char, unsigned char, unsigned char, unsigned char);
    };

    // basic colors for game
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
    color add_colors(color, color);
    color mesh_colors(color, color, double t);
    void fill_with_color(uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH], int x0, int y0, int x1, int y1, color);
    void fill_with_color(uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH], color);
    void fill_with_color(uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH], pixel a, pixel b, color);
    void set_pixel_color(uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH], int x, int y, color);
    void draw_line(uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH], int x0, int y0, int x1, int y1, color);
    void draw_line(uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH], pixel a, pixel b, color);
    void draw_circle(uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH], int x, int y, int r, color);
    void draw_circle(uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH], pixel p, int r, color);
    void draw_circle_fill(uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH], int x, int y, int r, color);
    void draw_circle_fill(uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH], pixel, int r, color);
    void draw_circle_segment(uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH], int x, int y, int r, double from, double to, color);
    void draw_circle_segment(uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH], pixel, int r, double from, double to, color);
    void draw_polygon(uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH], pixel*, int, color);
    void draw_number(uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH], spr::sprite font, int x, int y, int n, color);
    void draw_int(uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH], spr::sprite font, int x, int y, int n, double scale, color);
}