#include "util.h"

struct Context context;

Vectorf util::saturate(Vectorf val, const Vectorf max_val)
{
	if (val.x > max_val.x)
		val.x = max_val.x;
	if (val.x < -max_val.x)
		val.x = -max_val.x;
	if (val.y > max_val.y)
		val.y = max_val.y;
	if (val.y < -max_val.y)
		val.y = -max_val.y;
	return val;
}

float util::deg_to_rad(float s)
{
	return(s / 180 * PI);
}

float util::rad_to_deg(float rdn)
{
	return rdn / PI * 180;
}

float util::ang_reduce(float ang)
{
	ang = fmod(ang, 360.0f);
	if (ang < 0)
		ang += 360.0f;
	return ang;
}

Vectorf util::rotate_vector(Vectorf vec, float ang)
{
	return { vec.x * cos(ang) -
			vec.y * sin(ang),
			vec.x * sin(ang) +
			vec.y * cos(ang) };
}

bool util::vectorf_compare(const Vectorf& a, const Vectorf& b)
{
	if (a.x != b.x)
	{
		return a.x > b.x;
	}
	else
	{
		return a.y > b.y;
	}
}

bool util::vectorf_binary_predicate(const Vectorf& a, const Vectorf& b)
{
	return fabs(a.x - b.x) < 1 && fabs(a.y - b.y) < 1;
}
