#include "util.h"

struct Context context;

Vectorf util::intersection(std::pair<Vectorf, Vectorf> a, std::pair<Vectorf, Vectorf> b)
{
	double a1 = a.second.y - a.first.y;
	double b1 = a.first.x - a.second.x;
	double c1 = a1 * (a.first.x) + b1 * (a.first.y);

	double a2 = b.second.y - b.first.y;
	double b2 = b.first.x - b.second.x;
	double c2 = a2 * (b.first.x) + b2 * (b.first.y);

	double determinant = a1 * b2 - a2 * b1;

	if (determinant == 0)
	{
		return Vectorf(FLT_MAX, FLT_MAX);
	}
	else
	{
		double x = (b2 * c1 - b1 * c2) / determinant;
		double y = (a1 * c2 - a2 * c1) / determinant;
		return Vectorf(x, y);
	}
}