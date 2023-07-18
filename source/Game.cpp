#define _USE_MATH_DEFINES

#include "Engine.h"
#include "DrawManager.h"
#include "SpriteManager.h"
#include "MovingObject.h"
#include "PlayerControl.h"
#include "Object.h"
#include "TimeManager.h"
#include "EnemyCubes.h"
#include "TargetCubes.h"
#include "HealthBonus.h"
#include "ParticleManager.h"
#include <stdlib.h>
#include <alg.h>
#include <memory.h>
#include <math.h>
#include <vector>

#include <fstream>

using namespace draw_manager;

data_time data_t;
int dt_count = 1;

player_object* player;

std::vector<std::unique_ptr<object>> object_pull;


int random(int low, int high) {
    return low + rand() % (high - low + 1);
}


double distance_sqr(double x1, double  y1, double  x2, double  y2) {
    return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
}


double distance_sqr(tfm::point a, tfm::point b) {
    return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

// structure that generates different items 
struct item_generator {
    double last_enemy_generated;
    double timed_enemy = 3.3;

    double last_target_generated;
    double timed_target = 7;

    double last_health_generated;
    double timed_health = 60;


    std::pair<tfm::transform, tfm::transform> generate_movement(double speed) {
        std::pair<tfm::transform, tfm::transform> movement;

        double direction = double(random(0, 360)) / 2 / M_PI;
        double distance_from_center = 700;

        movement.first.position = {
            double(SCREEN_WIDTH / 2 + random(-100., 100.) + cos(direction) * distance_from_center),
            double(SCREEN_HEIGHT / 2 + random(-100., 100.) + sin(direction) * distance_from_center)
        };
        movement.first.scale = 1;
        movement.second.position = { -speed * cos(direction), -speed * sin(direction) };
        movement.second.rotation = double(random(5, 10)) / 5;
        return movement;
    }

    
    void generate_enemy_cube(data_time data_t) {
        std::pair<tfm::transform, tfm::transform> movement = generate_movement(10. * player->get_level() + random(100, 200));
        object_pull.push_back(
            std::unique_ptr<object>(new enemy_cube(data_t, movement.first, movement.second))
        );
    }


    void generate_target_cube(data_time data_t) {
        std::pair<tfm::transform, tfm::transform> movement = generate_movement(10. * player->get_level() + random(100, 200));
        object_pull.push_back(
            std::unique_ptr<object>(new target_cube(data_t, movement.first, movement.second))
        );
    }


    void generate_health(data_time data_t) {
        std::pair<tfm::transform, tfm::transform> movement = generate_movement(10. * player->get_level() + random(100, 200));
        object_pull.push_back(
            std::unique_ptr<object>(new health_bonus(data_t, movement.first, movement.second))
        );
    }


    void act(data_time) {
        if (data_t.time - last_enemy_generated > timed_enemy - min(timed_enemy/3, player->get_level() * 0.3)) {
            generate_enemy_cube(data_t);
            last_enemy_generated = data_t.time;
        }

        if (data_t.time - last_target_generated > timed_target - min(timed_target / 3, player->get_level() * 0.3)) {
            generate_target_cube(data_t);
            last_target_generated = data_t.time;
        }

        if (data_t.time - last_health_generated > timed_health) {
            generate_health(data_t);
            last_health_generated = data_t.time;
        }
    }
};

item_generator ig;

// initialize game data in this function
void initialize()
{
    init_font();
    init_particle_manager();

    data_t = { 0, 0 };

    object_pull.push_back(
        std::unique_ptr<object>(
            new player_object(data_t, tfm::transform(tfm::point(SCREEN_WIDTH/2, SCREEN_HEIGHT/2)))
        )
    );

    player = (player_object*)object_pull[0].get();
}


void clear_unactive() {
    auto it = object_pull.begin();
    while (it != object_pull.end())
    {
        if (!(it->get()->active))
        {
            it = object_pull.erase(it);
        }
        else {
            ++it;
        }
    }
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

    for (size_t j = 0; j < 2; j++)
    for (size_t i = 0; i < object_pull.size(); i++)
    {
        if (distance_sqr(object_pull[i]->get_transform().position, player->get_ball_transform(j).position) < 1000) {
            if (object_pull[i]->tag == Tags::Enemy) {
                player->add_hp(-1);
            }
            if (object_pull[i]->tag == Tags::Bonus) {
                player->add_hp(1);
            }
            if (object_pull[i]->tag == Tags::Target) {
                player->add_score(1);
            }
            object_pull[i]->active = false;
        }
    }

    ig.act(data_t);

    clear_unactive();
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
        draw_int(buffer, 100, 200, int(object_pull.size()), Colors::defoult);

        draw_circle(buffer, int(player->get_ball_transform(0).position.x),
            int(player->get_ball_transform(0).position.y), 30, Colors::defoult);
        draw_circle(buffer, int(player->get_ball_transform(1).position.x),
            int(player->get_ball_transform(1).position.y), 30, Colors::defoult);
    }

    draw_particles(buffer, data_t);


}


// free game data in this function
void finalize()
{
}

