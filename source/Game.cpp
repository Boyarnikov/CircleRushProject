#define _USE_MATH_DEFINES

#include "Engine.h"
#include "DrawManager.h"
#include "SpriteManager.h"
#include "MovingObject.h"
#include "Object.h"
#include <stdlib.h>
#include <memory.h>
#include <math.h>
#include <vector>

#include <fstream>

using namespace draw_manager;

double global_time = 0;
spr::sprite font = spr::sprite();

std::vector<std::unique_ptr<object>> object_pull;

object m3 = moving_object(global_time,
    tfm::transform(tfm::point(200, 300), 0.),
    tfm::transform(tfm::point(50, 0), 1.),
    "None");



// initialize game data in this function
void initialize()
{
    font = spr::sprite("numbers.bmp", 64, 64);
    object_pull.push_back(
        std::unique_ptr<object>(new moving_object(global_time,
            tfm::transform(tfm::point(200, 300), 0.),
            tfm::transform(tfm::point(50, 0), 1.),
            "None"))
    );
    
    object_pull.push_back(
        std::unique_ptr<object>(
            new moving_object(global_time,
            tfm::transform(tfm::point(100, 300), 0.),
            tfm::transform(tfm::point(50, 0), 3.),
            "None")
        )
    );
    object_pull.push_back(
        std::unique_ptr<object>(
            new object(global_time,
                tfm::transform(
                    tfm::point(100, 200),
                    100)
            )
        )
    );
}



// this function is called to update game data,
// dt - time elapsed since the previous update (in seconds)
void act(float dt)
{
    global_time += dt;

    if (is_key_pressed(VK_ESCAPE))
        schedule_quit_game();

    for (size_t i = 0; i < object_pull.size(); i++)
    {
        object_pull[i]->act(global_time, dt);
    }
    m3.act(global_time, dt);

    
}

void draw_test_scene() {
    fill_with_color(buffer, color(100, 100, 100, 255));


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
        draw_circle_segment(buffer, 400, 375, 10 + i, i + global_time, i + M_PI + global_time, color(i, 255-i, 255, 255));
    
    draw_circle_fill(buffer, 100, 200, 50, color(255, 0, 0, 100));
    draw_circle_fill(buffer, 150, 200, 50, color(0, 255, 0, 100));
    draw_circle_fill(buffer, 125, 225, 50, color(0, 0, 255, 100));

    pixel p[10];

    for (size_t k = 3; k < 10; k++)
    {
        for (size_t j = 0; j < k; j++)
        {
            double i = double(j) / k * 2 * M_PI;
            p[j].x = int(10. * k * cos(i)) + 200;
            p[j].y = int(10. * k * sin(i)) + 500;
        }
        draw_polygon(buffer, p, k, Colors::green);
    }

    
    if (is_mouse_button_pressed(0)) {
        draw_circle_fill(buffer, get_cursor_x(), get_cursor_y(), 20, color(255, 255, 255, 100));
    }
    else {
        draw_circle_fill(buffer, get_cursor_x(), get_cursor_y(), 10, color(255, 255, 255, 100));
    }

    draw_int(buffer, font, 600, 100, int(global_time), 1.0, Colors::defoult);
}


// fill buffer in this function
// uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH] - is an array of 32-bit colors (8 bits per R, G, B)
void draw()
{
    // clear backbuffer
    memset(buffer, 0, SCREEN_HEIGHT * SCREEN_WIDTH * sizeof(uint32_t));
    //draw_test_scene();

    for (size_t i = 0; i < object_pull.size(); i++)
    {
        object_pull[i]->draw(buffer, global_time);
    }
    m3.draw(buffer, global_time);
}


// free game data in this function
void finalize()
{
}

