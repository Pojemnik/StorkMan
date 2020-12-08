#include "polygon_generator.h"

std::pair<float, Vectorf> Polygon_generator::cast_ray(Vectorf source, Vectorf alfa,
	std::vector<std::pair<Vectorf, Vectorf>>& map_edges)
{
	if (fabs(alfa.x) < eps)
	{
		return std::pair<float, Vectorf>(util::convert_vector(alfa), source);
	}
	Vectorf beta;
	float min_t1 = INFINITY;
	Vectorf point = source;
	for (auto& it : map_edges)
	{
		beta = { it.first.x - it.second.x, it.first.y - it.second.y };
		float t2 = ((alfa.x * (it.second.y - source.y) +
			alfa.y * (source.x - it.second.x))
			/ (beta.x * alfa.y - beta.y * alfa.x));
		if (t2 >= 0 && t2 <= 1)
		{
			float t1 = (it.second.x + beta.x * t2 - source.x) / alfa.x;
			if (t1 > 0)
			{
				if (t1 < min_t1)
				{
					min_t1 = t1;
					point = { t1 * alfa.x + source.x, t1 * alfa.y + source.y };
				}
			}
		}
	}
	return std::pair<float, Vectorf>(util::convert_vector(alfa), point);
}

std::vector<Vectorf> Polygon_generator::calc_polygon(Vectorf source,
	float max_size,
	std::vector<std::pair<Vectorf, Vectorf>>& map_edges, std::vector<Vectorf>& map_vertices)
{
	static const float COS(cos(0.0001f));
	static const float SIN(sin(0.0001f));
	std::vector<std::pair<float, Vectorf>> points;
	int added_vertices = add_bounds(source, max_size, map_edges, map_vertices);
	for (const auto& vertex_it : map_vertices)
	{
		const Vectorf alfa = vertex_it - source;
		if (alfa == Vectorf(0, 0))
			continue;
		if (util::sq(alfa.x) + util::sq(alfa.y) <
			2.f * util::sq(max_size) + 2)
		{
			std::pair<float, Vectorf> point;
			point = cast_ray(source, alfa, map_edges);
			if (point.second != source)
				points.push_back(point);
			point = cast_ray(source, { alfa.x * COS - alfa.y * SIN,
				alfa.x * SIN + alfa.y * COS }, map_edges);
			if (point.second != source)
				points.push_back(point);
			point = cast_ray(source, { alfa.x * COS + alfa.y * SIN,
				alfa.y * COS - alfa.x * SIN }, map_edges);
			if (point.second != source)
				points.push_back(point);
		}
	}
	remove_bounds(added_vertices, map_edges, map_vertices);
	std::sort(points.begin(), points.end(),
		[](const std::pair<float, Vectorf>& a, const std::pair<float, Vectorf>& b)
	{return a.first > b.first; });
	std::vector<Vectorf> polygon;
	for (const auto& it : points)
	{
		polygon.push_back(it.second);
	}
	return simplify(std::move(polygon));
}

int Polygon_generator::add_bounds(Vectorf source, float max_size, std::vector<std::pair<Vectorf,
	Vectorf>>&map_edges, std::vector<Vectorf>& map_vertices)
{
	int added = 4;
	const float edge = max_size;
	map_edges.push_back(std::make_pair(
		source + Vectorf(-edge, edge), source + Vectorf(edge, edge)));
	map_edges.push_back(std::make_pair(
		source + Vectorf(edge, edge), source + Vectorf(edge, -edge)));
	map_edges.push_back(std::make_pair(
		source + Vectorf(edge, -edge), source + Vectorf(-edge, -edge)));
	map_edges.push_back(std::make_pair(
		source + Vectorf(-edge, -edge), source + Vectorf(-edge, edge)));
	for (int j = 0; j < 4; j++)
	{
		for (int i = 0; i < map_edges.size() - 4; i++)
		{
			Vectorf inter =
				util::intersection_point(map_edges[i], map_edges[map_edges.size() - 1 - j]);
			if (inter != Vectorf(FLT_MAX, FLT_MAX))
			{
				map_vertices.push_back(inter);
				added++;
			}
		}
	}
	map_vertices.push_back(source + Vectorf(edge, edge));
	map_vertices.push_back(source + Vectorf(-edge, edge));
	map_vertices.push_back(source + Vectorf(-edge, -edge));
	map_vertices.push_back(source + Vectorf(edge, -edge));
	return added;
}

void Polygon_generator::remove_bounds(int number, std::vector<std::pair<Vectorf, Vectorf>>& map_edges, std::vector<Vectorf>& map_vertices)
{
	map_edges.pop_back();
	map_edges.pop_back();
	map_edges.pop_back();
	map_edges.pop_back();
	while (number--)
	{
		map_vertices.pop_back();
	}
}

std::vector<Vectorf> Polygon_generator::simplify(std::vector<Vectorf>&& polygon)
{
	std::vector<bool> to_remove(polygon.size(), false);
	int i = 0;
	int j = 1;
	while (j + 1 < polygon.size())
	{
		if (util::are_colinear(polygon[i], polygon[j], polygon[j + 1], 0.1f))
		{
			to_remove[j] = true;
			j++;
		}
		else
		{
			do
			{
				i++;
			} while (i < polygon.size() && to_remove[i] == true);
			j = j + 1;
		}
	}
	polygon.erase(std::remove_if(polygon.begin(), polygon.end(), [to_remove]
	(Vectorf element)
	{
		static int i(0);
		return to_remove[i++];
	}), polygon.end());
	return polygon;
}
