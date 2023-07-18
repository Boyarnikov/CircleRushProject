#include "TargetCubes.h"
#include "DrawManager.h"

using namespace draw_manager;

void target_cube::draw(uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH], data_time data_time) {
	pixel block[4];

	for (int k = -4; k < 10; k++) {
		int vecx = int((block_size + k) * cos(obj_transform.rotation));
		int vecy = int((block_size + k) * sin(obj_transform.rotation));

		for (size_t j = 0; j < 4; j++)
		{
			block[j] = { int(obj_transform.position.x + vecx), int(obj_transform.position.y + vecy) };
			int swap = vecx;
			vecx = -vecy;
			vecy = swap;
		}

		draw_polygon(buffer, block, 4, Colors::white);
	}

	object::draw(buffer, data_time);
}

target_cube::target_cube(data_time time, tfm::transform t, tfm::transform dt = tfm::transform()) : moving_object(time, t, dt, Tags::Target) {};

