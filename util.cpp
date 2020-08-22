#include "util.h"

struct Context context;

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