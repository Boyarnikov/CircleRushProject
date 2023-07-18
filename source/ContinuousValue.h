#pragma once
#include "TimeManager.h"

// .continuous continuously moving towards .value, allowing for smoth transitions in double. flags indicate different states of value folloving status
struct continuous_value
{
	double value;
	double continuous;
	bool is_started;
	bool is_progress;
	bool is_finished;

	void process(data_time);
};

