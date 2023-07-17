#pragma once
#include "Transform.h"
#include "Engine.h"
#include "Tags.h"

class object
{
protected:
	bool active;
	double creation_time;
	tfm::transform obj_transform;
	Tags tag;
public:
	object(double time);
	object(double time, tfm::transform);
	object(double time, Tags);
	object(double time, tfm::transform, Tags);
	bool is_active();
	const tfm::transform get_transform();
	virtual void act(double time, double dt);
	void draw(uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH], double time);
	void set_transform(tfm::transform);
};

