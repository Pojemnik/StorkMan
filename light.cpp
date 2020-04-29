#include "light.h"

Light_source::Light_source(Vectorf p, const sf::Texture* t, sf::Color c, float i)
	: intensity(i), texture(t), pos(p), color(c)
{

}

Light::Light(Vectorf level_size, sf::Texture* light_tex) : light_texture(light_tex)
{
	target = new sf::RenderTexture();
	states.blendMode = sf::BlendAdd;
	states.texture = light_texture;
	if (!(target->create(level_size.x * 2, level_size.y * 2)))
	{
		std::cerr << "Error creating lightmap" << std::endl;
	}
}

void Light::calc_light(std::vector<Light_source>& sources,
	sf::Transform transform, std::vector<std::pair<Vectorf,
	Vectorf>>& map_edges, std::vector<Vectorf>& map_vertices)
{
	target->clear(sf::Color(70, 70, 70, 255));
	for (Light_source source : sources)
	{
		std::vector<std::pair<float, Vectorf>> points = calc_light_source(source,
			map_edges, map_vertices);
		sf::VertexArray light(sf::TriangleFan, points.size() + 2);
		light[0].position = source.pos;
		light[0].texCoords = { 500,500 };
		for (size_t i = 1; i < points.size() + 1; i++)
		{
			light[i].position = points[i - 1].second;
			light[i].texCoords = points[i - 1].second + Vectorf(500, 500)
				- source.pos;
		}
		light[points.size() + 1].position = points[0].second;
		light[points.size() + 1].texCoords = points[0].second + Vectorf(500, 500)
			- source.pos;
		states.transform = transform;
		target->draw(light, states);
		sf::RenderStates source_states;
		source_states.transform = transform;
		sf::CircleShape source_point(5);
		source_point.setFillColor(sf::Color::Red);
		source_point.setPosition(source.pos-Vectorf(5,5));
		target->draw(source_point, source_states);
	}
	target->display();
	lightmap_texture = target->getTexture();
	lightmap.setTexture(lightmap_texture);
}

std::pair<float, Vectorf> Light::cast_ray(Vectorf source, Vectorf alfa,
	std::vector<std::pair<Vectorf, Vectorf>>& map_edges)
{
	Vectorf beta;
	float min_t1 = INFINITY;
	Vectorf point = source;
	for (auto& it : map_edges)
	{
		beta = { it.first.x - it.second.x, it.first.y - it.second.y };
		float t2 = ((alfa.x * (it.second.y - source.y) +
			alfa.y * (source.x - it.second.x))
			/ (beta.x * alfa.y - beta.y * alfa.x));
		if (t2 >= 0 && t2 <= 1 && alfa.x != 0)
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
	return std::pair<float, Vectorf>(atan2(-alfa.y, alfa.x), point);
}

std::vector<std::pair<float, Vectorf>> Light::calc_light_source(
	Light_source source, std::vector<std::pair<Vectorf,
	Vectorf>>& map_edges, std::vector<Vectorf>& map_vertices)
{
	std::vector<std::pair<float, Vectorf>> points;
	Vectorf point;
	Vectorf alfa;
	map_edges.push_back(std::make_pair(
		source.pos + Vectorf(-500, 500), source.pos + Vectorf(500, 500)));
	map_edges.push_back(std::make_pair(
		source.pos + Vectorf(500, 500), source.pos + Vectorf(500, -500)));
	map_edges.push_back(std::make_pair(
		source.pos + Vectorf(500, -500), source.pos + Vectorf(-500, -500)));
	map_edges.push_back(std::make_pair(
		source.pos + Vectorf(-500, -500), source.pos + Vectorf(-500, 500)));
	map_vertices.push_back(source.pos + Vectorf(-500, 500));
	map_vertices.push_back(source.pos + Vectorf(500, 500));
	map_vertices.push_back(source.pos + Vectorf(500, -500));
	map_vertices.push_back(source.pos + Vectorf(-500, -500));
	for (const auto& vertex_it : map_vertices)
	{
		Vectorf dist = vertex_it - source.pos;
		if (dist == Vectorf(0, 0))	//atan2 domain
			continue;
		if (util::sq(dist.x) + util::sq(dist.y) < 1000050)//500050
		{
			alfa = vertex_it - source.pos;
			points.push_back(cast_ray(source.pos, alfa, map_edges));
			float angle = atan2(-alfa.y, alfa.x);
			points.push_back(cast_ray(source.pos,
				Vectorf(1 * util::sgn(alfa.x),
					tan(angle + 0.0001f) * -util::sgn(alfa.x)), map_edges));
			points.push_back(cast_ray(source.pos,
				Vectorf(1 * util::sgn(alfa.x),
					tan(angle - 0.0001f) * -util::sgn(alfa.x)), map_edges));
		}
	}
	map_edges.pop_back();
	map_edges.pop_back();
	map_edges.pop_back();
	map_edges.pop_back();
	map_vertices.pop_back();
	map_vertices.pop_back();
	map_vertices.pop_back();
	map_vertices.pop_back();
	std::sort(points.begin(), points.end(),
		[](const std::pair<float, Vectorf>& a, const std::pair<float, Vectorf>& b)
	{return a.first > b.first; });
	return points;
}