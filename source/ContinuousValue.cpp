#include "ContinuousValue.h"
#include <math.h>

void continuous_value::process(data_time data_t) {
	is_started = false;
	is_finished = false;
	if (fabs(value - continuous) > 0.001 && !is_progress) {
		is_progress = true;
		is_started = true;
	}
	else if (fabs(value - continuous) < 0.001 && is_progress) {
		is_progress = false;
		is_finished = true;
	}
	if (is_progress) {
		continuous = (1 - data_t.dt * 5) * continuous + (data_t.dt * 5) * value;
	}
	else {
		continuous = value;
	}
}