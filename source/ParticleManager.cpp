#include "ParticleManager.h"

particle::particle(tfm::transform start, tfm::transform end,
	double time_start, double time_end, particleMeta meta,
	draw_manager::color color_start, draw_manager::color color_end)
	: start(start), end(end), time_start(time_start), time_end(time_end), meta(meta), color_start(color_start), color_end(color_end) {}


void particle::draw(uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH], data_time data_t)
{
	if (meta == particleMeta::circle) {
		double t = (data_t.time - time_start) / (time_end - time_start);
		draw_manager::pixel p = { int(start.position.x * (1 - t) + end.position.x * (t)),
					int(start.position.y * (1 - t) + end.position.y * (t)) };
		double s = start.scale * (1 - t) + end.scale * (t);
		draw_manager::draw_circle_fill(buffer, p, s, draw_manager::mesh_colors(color_start, color_end, t));
	}
	// NOTE: more draw cases may be added to accept different meta for particle
};


void particle_manager::draw(uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH], data_time data_t) {
	// clear all past particles
    auto it = particle_butch.begin();
    while (it != particle_butch.end())
    {
        if (!(it->time_end > data_t.time))
        {
            it = particle_butch.erase(it);
        }
        else {
            ++it;
        }
    }

	for (size_t i = 0; i < particle_butch.size(); i++)
	{
		particle_butch[i].draw(buffer, data_t);
	}
};

particle_manager pm;

void init_particle_manager() {
	particle_manager pm = particle_manager();
}

void draw_particles(uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH], data_time data_t) {
	pm.draw(buffer, data_t);
}

void add_particle(particle p) {
	pm.particle_butch.push_back(p);
}
