#define _USE_MATH_DEFINES

#include "Engine.h"
#include "DrawManager.h"
#include <stdlib.h>
#include <memory.h>
#include <math.h>


double time = 0;

// initialize game data in this function
void initialize()
{
}

// this function is called to update game data,
// dt - time elapsed since the previous update (in seconds)
void act(float dt)
{
    if (is_key_pressed(VK_ESCAPE))
        schedule_quit_game();

    time += dt;
}

void draw_test_scene() {
    for (uint32_t i = 150; i < 350; i++)
        for (uint32_t j = 50; j < 250; j++)
        {
            if (i%6 < 3 && j % 6 < 3)
            set_pixel_color(buffer, j, i, color(255, 255, 255, 255));
        }

    for (uint32_t i = 0; i < 100; i+=10)
        draw_line(buffer, 50, 50, 150, 50 + i, Colors::blue);

    for (uint32_t i = 0; i < 100; i += 10)
        draw_circle(buffer, 400, 150, 10 + i, color(255, 0, 0, 255));

    for (uint32_t i = 0; i < 100; i += 10)
        draw_circle_segment(buffer, 400, 350, 10 + i, i, i + M_PI, color(i, 255-i, 255, 255));
    
    draw_circle_fill(buffer, 100, 200, 50, color(255, 0, 0, 100));
    draw_circle_fill(buffer, 150, 200, 50, color(0, 255, 0, 100));
    draw_circle_fill(buffer, 125, 225, 50, color(0, 0, 255, 100));

    //draw_circle_fill(buffer, 500, 200, 50, mesh_colors(color(255, 255, 255, 255), color(255, 0, 0, 255), time / 10));
}


// fill buffer in this function
// uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH] - is an array of 32-bit colors (8 bits per R, G, B)
void draw()
{
    // clear backbuffer
    memset(buffer, 0, SCREEN_HEIGHT * SCREEN_WIDTH * sizeof(uint32_t));
    draw_test_scene();
}


// free game data in this function
void finalize()
{
}

