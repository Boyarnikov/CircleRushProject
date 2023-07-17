#include "Object.h"

object::object(double time) : creation_time(time), obj_transform(tfm::transform()) {};

const tfm::transform object::get_transform() {
	return obj_transform;
}

void object::act(double time, double dt) {
	return;
}

void object::draw(uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH], double time) {
	return;
}