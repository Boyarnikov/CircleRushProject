#define _USE_MATH_DEFINES

#include "DrawManager.h"
#include "PlayerControl.h"
#include "TimeManager.h"
#include "Tags.h"
#include <iostream>
#include <alg.h>


player_object::player_object(data_time data_t, tfm::transform t) : object(data_t, t, Tags::Player) {};

void player_object::act(data_time data_t) {
	object::act(data_t);

	// player input
	if (is_key_pressed(VK_SPACE)) {
		if (!direction_button_pressed) {
			is_clockwise = !is_clockwise;
			score++;
		}
		direction_button_pressed = true;
	}
	if (!is_key_pressed(VK_SPACE)) {
		direction_button_pressed = false;
	}

	// level update logic
	level_fullnes.value = double(score - level) / double(next_level - level);
	level_fullnes.process(data_t);
	if (score >= next_level && (level_fullnes.is_finished || score > next_level)) {
		level = next_level;
		next_level += max(next_level * 0.5, 3);
		level_count++;
		speed += 0.1;
	}

	// balls position calculations
	obj_transform.scale =  (1 + sin(data_t.time) / 5);
	obj_transform.rotation += (is_clockwise ? data_t.dt : -data_t.dt) * speed;
	tfm::point delta = { obj_transform.scale * radius * cos(obj_transform.rotation + M_PI / 2.),
		obj_transform.scale * radius * sin(obj_transform.rotation + M_PI / 2.) };
	player_balls[0].position = { obj_transform.position.x + delta.x, obj_transform.position.y + delta.y };
	player_balls[1].position = { obj_transform.position.x - delta.x, obj_transform.position.y - delta.y };
}


void player_object::draw(uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH], data_time data_time) {
	
	// big tragectory circle
	draw_manager::draw_circle_fill(buffer, obj_transform.position.x, obj_transform.position.y,
		radius * obj_transform.scale + 7, draw_manager::Colors::dark);
	draw_manager::draw_circle_fill(buffer, obj_transform.position.x, obj_transform.position.y,
		radius * obj_transform.scale - 7, draw_manager::Colors::bgc);

	// draw score
	for (int i = -2; i <= 3; i++)
	{
		draw_manager::draw_circle_segment(buffer, obj_transform.position.x, obj_transform.position.y,
			radius * obj_transform.scale + i, 0 - 0.5 * M_PI, level_fullnes.continuous * 2 * M_PI - 0.5 * M_PI, draw_manager::Colors::white);
	}
	draw_manager::draw_int(buffer, obj_transform.position.x + 5,
		obj_transform.position.y - radius * obj_transform.scale - 32 + 5, level_count, draw_manager::Colors::dark);
	draw_manager::draw_int(buffer, obj_transform.position.x,
		obj_transform.position.y - radius * obj_transform.scale - 32, level_count, draw_manager::Colors::white);


	// player balls
	draw_manager::draw_circle_fill(buffer, player_balls[0].position.x, player_balls[0].position.y,
		player_ball_size * player_balls[0].scale, draw_manager::Colors::green);
	draw_manager::draw_circle_fill(buffer, player_balls[1].position.x, player_balls[1].position.y,
		player_ball_size * player_balls[1].scale, draw_manager::Colors::green);

	object::draw(buffer, data_time);
}