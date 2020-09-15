#include "util.h"

struct Context context;


std::string util::pass_or_default(std::string val, std::string default_val)
{
	return val == "" ? default_val : val;
}

sf::Vector2f util::normalize(sf::Vector2f x, float l)
{
	return x / float(sqrt(x.x * x.x + x.y * x.y) * l);
}

sf::Vector2f util::get_axis_normal(const std::vector<sf::Vector2f>* a,
	size_t i)
{
	Vectorf p1 = (*a)[i];
	Vectorf p2 = (i >= a->size() - 1) ? (*a)[0] : (*a)[i + 1];
	return util::normalize({ p1.y - p2.y,p2.x - p1.x }, 1);
}

float util::vector_dot_product(sf::Vector2f a, sf::Vector2f b)
{
	return a.x * b.x + a.y * b.y;
}

float util::vector_cross_product(sf::Vector2f a, sf::Vector2f b)
{
	return a.x * b.y - a.y * b.x;
}

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
	float COS = cos(ang);
	float SIN = sin(ang);
	return { vec.x * COS -
			vec.y * SIN,
			vec.x * SIN +
			vec.y * COS };
}
Vectorf util::rotate_vector(Vectorf vec, Vectorf normalized_rotation_vector)
{
	return { vec.x * normalized_rotation_vector.x -
			vec.y * normalized_rotation_vector.y,
			vec.x * normalized_rotation_vector.y +
			vec.y * normalized_rotation_vector.x };
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

bool util::vectorf_binary_predicate(const Vectorf& a,
	const Vectorf& b)
{
	return fabs(a.x - b.x) < 1 && fabs(a.y - b.y) < 1;
}

float util::convert_vector(const Vectorf& vec)
{
	float SIN = vec.y / hypot(vec.x, vec.y);
	if (vec.x > 0)
		return SIN - 1;
	else
		return 1 - SIN;
}

bool util::round_and_compare(Vectorf a, Vectorf b)
{
	a = { float(round(a.x)), float(round(a.y)) };
	b = { float(round(b.x)), float(round(b.y)) };
	return a == b;
}

Vectorf util::intersection_point(std::pair<Vectorf, Vectorf> a, std::pair<Vectorf, Vectorf> b)
{
	float a1 = a.second.y - a.first.y;
	float b1 = a.first.x - a.second.x;
	float c1 = a1 * (a.first.x) + b1 * (a.first.y);

	float a2 = b.second.y - b.first.y;
	float b2 = b.first.x - b.second.x;
	float c2 = a2 * (b.first.x) + b2 * (b.first.y);

	float determinant = a1 * b2 - a2 * b1;

	if (determinant == 0)
	{
		return Vectorf(FLT_MAX, FLT_MAX);
	}
	else
	{
		float x = (b2 * c1 - b1 * c2) / determinant;
		float y = (a1 * c2 - a2 * c1) / determinant;
		if (x >= std::min(a.first.x, a.second.x) && x <= std::max(a.first.x, a.second.x) &&
			x >= std::min(b.first.x, b.second.x) && x <= std::max(b.first.x, b.second.x) &&
			y >= std::min(a.first.y, a.second.y) && y <= std::max(a.first.y, a.second.y) &&
			y >= std::min(b.first.y, b.second.y) && y <= std::max(b.first.y, b.second.y))
		{
			return Vectorf(x, y);
		}
		else
		{
			return Vectorf(FLT_MAX, FLT_MAX);
		}
	}
}

bool util::intersection(std::pair<Vectorf, Vectorf> a, std::pair<Vectorf, Vectorf> b)
{
	Vectorf tmp = intersection_point(a, b);
	if (tmp.x == FLT_MAX && tmp.y == FLT_MAX)
	{
		return false;
	}
	return true;
}

void util::save_texture(std::string path, sf::Texture* texture)
{
	sf::Image im = texture->copyToImage();
	im.saveToFile(path);
}

sf::FloatRect util::mesh_to_rect(std::vector<sf::Vertex> vertices)
{
	float maxx, maxy, miny, minx;
	maxx = minx = vertices[0].position.x;
	maxy = miny = vertices[0].position.y;
	for (auto it : vertices)
	{
		if (it.position.x < minx)
			minx = it.position.x;
		if (it.position.y < miny)
			miny = it.position.y;
		if (it.position.x > maxx)
			maxx = it.position.x;
		if (it.position.y > maxy)
			maxy = it.position.y;
	}
	return sf::FloatRect(minx, miny, maxx - minx, maxy - miny);
}

bool util::contained_in_polygon(Vectorf point, float max_x, const std::vector<Vectorf>& polygon)
{
	Vectorf outside_p = { max_x + 1, point.y };
	auto section = std::make_pair(point, outside_p);
	int n = 0;
	for (int i = 0; i < polygon.size() - 1; i++)
	{
		if (util::intersection(
			std::make_pair(polygon[i], polygon[i + 1]), section))
		{
			n++;
		}
	}
	if (util::intersection(
		std::make_pair(polygon[0], polygon[polygon.size() - 1]), section))
		n++;
	if (n % 2)
		return true;
	return false;
}