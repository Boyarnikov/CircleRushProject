#define _USE_MATH_DEFINES

#include "DrawManager.h"
#include "PlayerControl.h"
#include "TimeManager.h"
#include "ParticleManager.h"
#include "Tags.h"
#include <iostream>
#include <alg.h>

player_object::player_object(data_time data_t, tfm::transform t) : object(data_t, t, Tags::Player) {};

int player_object::get_level()
{
	return level;
}

int player_object::get_score()
{
	return score;
}

int player_object::get_hp()
{
	return hp;
}

void player_object::add_score(int s)
{
	score += s;
}

void player_object::add_hp(int h)
{
	hp += h;
}

tfm::transform player_object::get_ball_transform(int i)
{
	if (hp > 0) 
		return player_balls[i];
	return tfm::transform();
}

int random_int(int low, int high) {
	return low + rand() % (high - low + 1);
}


void player_object::act(data_time data_t) {
	object::act(data_t);

	// player input
	if (is_key_pressed(VK_SPACE)) {
		if (!direction_button_pressed) {
			is_clockwise = !is_clockwise;
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
		next_level += max(next_level * 0.35, 3);
		level_count++;
		speed += 0.2;
	}

	player_hp.value = hp;
	player_hp.process(data_t);

	if (hp == 0 && player_hp.is_started) {
		for (size_t i = 0; i < 10; i++)
		{
			for (size_t j = 0; j < 2; j++)
			{
				tfm::transform start = tfm::transform(player_balls[j]);
				tfm::transform end = tfm::transform(player_balls[j]);
				end.position.x += random_int(-300, 300);
				end.position.y += random_int(-300, 300);
				start.scale = random_int(5, 20);
				end.scale = start.scale / 2; 
				add_particle(
					particle(start, end,
						data_t.time, data_t.time + 1,
						particleMeta::circle, draw_manager::Colors::green, draw_manager::Colors::green_fade));
			}
		}
	}

	// balls position calculations
	obj_transform.scale =  (1 + sin(data_t.time) / 5);
	obj_transform.rotation += (is_clockwise ? data_t.dt : -data_t.dt) * speed;
	tfm::point delta = { obj_transform.scale * radius * cos(obj_transform.rotation + M_PI / 2.),
		obj_transform.scale * radius * sin(obj_transform.rotation + M_PI / 2.) };
	player_balls[0].scale = player_ball_size;
	player_balls[1].scale = player_ball_size;
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
	if (hp > 0) {
		//draw_manager::draw_circle_fill(buffer, player_balls[0].position.x, player_balls[0].position.y,
		//	player_ball_size * player_balls[0].scale + 10, draw_manager::Colors::dark);
		draw_manager::draw_circle_fill(buffer, player_balls[0].position.x, player_balls[0].position.y,
			player_balls[0].scale, draw_manager::Colors::green);
		//draw_manager::draw_circle_fill(buffer, player_balls[1].position.x, player_balls[1].position.y,
		//	player_ball_size * player_balls[1].scale + 10, draw_manager::Colors::dark);
		draw_manager::draw_circle_fill(buffer, player_balls[1].position.x, player_balls[1].position.y,
			player_balls[1].scale, draw_manager::Colors::green);

		if (int(data_time.time) > int(data_time.time - data_time.dt)) {
			add_particle(
				particle(player_balls[0], player_balls[0],
					data_time.time, data_time.time + 0.5,
					particleMeta::circle, draw_manager::Colors::green, draw_manager::Colors::green_fade));
			add_particle(
				particle(player_balls[1], player_balls[1],
					data_time.time, data_time.time + 0.5,
					particleMeta::circle, draw_manager::Colors::green, draw_manager::Colors::green_fade));
		
		}
	}


	// player hp
	double gt = data_time.time - 2 * M_PI * int(data_time.time / 2 / M_PI);
	if (player_hp.value > 0) {
		for (size_t i = 0; i < player_hp.value; i++)
		{
			float angle = gt + i * 2 * M_PI / player_hp.continuous;
			float r = ((player_hp.continuous - 1.0) * radius * obj_transform.scale / player_hp.continuous / 2);
			//if (health > 0) {
			//	r *= sin(global_time * 10) * 0.01 + 1;
			//}
			int vecx = int(r * cos(angle));
			int vecy = int(r * sin(angle));
			draw_manager::draw_circle_fill(buffer, vecx + obj_transform.position.x, vecy + obj_transform.position.y, 10, draw_manager::Colors::green);
		}
	}

	object::draw(buffer, data_time);
}