#pragma once
#include <optional>

class Timed_event
{
	float time_left;
	const float base_time;
	const int target_event;
	int enable;
	int disable;
	bool enabled = false;
	void reset();

public:
	Timed_event(float time_, int target_, int enable_, int disable_);
	std::optional<int> update(float dt);
	void change_state(int event);
};
