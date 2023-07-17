#define _USE_MATH_DEFINES

#include "DrawManager.h"
#include "PlayerControl.h"
#include "TimeManager.h"
#include "Tags.h"
#include <iostream>


player_object::player_object(data_time data_t, tfm::transform t) : object(data_t, t, Tags::Player) {};

void player_object::act(data_time data_t) {
	object::act(data_t);
	obj_transform.scale =  (1 + sin(data_t.time) / 5);
	obj_transform.rotation += (is_clockwise ? data_t.dt : -data_t.dt) * speed;
	tfm::point delta = { obj_transform.scale * radius * cos(obj_transform.rotation + M_PI / 2.),
		obj_transform.scale * radius * sin(obj_transform.rotation + M_PI / 2.) };
	player_balls[0].position = { obj_transform.position.x + delta.x, obj_transform.position.y + delta.y };
	player_balls[1].position = { obj_transform.position.x - delta.x, obj_transform.position.y - delta.y };
}


void player_object::draw(uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH], data_time data_time) {
	object::act(data_time);

	draw_manager::draw_circle_fill(buffer, obj_transform.position.x, obj_transform.position.y,
		radius * obj_transform.scale + 7, draw_manager::Colors::dark);
	draw_manager::draw_circle_fill(buffer, obj_transform.position.x, obj_transform.position.y,
		radius * obj_transform.scale - 7, draw_manager::Colors::dark);

	draw_manager::draw_circle_fill(buffer, player_balls[0].position.x, player_balls[0].position.y,
		player_ball_size * player_balls[0].scale, draw_manager::Colors::green);
	draw_manager::draw_circle_fill(buffer, player_balls[1].position.x, player_balls[1].position.y,
		player_ball_size * player_balls[1].scale, draw_manager::Colors::green);
}