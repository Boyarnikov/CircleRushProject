#pragma once
#include "TimeManager.h"

struct continuous_value
{
	double value;
	double continuous;
	bool is_started;
	bool is_progress;
	bool is_finished;

	void process(data_time);
};

