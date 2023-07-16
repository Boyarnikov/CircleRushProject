#include "DrawManager.h"
#include "Engine.h"


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
    char* b_ptr = (char*)point;
    char* g_ptr = (char*)point + 1;
    char* r_ptr = (char*)point + 2;
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



