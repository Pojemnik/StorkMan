#include "map.h"

Level::Level()
{
}

Level::Level(const Level& level) : global_pos(level.global_pos), is_loaded(level.is_loaded), global_size(level.global_size), platforms(level.platforms)
{
	for (auto& it : platforms)
	{
		addColidable(&it);
		addTexturable(&it);
	}
}

void Level::addRenderable(Renderable* d)
{
	drawables.push_back(d);
}

void Level::addTexturable(Texturable* t)
{
	texturables.push_back(t);
}

void Level::addPhysical(Physical* p)
{
	physicals.push_back(p);
}

void Level::addColidable(Colidable* c)
{
	colidables.push_back(c);
}

void Level::addPlatfrom(Platform p)
{
	platforms.push_back(p);
	addColidable(&p);
	addTexturable(&p);
}

Map::Map(Vectori dimensions, std::vector<Level>& lvls, Vectori start_pos, sf::Texture& bg) : size(dimensions), current_pos(start_pos)
{
	background.setTexture(bg);
	level_placement = new Level * *[size.y];
	for (int i = 0; i < size.y; i++)
	{
		level_placement[i] = new Level * [size.x];
	}
	levels = lvls;
	for (auto& it : levels)
	{
		for (int i = 0; i < it.global_size.x; i++)
		{
			for (int j = 0; j < it.global_size.y; j++)
			{
				level_placement[it.global_pos.x + i][it.global_pos.y + j] = &it;
			}
		}
		for (auto& col_it : it.colidables)
		{
			col_it->rect_collision.left += it.global_pos.x * level_size.x;
			col_it->rect_collision.top += it.global_pos.y * level_size.y;
			for (auto& vertex_it : col_it->mesh.vertices)
			{
				vertex_it += Vectorf(it.global_pos.x * level_size.x, it.global_pos.y * level_size.y);
			}
		}
	}
	for (int x = -1; x < 2; x++)
	{
		for (int y = -1; y < 2; y++)
		{
			if (start_pos.x + x < size.x && start_pos.y + y < size.y && start_pos.x + x >= 0 && start_pos.y + y >= 0)
				load_level(Vectori(start_pos.x + x, start_pos.y + y));
		}
	}
	calc_map_vertices();
	if (!(context.lightmap.create(context.resolution.x, context.resolution.y)))
	{
		std::cerr << "Error creating lightmaps" << std::endl;
	}
	if (!(context.lm2.create(context.resolution.x, context.resolution.y) && context.lm3.create(context.resolution.x, context.resolution.y) && context.lm4.create(context.resolution.x, context.resolution.y)))
	{
		std::cerr << "Error creating lightmaps" << std::endl;
	}
	context.map_states.shader = &context.generate_map;
	context.blurh_states.shader = &context.blurh;
	context.blurv_states.shader = &context.blurv;
	context.final_states.blendMode = sf::BlendMultiply;
	lightmap.setPosition(0, 0);
}

void Map::load_level(Vectori pos)
{
	if (!level_placement[pos.x][pos.y]->is_loaded)
	{
		level_placement[pos.x][pos.y]->is_loaded = true;
		loaded_levels.push_back(level_placement[pos.x][pos.y]);
	}
}

void Map::unload_level(Vectori pos)//Never called
{
	if (level_placement[pos.x][pos.y]->is_loaded)
	{
		level_placement[pos.x][pos.y]->is_loaded = false;
		auto x = &*(level_placement[pos.x][pos.y]);
		loaded_levels.remove_if([=](const Level* a) {return &*a == (const Level*)&x; });
	}
}

void Map::unload_level(std::list<Level*>::iterator& lvl)
{
	(*lvl)->is_loaded = false;
	lvl = loaded_levels.erase(lvl);
}

