#include "map.h"

Map::Map()
{
	level_placement = nullptr;
	player = nullptr;
	light_texture = nullptr;
	global_scale = .0f;
}

Map::Map(Vectori dimensions, std::vector<Level>& lvls, Vectori start_pos,
	sf::Texture& bg_tex, sf::Texture& layer2_tex, sf::Texture* light_tex)
	: size(dimensions), current_pos(start_pos), light(level_size, light_tex)
{
	background.setTexture(bg_tex);
	layer2.setTexture(layer2_tex);
	level_placement = new Level * *[size.y];
	for (int i = 0; i < size.y; i++)
	{
		level_placement[i] = new Level * [size.x];
	}
	levels = lvls;
	place_levels();
	load_levels_in_bounds(start_pos);
	calc_map_vertices();
	global_scale = context.global_scale;
	light.lightmap.setPosition(0, 0);
	light_texture = nullptr;
}

void Map::place_levels()
{
	for (auto& it : levels)
	{
		for (int i = 0; i < it.global_size.x; i++)
		{
			for (int j = 0; j < it.global_size.y; j++)
			{
				level_placement[it.global_pos.x + i][it.global_pos.y + j] = &it;
			}
		}
		for (auto& col_it : it.collidables)
		{
			col_it->rect_collision.left += it.global_pos.x * level_size.x;
			col_it->rect_collision.top += it.global_pos.y * level_size.y;
			for (auto& vertex_it : col_it->mesh.vertices)
			{
				vertex_it += Vectorf(it.global_pos.x * level_size.x,
					it.global_pos.y * level_size.y);
			}
		}
		for (auto& light_it : it.light_sources)
		{
			light_it.pos.x += it.global_pos.x * level_size.x;
			light_it.pos.y += it.global_pos.y * level_size.y;
		}
	}
}

void Map::load_level(Vectori pos)
{
	if (!level_placement[pos.x][pos.y]->is_loaded)
	{
		level_placement[pos.x][pos.y]->is_loaded = true;
		loaded_levels.push_back(level_placement[pos.x][pos.y]);
	}
}

void Map::unload_level(std::list<Level*>::iterator& lvl)
{
	(*lvl)->is_loaded = false;
	lvl = loaded_levels.erase(lvl);
}

void Map::draw_backgrounds(sf::RenderTarget& target, sf::RenderStates states) const
{
	const float* matrix = states.transform.getMatrix();
	Vectorf move = { matrix[12], matrix[13] };
	Vectorf bg_move_parallax = move / context.parallax;
	context.bg_states = states;
	context.bg_states.transform.translate(bg_move_parallax);
	target.draw(background, context.bg_states);
	Vectorf layer2_move_parallax = move / context.parallax2;
	context.layer2_states = states;
	context.layer2_states.transform.translate(layer2_move_parallax);
	target.draw(layer2, context.layer2_states);
}

void Map::draw_bottom_layers(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const auto& it : loaded_levels)
	{
		states.transform *= sf::Transform().translate(
			{ level_size.x * it->global_pos.x,
			level_size.y * it->global_pos.y }
		);
		for (const auto& it2 : it->bottom_layers)
		{
			for (const auto& it3 : it2)
			{
				target.draw(*it3, states);
			}
		}
		states.transform *= sf::Transform().translate(
			{ -1 * level_size.x * it->global_pos.x,
			-1 * level_size.y * it->global_pos.y }
		);
	}
}

void Map::draw_middle_layers(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const auto& it : loaded_levels)
	{
		states.transform *= sf::Transform().translate(
			{ level_size.x * it->global_pos.x,
			level_size.y * it->global_pos.y }
		);
		for (const auto& it2 : it->middle_layers)
		{
			for (const auto& it3 : it2)
			{
				target.draw(*it3, states);
			}
		}
		states.transform *= sf::Transform().translate(
			{ -1 * level_size.x * it->global_pos.x,
			-1 * level_size.y * it->global_pos.y }
		);
	}
}

