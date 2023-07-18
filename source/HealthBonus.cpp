#include "HealthBonus.h"
#include "DrawManager.h"

using namespace draw_manager;

void health_bonus::draw(uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH], data_time data_time) {
	pixel block[4];

	draw_circle_fill(buffer, obj_transform.position.x, obj_transform.position.y, 10, Colors::green);

	object::draw(buffer, data_time);
}

health_bonus::health_bonus(data_time time, tfm::transform t, tfm::transform dt = tfm::transform()) : moving_object(time, t, dt, Tags::Bonus) {};

