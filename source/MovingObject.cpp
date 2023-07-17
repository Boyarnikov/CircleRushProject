#include "MovingObject.h"
#include <iostream>

moving_object::moving_object(double time, tfm::transform t, tfm::transform dt = tfm::transform(), Tags s = Tags::None) : object(time, t, s), transform_dt(dt) {};
moving_object::moving_object(double time, tfm::transform t, tfm::transform dt = tfm::transform()) : object(time, t), transform_dt(dt) {};
moving_object::moving_object(double time, tfm::transform t, Tags s = Tags::None) : object(time, t, s), transform_dt() {};
moving_object::moving_object(double time, tfm::transform t) : object(time, t), transform_dt() {};
moving_object::moving_object(double time, Tags s = Tags::None) : object(time, s), transform_dt() {};
moving_object::moving_object(double time) : object(time), transform_dt() {};


void moving_object::act(double time, double dt) {
	object::act(time, dt);
	obj_transform.position.x += dt * transform_dt.position.x;
	obj_transform.position.y += dt * transform_dt.position.y;
	obj_transform.rotation += dt * transform_dt.rotation;
	obj_transform.scale += dt * transform_dt.scale;
}