std::pair<float, Vectorf> Map::cast_ray(Vectorf source, Vectorf alfa) const
{
	Vectorf beta;
	float min_t1 = INFINITY;
	Vectorf point = source;
	for (auto& it : map_vertices)
	{
		beta = { it.first.x - it.second.x, it.first.y - it.second.y };
		float t2 = ((alfa.x * (it.second.y - source.y) + alfa.y * (source.x - it.second.x))
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
	return std::pair<float, Vectorf>(atan2(alfa.y, alfa.x), point);
}
std::vector<std::pair<float, Vectorf>> Map::calc_light_source(Vectorf source, Vectorf delta) const
{
	std::vector<std::pair<float, Vectorf>> points;
	Vectorf point;
	Vectorf alfa;
	for (const auto& level_it : loaded_levels)
	{
		for (const auto& texturables_it : level_it->texturables)
		{
			for (int i = 0; i < texturables_it->vertices.size(); i++)
			{
				alfa = texturables_it->vertices[i].position + texturables_it->pos - source;
				points.push_back(cast_ray(source, alfa));
				float angle = atan2(alfa.y, alfa.x);
				points.push_back(cast_ray(source, Vectorf(1, tan(angle + 0.00001f))));
				points.push_back(cast_ray(source, Vectorf(1, tan(angle - 0.00001f))));
			}
		}
	}
	Vectorf a = { -context.resolution.x,-context.resolution.y };
	Vectorf b = { -context.resolution.x,context.resolution.y*2 };
	Vectorf c = { context.resolution.x,context.resolution.y * 2 };
	Vectorf d = { context.resolution.x,-context.resolution.y };
	points.push_back(std::make_pair(atan2(a.y - source.y, a.x - source.x), a));
	points.push_back(std::make_pair(atan2(b.y - source.y, b.x - source.x), b));
	points.push_back(std::make_pair(atan2(c.y - source.y, c.x - source.x), c));
	points.push_back(std::make_pair(atan2(d.y - source.y, d.x - source.x), d));
	std::sort(points.begin(), points.end(),
		[](const std::pair<float, Vectorf>& a, const std::pair<float, Vectorf>& b)
	{return a.first > b.first; });
	return points;
}

void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	const float* matrix = states.transform.getMatrix();
	Vectorf move = { matrix[12], matrix[13] };
	Vectorf bg_move_parallax = move / context.parrallax;
	context.bg_states = states;
	context.bg_states.transform.translate(bg_move_parallax);
	target.draw(background, context.bg_states);
	Vectorf layer2_move_parallax = move / context.parrallax2;
	context.layer2_states = states;
	context.layer2_states.transform.translate(layer2_move_parallax);
	target.draw(layer2, context.layer2_states);
	for (const auto& it : loaded_levels)
	{
		states.transform *= sf::Transform().translate({ level_size.x * it->global_pos.x,level_size.y * it->global_pos.y });
		for (const auto& it2 : it->drawables)
		{
			target.draw(*it2, states);
		}
		for (const auto& it2 : it->texturables)
		{
			target.draw(*it2, states);
		}
		states.transform *= sf::Transform().translate({ -1 * level_size.x * it->global_pos.x,-1 * level_size.y * it->global_pos.y });
	}
	Vectorf source = { 300, 300 };
	std::vector<std::pair<float, Vectorf>> points = calc_light_source(source, move);
	sf::VertexArray light(sf::TriangleFan, points.size() + 2);
	light[0].position = source;
	light[0].texCoords = { 500,500 };
	for (int i = 1; i < points.size() + 1; i++)
	{
		light[i].position = points[i - 1].second;
		light[i].texCoords = points[i - 1].second + Vectorf(500, 500) - source;
	}
	light[points.size() + 1].position = points[0].second;
	light[points.size() + 1].texCoords = points[0].second + Vectorf(500, 500) - source;
	context.lightmap.clear(sf::Color(50, 50, 50, 255));
	context.light_states.transform = states.transform;
	context.lightmap.draw(light, context.light_states);
	context.lightmap.display();
	sf::Texture tex = context.lightmap.getTexture();
	sf::Sprite s(tex);
	target.draw(s, context.final_states);
	/*
	context.lightmap.clear(sf::Color(0, 0, 0, 0));
	context.lm2.clear(sf::Color(0, 0, 0, 0));
	context.lightmap.draw(light, states);
	context.lightmap.display();
	sf::Texture tex = context.lightmap.getTexture();
	sf::Sprite s;
	s.setTexture(tex);
	source += delta;
	context.shade.setUniform("light_pos", Vectorf(source.x / context.resolution.x, 1.f - source.y / context.resolution.y));
	context.shade.setUniform("texture", sf::Shader::CurrentTexture);
	context.shade.setUniform("dark", .2f);
	context.shade.setUniform("aspect", context.resolution.x / context.resolution.y);
	sf::RenderStates st;
	st.shader = &context.shade;
	context.lm2.draw(s, st);
	context.lm2.display();
	tex = context.lm2.getTexture();
	s.setTexture(tex);
	target.draw(s, context.final_states);
	*/
}

void Map::generate_lightmap(sf::RenderStates states)
{
	const float* matrix = states.transform.getMatrix();
	Vectorf move = { matrix[12], matrix[13] };
	context.lightmap.clear(sf::Color(255, 255, 255, 255));
	context.lm2.clear();
	context.lm3.clear();
	context.lm4.clear();
	context.states_black = states;
	context.states_black.shader = &context.black;
	for (const auto& it : loaded_levels)
	{
		context.states_black.transform *= sf::Transform().translate({ level_size.x * it->global_pos.x,level_size.y * it->global_pos.y });
		for (const auto& it2 : it->texturables)
		{
			context.lightmap.draw(*it2, context.states_black);
		}
		context.states_black.transform *= sf::Transform().translate({ -1 * level_size.x * it->global_pos.x,-1 * level_size.y * it->global_pos.y });
	}
	context.lightmap.display();
	Vectorf source = { 0.7f, 0.7f };
	Vectorf delta = { move.x / context.resolution.x, -move.y / context.resolution.y };
	source += delta;
	context.generate_map.setUniform("light_pos", source);
	context.generate_map.setUniform("delta", delta);
	context.generate_map.setUniform("light_range", 2.f);
	lightmap.setTexture(context.lightmap.getTexture());
	lightmap.setScale(.5f, .5f);
	context.lm2.draw(lightmap, context.map_states);
	context.lm2.display();
	lightmap.setScale(1.f, 1.f);
	lightmap.setTexture(context.lm2.getTexture());
	context.lm3.draw(lightmap, context.blurh_states);
	context.lm3.display();
	lightmap.setTexture(context.lm3.getTexture());
	context.lm4.draw(lightmap, context.blurv_states);
	context.lm4.display();
	lightmap.setTexture(context.lm4.getTexture());
	lightmap.setScale(2.f, 2.f);
}
void Map::calc_map_vertices()
{
	map_vertices.clear();
	for (const auto& it : loaded_levels)
	{
		for (const auto& it2 : it->texturables)
		{
			for (int i = 1; i < it2->vertices.size(); i++)
			{
				map_vertices.push_back(std::make_pair(it2->vertices[i].position + it2->pos, it2->vertices[i - 1].position + it2->pos));
			}
			map_vertices.push_back(std::make_pair(
				it2->vertices.back().position + it2->pos, it2->vertices.front().position + it2->pos));
		}
	}
	map_vertices.push_back({ {-context.resolution.x,-context.resolution.y}, {-context.resolution.x,context.resolution.y * 2} });
	map_vertices.push_back({ {-context.resolution.x,context.resolution.y * 2}, {context.resolution.x * 2,context.resolution.y * 2} });
	map_vertices.push_back({ {context.resolution.x * 2,context.resolution.y * 2}, {context.resolution.x * 2,-context.resolution.y} });
	map_vertices.push_back({ {context.resolution.x * 2,-context.resolution.y}, {-context.resolution.x,-context.resolution.y} });
}
void Map::update(float dt)
{
	background.setPosition(context.background_position);
	background.setScale(context.background_scale, context.background_scale);
	layer2.setPosition(context.layer2_position);
	layer2.setScale(context.layer2_scale, context.layer2_scale);
	Vectori pos = { int(player->get_position().x) / int(level_size.x), int(player->get_position().y) / int(level_size.y) };
	if (pos != current_pos)
	{
		Vectori delta = pos - current_pos;
		current_pos = pos;
		int removed = 1;
		for (auto level_it = loaded_levels.begin(); level_it != loaded_levels.end(); std::advance(level_it, removed))
		{
			removed = 1;
			if ((*level_it)->global_pos.x + (*level_it)->global_size.x < current_pos.x - 1 || (*level_it)->global_pos.x > current_pos.x + 1 || (*level_it)->global_pos.y + (*level_it)->global_size.y < current_pos.y - 1 || (*level_it)->global_pos.y > current_pos.y + 1)
			{
				if ((*level_it)->is_loaded)
				{
					removed = 0;
					unload_level(level_it);
				}
			}
		}
		for (int x = -1; x < 2; x++)
		{
			for (int y = -1; y < 2; y++)
			{
				if (current_pos.x + x < size.x && current_pos.y + y < size.y && current_pos.x + x >= 0 && current_pos.y + y >= 0 && !level_placement[current_pos.x + x][current_pos.y + y]->is_loaded)
					load_level(Vectori(current_pos.x + x, current_pos.y + y));
			}
		}
		calc_map_vertices();
	}
	for (auto& level_it : loaded_levels)
	{
		for (auto& physical_it : level_it->physicals)
		{
			physical_it->update(dt);
			for (auto& colidable_it : level_it->colidables)
			{
				physical_it->uncolide(colidable_it, dt);
			}
		}
		Vectorf maxv = { 0,0 };
		for (auto& colidable_it : level_it->colidables)
		{
			Vectorf tmp = player->uncolide(colidable_it, dt);
			if (tmp.y > maxv.y)
				maxv = tmp;
		}
		player->maxcollisionvector = maxv;
	}
}