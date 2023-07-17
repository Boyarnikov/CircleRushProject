#include "MovingObject.h"
#include <iostream>

moving_object::moving_object(double time, tfm::transform t, tfm::transform dt, std::string s) : object(time, t, s), transform_dt(dt) {};

void moving_object::act(double time, double dt) {
	object::act(time, dt);
	obj_transform.position.x += dt * transform_dt.position.x;
	obj_transform.position.y += dt * transform_dt.position.y;
	obj_transform.rotation += dt * transform_dt.rotation;
	obj_transform.scale += dt * transform_dt.scale;
	std::cout << "gelp";
}