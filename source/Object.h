#pragma once
#include "Transform.h"
#include "Engine.h"

class object
{
private:
	double creation_time;
	tfm::transform obj_transform;
public:
	object(double time);
	const tfm::transform get_transform();
	void act(double time, double dt);
	void draw(uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH], double time);
};

