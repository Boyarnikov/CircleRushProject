#include "Engine.h"
#include "DrawManager.h"
#include <stdlib.h>
#include <memory.h>


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
}

void draw_test_scene() {
    draw_circle(buffer, 20, 20, 10, color(255, 0, 0, 255));
    draw_line(buffer, 10, 20, 30, 40, Colors::blue);
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

