#define _USE_MATH_DEFINES

#include "Engine.h"
#include "Object.h"
#include "DrawManager.h"
#include <math.h>

object::object(double time) : creation_time(time), obj_transform(tfm::transform()), active(true) {};
object::object(double time, tfm::transform t) : creation_time(time), obj_transform(t), active(true) {};
object::object(double time, std::string s) : creation_time(time), obj_transform(tfm::transform()), active(true), tag(s) {};
object::object(double time, tfm::transform t, std::string s) : creation_time(time), obj_transform(t), active(true), tag(s) {};

bool object::is_active() {
	return active;
}

const tfm::transform object::get_transform() {
	return obj_transform;
}

void object::act(double time, double dt) {
	obj_transform.position.x += dt * 1;
	return;
}

void object::draw(uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH], double time) {
	if (is_key_pressed('W')) {
		draw_manager::pixel pos(obj_transform.position.x, obj_transform.position.y);
		double len = 10;
		draw_manager::pixel x(obj_transform.position.x + len * cos(obj_transform.rotation),
			obj_transform.position.y + len * sin(obj_transform.rotation));
		draw_manager::pixel y(obj_transform.position.x + len * cos(obj_transform.rotation - M_PI / 2),
			obj_transform.position.y + len * sin(obj_transform.rotation - M_PI / 2));
		draw_manager::draw_line(buffer, pos, x, draw_manager::Colors::blue);
		draw_manager::draw_line(buffer, pos, y, draw_manager::Colors::green);
	}
	return;
}