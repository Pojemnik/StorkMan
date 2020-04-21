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
			{
				load_level(Vectori(start_pos.x + x, start_pos.y + y));
			}
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
	context.light_states.blendMode = sf::BlendAdd;
	lightmap.setPosition(0, 0);
	map_texture = nullptr;
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
std::vector<std::pair<float, Vectorf>> Map::calc_light_source(Vectorf source, Vectorf move) const
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
				Vectorf dist = texturables_it->pos - source;
				if (sqrt(pow(dist.x, 2) + pow(dist.y, 2)) < 500 * sqrt(2))
				{
					alfa = texturables_it->vertices[i].position + texturables_it->pos - source;
					points.push_back(cast_ray(source, alfa));
					float angle = atan2(alfa.y, alfa.x);
					points.push_back(cast_ray(source, Vectorf(1, tan(angle + 0.00001f))));
					points.push_back(cast_ray(source, Vectorf(1, tan(angle - 0.00001f))));
				}
			}
		}
	}
	Vectorf a = { float(-context.resolution.x),float(-context.resolution.y) };
	Vectorf b = { float(-context.resolution.x),float(context.resolution.y * 2) };
	Vectorf c = { float(context.resolution.x),float(context.resolution.y * 2) };
	Vectorf d = { float(context.resolution.x),float(-context.resolution.y) };
	points.push_back(cast_ray(source, Vectorf(a.y - source.y, a.x - source.x)));
	points.push_back(cast_ray(source, Vectorf(b.y - source.y, b.x - source.x)));
	points.push_back(cast_ray(source, Vectorf(c.y - source.y, c.x - source.x)));
	points.push_back(cast_ray(source, Vectorf(d.y - source.y, d.x - source.x)));
	std::sort(points.begin(), points.end(),
		[](const std::pair<float, Vectorf>& a, const std::pair<float, Vectorf>& b)
	{return a.first > b.first; });
	return points;
}

sf::Texture Map::calc_light(std::vector<Vectorf>& sources, sf::Transform transform) const
{
	context.lightmap.clear(sf::Color(20, 20, 20, 255));
	const float* matrix = transform.getMatrix();
	Vectorf move = { matrix[12], matrix[13] };
	for (Vectorf source : sources)
	{
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
		context.light_states.transform = transform;
		context.lightmap.draw(light, context.light_states);
	}
	context.lightmap.display();
	return context.lightmap.getTexture();
	//sf::Sprite lightmap;
	//lightmap.setTexture(context.lightmap.getTexture());
	//context.lm2.draw(lightmap, context.blurh_states);
	//context.lm2.display();
	//lightmap.setTexture(context.lm2.getTexture());
	//context.lm3.draw(lightmap, context.blurv_states);
	//context.lm3.display();
	//return context.lm3.getTexture();
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
	target.draw(map_sprite, states);
}

void Map::calc_map_vertices()
{
	std::list<std::pair<Vectorf, Vectorf> > tab;
	map_vertices.clear();
	for (const auto& it : loaded_levels)
	{
		for (const auto& it2 : it->texturables)
		{
			for (int i = 1; i < it2->vertices.size(); i++)
			{
				tab.push_back(std::make_pair(it2->vertices[i].position + it2->pos, it2->vertices[i - 1].position + it2->pos));
			}
			tab.push_back(std::make_pair(
				it2->vertices.back().position + it2->pos, it2->vertices.front().position + it2->pos));
		}
	}
	tab.push_back({ Vectorf(-context.resolution.x,-context.resolution.y), Vectorf(-context.resolution.x,context.resolution.y * 2) });
	tab.push_back({ Vectorf(-context.resolution.x,context.resolution.y * 2), Vectorf(context.resolution.x * 2,context.resolution.y * 2) });
	tab.push_back({ Vectorf(context.resolution.x * 2,context.resolution.y * 2), Vectorf(context.resolution.x * 2,-context.resolution.y) });
	tab.push_back({ Vectorf(context.resolution.x * 2,-context.resolution.y), Vectorf(-context.resolution.x,-context.resolution.y) });

	for (auto it1 = tab.begin(); it1 != tab.end(); it1++)
	{
		Vectorf normal = it1->second-it1->first;
		float a = util::vector_dot_product(normal, it1->first), b = util::vector_dot_product(normal, it1->second);
		int removed = 1;
		for (auto it2 = (++it1)--; it2 != tab.end(); std::advance(it2, removed))
		{
			Vectorf normal2 =it2->second-it2->first;
			removed = 1;
			if (fabs(normal.x * normal2.y - normal.y * normal2.x) > 0.0001 || fabs(normal.y*(it2->first.x-it1->first.x)-normal.x*(it2->first.y - it1->first.y)) > 0.0001)
				continue;
			float c = util::vector_dot_product(normal, it2->first), d = util::vector_dot_product(normal, it2->second);
			if (std::min(a, b) <= std::max(c, d) && std::min(a, b) >= std::min(c, d)|| std::max(a, b) <= std::max(c, d) && std::max(a, b) >= std::min(c, d))
			{
				std::pair<Vectorf, float> tmp[4] = { {it1->first,a},{it1->second,b},{it2->first,c},{it2->second,d} };
				std::sort(tmp, tmp + 4, [](std::pair<Vectorf, float> a, std::pair<Vectorf, float> b) {return a.second < b.second; });
				it1->first = tmp[0].first;
				it1->second = tmp[3].first;
				a = tmp[0].second;
				b = tmp[3].second;
				removed = 0;
				it2 = tab.erase(it2);
			}
		}
	}
	map_vertices.insert(map_vertices.begin(), tab.begin(), tab.end());

}

void Map::update(float dt)
{
	if (map_texture == nullptr)
	{
		redraw();
	}
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
		redraw();
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

void Map::redraw()
{
	if (map_texture == nullptr)
	{
		map_texture = new sf::RenderTexture();
		if (!map_texture->create(level_size.x * 2, level_size.y * 2))
			std::cerr << "Error creating map" << std::endl;
	}
	map_texture->clear(sf::Color(0, 0, 0, 0));
	sf::RenderStates states;
	for (const auto& it : loaded_levels)
	{
		states.transform *= sf::Transform().translate({ level_size.x * it->global_pos.x,level_size.y * it->global_pos.y });
		for (const auto& it2 : it->drawables)
		{
			map_texture->draw(*it2, states);
		}
		for (const auto& it2 : it->texturables)
		{
			map_texture->draw(*it2, states);
		}
		states.transform *= sf::Transform().translate({ -1 * level_size.x * it->global_pos.x,-1 * level_size.y * it->global_pos.y });
	}
	map_texture->display();
	map_sprite.setTexture(map_texture->getTexture());
}