#include "light.h"

Light_source::Light_source(Vectorf p, const sf::Texture* t, sf::Color c, float i)
	: intensity(i), texture(t), pos(p), color(c)
{

}

Light::Light(Vectorf level_size, sf::Texture* light_tex)
	: light_texture(light_tex)
{
	states.blendMode = sf::BlendAdd;
	states.texture = light_texture;
	create_lightmap(level_size);
}



void Light::rescale(Vectorf level_size)
{
	delete target;
	create_lightmap(level_size);
}

void Light::create_lightmap(Vectorf& level_size)
{
	target = new sf::RenderTexture();
	if (!(target->create((unsigned int)level_size.x * 2,
		(unsigned int)level_size.y * 2)))
	{
		std::cerr << "Error creating lightmap" << std::endl;
	}
}

sf::VertexArray* Light::calc_light_source_parallel(int id, Light_source& source,
	std::vector<std::pair<Vectorf, Vectorf>>& map_edges,
	std::vector<Vectorf>& map_vertices)
{
	std::vector<std::pair<float, Vectorf>> points =
		calc_light_source(source, map_edges, map_vertices);
	sf::VertexArray* target = new sf::VertexArray(sf::TriangleFan, points.size() + 2);
	(*target)[0].position = source.pos;
	(*target)[0].texCoords = { 500,500 };
	(*target)[0].color = source.color;
	for (size_t i = 1; i < points.size() + 1; i++)
	{
		(*target)[i].position = points[i - 1].second;
		(*target)[i].texCoords = Vectorf(500, 500) +
			(points[i - 1].second - source.pos) / source.intensity;
		(*target)[i].color = source.color;
	}
	(*target)[points.size() + 1].position = points[0].second;
	(*target)[points.size() + 1].texCoords = Vectorf(500, 500) +
		(points[0].second - source.pos) / source.intensity;
	(*target)[points.size() + 1].color = source.color;
	return target;
}

void Light::calc_light(std::vector<Light_source>& sources,
	sf::Transform transform, std::vector<std::pair<Vectorf,
	Vectorf>>&map_edges, std::vector<Vectorf>& map_vertices)
{
	target->clear(
		sf::Color(context.darkness, context.darkness, context.darkness, 255));
	std::vector<std::future<sf::VertexArray*>> f_arr(sources.size());
	for (int i = 0; i < sources.size(); i++)
	{
		f_arr[i] = context.thread_pool->push(Light::calc_light_source_parallel,
			sources[i],	map_edges, map_vertices);
	}
	for (int i = 0; i < sources.size(); i++)
	{
		f_arr[i].wait();
		sf::VertexArray* va = f_arr[i].get();
		states.transform = transform;
		target->draw(*va, states);
		delete va;
	}
	target->display();
	lightmap.setTexture(target->getTexture());
}

std::pair<float, Vectorf> Light::cast_ray(Vectorf source, Vectorf alfa,
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

std::vector<std::pair<float, Vectorf>> Light::calc_light_source(
	Light_source source, std::vector<std::pair<Vectorf,
	Vectorf>>&map_edges, std::vector<Vectorf>& map_vertices)
{
	static const float COS(cos(0.0001f));
	static const float SIN(sin(0.0001f));
	std::vector<std::pair<float, Vectorf>> points;
	int added_vertices = add_light_edges(source, map_edges, map_vertices);
	for (const auto& vertex_it : map_vertices)
	{
		const Vectorf alfa = vertex_it - source.pos;
		if (alfa == Vectorf(0, 0))
			continue;
		if (util::sq(alfa.x) + util::sq(alfa.y) <
			light_const * util::sq(500 * source.intensity) + 50)
		{
			std::pair<float, Vectorf> point;
			point = cast_ray(source.pos, alfa, map_edges);
			if (point.second != source.pos)
				points.push_back(point);
			point = cast_ray(source.pos, { alfa.x * COS - alfa.y * SIN,
				alfa.x * SIN + alfa.y * COS }, map_edges);
			if (point.second != source.pos)
				points.push_back(point);
			point = cast_ray(source.pos, { alfa.x * COS + alfa.y * SIN,
				alfa.y * COS - alfa.x * SIN }, map_edges);
			if (point.second != source.pos)
				points.push_back(point);
		}
	}
	remove_light_edges(added_vertices, map_edges, map_vertices);
	std::sort(points.begin(), points.end(),
		[](const std::pair<float, Vectorf>& a, const std::pair<float, Vectorf>& b)
	{return a.first > b.first; });
	return points;
}

int Light::add_light_edges(Light_source& source, std::vector<std::pair<Vectorf,
	Vectorf>>&map_edges, std::vector<Vectorf>& map_vertices)
{
	int added = 4;
	const float edge = 500 * source.intensity;
	map_edges.push_back(std::make_pair(
		source.pos + Vectorf(-edge, edge), source.pos + Vectorf(edge, edge)));
	map_edges.push_back(std::make_pair(
		source.pos + Vectorf(edge, edge), source.pos + Vectorf(edge, -edge)));
	map_edges.push_back(std::make_pair(
		source.pos + Vectorf(edge, -edge), source.pos + Vectorf(-edge, -edge)));
	map_edges.push_back(std::make_pair(
		source.pos + Vectorf(-edge, -edge), source.pos + Vectorf(-edge, edge)));
	for (int j = 0; j < 4; j++)
	{
		for (int i = 0; i < map_edges.size() - 4; i++)
		{
			Vectorf inter =
				util::intersection(map_edges[i], map_edges[map_edges.size() - 1 - j]);
			if (inter != Vectorf(FLT_MAX, FLT_MAX))
			{
				map_vertices.push_back(inter);
				added++;
			}
		}
	}
	map_vertices.push_back(source.pos + Vectorf(edge, edge));
	map_vertices.push_back(source.pos + Vectorf(-edge, edge));
	map_vertices.push_back(source.pos + Vectorf(-edge, -edge));
	map_vertices.push_back(source.pos + Vectorf(edge, -edge));
	return added;
}

void Light::remove_light_edges(int number, std::vector<std::pair<Vectorf, Vectorf>>& map_edges, std::vector<Vectorf>& map_vertices)
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