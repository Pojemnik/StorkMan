#include "util.h"

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

float util::rdn(float s)
{
	return(s / 180 * PI);
}
