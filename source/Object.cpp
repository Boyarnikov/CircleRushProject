#define _USE_MATH_DEFINES

#include "Engine.h"
#include "Object.h"
#include "DrawManager.h"
#include "TimeManager.h"
#include <math.h>

object::object(data_time time) : creation_time(time.time), obj_transform(tfm::transform()), active(true) {};
object::object(data_time time, tfm::transform t) : creation_time(time.time), obj_transform(t), active(true) {};
object::object(data_time time, Tags s) : creation_time(time.time), obj_transform(tfm::transform()), active(true), tag(s) {};
object::object(data_time time, tfm::transform t, Tags s) : creation_time(time.time), obj_transform(t), active(true), tag(s) {};

bool object::is_active() {
	return active;
}

const tfm::transform object::get_transform() {
	return obj_transform;
}

void object::set_transform(tfm::transform t) {
	obj_transform = t;
}

void object::act(data_time data_time) {
	return;
}

void object::debug_draw(uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH]) {
	if (is_key_pressed('W')) {
		draw_manager::pixel pos(obj_transform.position.x, obj_transform.position.y);
		double len = 50;
		draw_manager::pixel x(obj_transform.position.x + len * cos(obj_transform.rotation),
			obj_transform.position.y + len * sin(obj_transform.rotation));
		draw_manager::pixel y(obj_transform.position.x + len * cos(obj_transform.rotation - M_PI / 2),
			obj_transform.position.y + len * sin(obj_transform.rotation - M_PI / 2));
		draw_manager::draw_line(buffer, pos, x, draw_manager::Colors::blue);
		draw_manager::draw_line(buffer, pos, y, draw_manager::Colors::green);
	}
}

void object::draw(uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH], data_time data_time) {
	debug_draw(buffer);
	return;
}