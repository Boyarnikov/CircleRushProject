#include "MovingObject.h"
#include "TimeManager.h"
#include <iostream>


moving_object::moving_object(data_time time, tfm::transform t, tfm::transform dt = tfm::transform(), Tags s = Tags::None) : object(time, t, s), transform_dt(dt) {};
moving_object::moving_object(data_time time, tfm::transform t, tfm::transform dt = tfm::transform()) : object(time, t), transform_dt(dt) {};
moving_object::moving_object(data_time time, tfm::transform t, Tags s = Tags::None) : object(time, t, s), transform_dt() {};
moving_object::moving_object(data_time time, tfm::transform t) : object(time, t), transform_dt() {};
moving_object::moving_object(data_time time, Tags s = Tags::None) : object(time, s), transform_dt() {};
moving_object::moving_object(data_time time) : object(time), transform_dt() {};


void moving_object::act(data_time data_time) {
	object::act(data_time);
	obj_transform.position.x += data_time.dt * transform_dt.position.x;
	obj_transform.position.y += data_time.dt * transform_dt.position.y;
	obj_transform.rotation += data_time.dt * transform_dt.rotation;
	obj_transform.scale += data_time.dt * transform_dt.scale;
	if (obj_transform.position.x < -SCREEN_WIDTH || obj_transform.position.x > 2 * SCREEN_WIDTH ||
		obj_transform.position.y < -SCREEN_HEIGHT || obj_transform.position.y > 2 * SCREEN_HEIGHT) {
		active = false;
	}
}


const tfm::transform moving_object::get_transform_dt() {
	return transform_dt;
}

void moving_object::set_transform_dt(tfm::transform t) {
	transform_dt = t;
}