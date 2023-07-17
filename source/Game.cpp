#define _USE_MATH_DEFINES

#include "Engine.h"
#include "DrawManager.h"
#include "SpriteManager.h"
#include "MovingObject.h"
#include "PlayerControl.h"
#include "Object.h"
#include "TimeManager.h"
#include <stdlib.h>
#include <memory.h>
#include <math.h>
#include <vector>

#include <fstream>

using namespace draw_manager;

data_time data_t;
int dt_count = 1;


std::vector<std::unique_ptr<object>> object_pull;


// initialize game data in this function
void initialize()
{
    init_font();

    data_t = { 0, 0 };

    object_pull.push_back(
        std::unique_ptr<object>(
            new player_object(data_t, tfm::transform(tfm::point(SCREEN_WIDTH/2, SCREEN_HEIGHT/2)))
        )
    );
    
    object_pull.push_back(
        std::unique_ptr<object>(new moving_object(data_t,
            tfm::transform(tfm::point(200, 300), 0.),
            tfm::transform(tfm::point(50, 0), 1.)))
    );

    moving_object(data_t,
        tfm::transform(tfm::point(100, 300), 0.),
        tfm::transform(tfm::point(50, 0), 3.));
    
    object_pull.push_back(
        std::unique_ptr<object>(
            new moving_object(data_t,
            tfm::transform(tfm::point(100, 300), 0.),
            tfm::transform(tfm::point(50, 0), 3.))
            
        )
    );
    object_pull.push_back(
        std::unique_ptr<object>(
            new object(data_t,
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
    dt_count++;
    data_t.time += dt;
    data_t.dt = dt;

    if (is_key_pressed(VK_ESCAPE))
        schedule_quit_game();

    for (size_t i = 0; i < object_pull.size(); i++)
    {
        object_pull[i]->act(data_t);
    }
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
        draw_circle_segment(buffer, 400, 375, 10 + i, i + data_t.time, i + M_PI + data_t.time, color(i, 255-i, 255, 255));
    
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

    draw_int(buffer, 600, 100, int(data_t.time), Colors::defoult);
}


// fill buffer in this function
// uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH] - is an array of 32-bit colors (8 bits per R, G, B)
void draw()
{
    memset(buffer, 0, SCREEN_HEIGHT * SCREEN_WIDTH * sizeof(uint32_t));
    fill_with_color(buffer, Colors::bgc);

    //draw_test_scene();

    for (size_t i = 0; i < object_pull.size(); i++)
    {
        object_pull[i]->draw(buffer, data_t);
    }

    if (is_key_pressed('W')) {
        draw_int(buffer, 100, 100, int(1. / (data_t.time / dt_count)), Colors::defoult);
    }
    
}


// free game data in this function
void finalize()
{
}

