#pragma once

#include "Transform.h"
#include "DrawManager.h"
#include "TimeManager.h"
#include "Engine.h"
#include <vector>


enum class particleMeta {
	none = -1,
	circle = 0,
	squere = 1
};

// structure that containts all particle info
struct particle {
	tfm::transform start = tfm::transform();
	tfm::transform end = tfm::transform();
	double time_start;
	double time_end;
	particleMeta meta = particleMeta::none;
	draw_manager::color color_start;
	draw_manager::color color_end;

	particle(tfm::transform start, tfm::transform end,
		double time_start, double time_end, particleMeta meta,
		draw_manager::color color_start, draw_manager::color color_end);

	void draw(uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH], data_time);
};

class particle_manager
{
public:
	std::vector<particle> particle_butch;

	void draw(uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH], data_time);
};

void draw_particles(uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH], data_time data_t);

void add_particle(particle p);

void init_particle_manager();