void Map::draw_top_layers(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const auto& it : loaded_levels)
	{
		states.transform *= sf::Transform().translate(
			{ level_size.x * it->global_pos.x,
			level_size.y * it->global_pos.y }
		);
		for (const auto& it2 : it->top_layers)
		{
			for (const auto& it3 : it2)
			{
				target.draw(*it3, states);
			}
		}
		states.transform *= sf::Transform().translate(
			{ -1 * level_size.x * it->global_pos.x,
			-1 * level_size.y * it->global_pos.y }
		);
	}
}

void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	draw_backgrounds(target, states);
	draw_bottom_layers(target, states);
}

void Map::calc_map_vertices()
{
	std::list<std::pair<Vectorf, Vectorf> > tab;
	map_edges.clear();
	for (const auto& it : loaded_levels)
	{
		for (const auto& it2 : it->platforms)
		{
			if (it2.visible)
			{
				for (size_t i = 1; i < it2.vertices.size(); i++)
				{
					tab.push_back(std::make_pair(it2.vertices[i].position + it2.pos, it2.vertices[i - 1].position + it2.pos));
				}
				tab.push_back(std::make_pair(
					it2.vertices.back().position + it2.pos, it2.vertices.front().position + it2.pos));
			}
		}
	}
	for (auto it1 = tab.begin(); it1 != tab.end(); it1++)
	{
		Vectorf normal = it1->second - it1->first;
		float a = util::vector_dot_product(normal, it1->first), b = util::vector_dot_product(normal, it1->second);
		int removed = 1;
		for (auto it2 = (++it1)--; it2 != tab.end(); std::advance(it2, removed))
		{
			Vectorf normal2 = it2->second - it2->first;
			removed = 1;
			if (fabs(util::vector_cross_product(normal, normal2)) > 0.0001 || fabs(util::vector_cross_product(normal, it2->first - it1->first)) > 0.0001)
				continue;
			float c = util::vector_dot_product(normal, it2->first), d = util::vector_dot_product(normal, it2->second);
			if (std::min(a, b) <= std::max(c, d) && std::min(a, b) >= std::min(c, d) || std::max(a, b) <= std::max(c, d) && std::max(a, b) >= std::min(c, d))
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
	map_edges.insert(map_edges.begin(), tab.begin(), tab.end());
	map_vertices.clear();
	for (const auto it : map_edges)
	{
		map_vertices.push_back(it.first);
		map_vertices.push_back(it.second);
	}
	std::sort(map_vertices.begin(), map_vertices.end(), util::vectorf_compare);
	const auto last = std::unique(map_vertices.begin(), map_vertices.end(), util::vectorf_binary_predicate);
	map_vertices.erase(last, map_vertices.end());
}

void Map::unload_levels_out_of_bounds()
{
	int removed;
	for (auto level_it = loaded_levels.begin();
		level_it != loaded_levels.end();
		std::advance(level_it, removed))
	{
		const Vectori level_pos = {
			(*level_it)->global_pos.x,
			(*level_it)->global_pos.y
		};
		removed = 1;
		if (level_pos.x + (*level_it)->global_size.x < current_pos.x - 1 ||
			level_pos.x > current_pos.x + 1 ||
			level_pos.y + (*level_it)->global_size.y < current_pos.y - 1 ||
			level_pos.y > current_pos.y + 1)
		{
			if ((*level_it)->is_loaded)
			{
				removed = 0;
				unload_level(level_it);
			}
		}
	}
}

void Map::load_levels_in_bounds(Vectori pos)
{
	for (int x = -1; x < 2; x++)
	{
		for (int y = -1; y < 2; y++)
		{
			if (pos.x + x < size.x && pos.y + y < size.y &&
				pos.x + x >= 0 && pos.y + y >= 0 &&
				!level_placement[pos.x + x][pos.y + y]->is_loaded)
				load_level(Vectori(pos.x + x, pos.y + y));
		}
	}
}

void Map::update(float dt)
{
	if (light_texture == nullptr)
	{
		recalc_light();
	}
	background.setPosition(context.background_position);
	background.setScale(context.background_scale, context.background_scale);
	layer2.setPosition(context.layer2_position);
	layer2.setScale(context.layer2_scale, context.layer2_scale);
	Vectori pos = {
		int(player->get_position().x) / int(level_size.x),
		int(player->get_position().y) / int(level_size.y)
	};
	if (pos != current_pos)
	{
		current_pos = pos;
		unload_levels_out_of_bounds();
		load_levels_in_bounds(current_pos);
		recalc_light();
	}
	for (auto& level_it : loaded_levels)
	{
		for (auto& physical_it : level_it->physicals)
		{
			physical_it->update(dt);
			for (auto& colidable_it : level_it->collidables)
			{
				physical_it->uncollide(colidable_it, dt);
			}
		}
		Vectorf maxv = { 0,0 };
		for (auto& physical_it : level_it->physicals)
		{
			Vectorf tmp = player->uncollide(physical_it, dt);
			if (tmp.y > maxv.y)
				maxv = tmp;
		}
		for (auto& colidable_it : level_it->collidables)
		{
			Vectorf tmp = player->uncollide(colidable_it, dt);
			if (tmp.y > maxv.y)
				maxv = tmp;
		}
		if (maxv.x == 0 && maxv.y == 0)
			maxv = {0,1};
		player->maxcollisionvector = util::normalize(maxv);
	}
}

void Map::pre_draw()
{
	for (auto& level_it : loaded_levels)
	{
		for (auto& animatable_it : level_it->anim_objects)
		{
			animatable_it.next_frame();
		}
	}
}

void Map::recalc_light()
{
	if (light_texture == nullptr)
	{
		light_texture = new sf::RenderTexture();
		if (!light_texture->create((unsigned int)level_size.x * 2, (unsigned int)level_size.y * 2))
		{
			std::cerr << "Error creating walls buffer" << std::endl;
			throw std::runtime_error("Memory error");
		}
	}
	calc_map_vertices();
	if (context.night)
	{
		context.final_states.shader = NULL;
		light_texture->clear(sf::Color(255, 255, 255));
	}
	else
	{
		light_texture->clear(sf::Color(0, 0, 0, 0));
		context.final_states.shader = &context.global;
		sf::RenderStates walls_states;
		context.white_states.transform *= sf::Transform().translate(level_size.x / 2,
			level_size.y / 2);
		for (const auto& it : loaded_levels)
		{
			context.white_states.transform *= sf::Transform().translate(
				{ level_size.x * it->global_pos.x,
				level_size.y * it->global_pos.y }
			);
			for (auto& it2 : it->walls)
			{
				light_texture->draw(it2, context.white_states);
			}
			//for (const auto& it2 : it->platforms)
			//{
			//	light_texture->draw(it2, context.white_states);
			//}
			context.white_states.transform *= sf::Transform().translate(
				{ -1 * level_size.x * it->global_pos.x,
				-1 * level_size.y * it->global_pos.y }
			);
		}
		context.white_states.transform *= sf::Transform().translate(-level_size.x / 2,
			-level_size.y / 2);
	}
	std::vector<Light_source> sources;
	for (const auto& level_it : loaded_levels)
	{
		sources.insert(sources.begin(), level_it->light_sources.begin(), level_it->light_sources.end());
	}
	sf::Transform transform = sf::Transform().translate(level_size.x / 2,
		level_size.y / 2);
	light.calc_light(sources, transform, map_edges, map_vertices);
	sf::RenderStates states;
	states.blendMode = sf::BlendMultiply;
	light_texture->draw(light.lightmap, states);
	light_texture->display();
	light_sprite.setTexture(light_texture->getTexture());
}

void Map::rescale(float new_global_scale)
{
	level_size = { 100 * new_global_scale, 100 * new_global_scale };
	float ratio = new_global_scale / global_scale;
	global_scale = new_global_scale;
	for (auto& it : levels)
	{
		it.rescale(ratio);
	}
	light.rescale(level_size);
	delete light_texture;
	light_texture = nullptr;
	recalc_light();
}