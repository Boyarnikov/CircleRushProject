#pragma once
#include "TimeManager.h"
#include "Transform.h"
#include "Engine.h"
#include "Tags.h"

// represents game object with transform that can be called to draw and act
class object
{
protected:
	bool active;
	double creation_time;
	tfm::transform obj_transform;
	Tags tag;
	void debug_draw(uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH]);
public:
	object(data_time);
	object(data_time, tfm::transform);
	object(data_time, Tags);
	object(data_time, tfm::transform, Tags);
	bool is_active();
	const tfm::transform get_transform();
	void set_transform(tfm::transform);
	virtual void act(data_time);
	void draw(uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH], double time);
	
};

