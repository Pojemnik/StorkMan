#include "timed_events.h"

Timed_event::Timed_event(float time_, int target_, int enable_, int disable_)
	: time_left(time_), base_time(time_), target_event(target_), enable(enable_),
	disable(disable_) {}

std::optional<int> Timed_event::update(float dt)
{
	if (enabled)
	{
		time_left -= dt;
		if (time_left <= 0)
		{
			enabled = false;
			reset();
			return std::make_optional(target_event);
		}
	}
	return std::optional<int>();
}

void Timed_event::change_state(int event)
{
	if (enable == event)
	{
		enabled = true;
	}
	else if (disable == event)
	{
		enabled = false;
		reset();
	}
}

void Timed_event::reset()
{
	time_left = base_time;
}
