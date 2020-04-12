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

const sf::Texture& Map::generate_lightmap(sf::RenderStates states) const
{
	sf::RenderTexture lightmap;
	if (!lightmap.create(1024, 576))
	{
		std::cout << "aaaa" << std::endl;
	}
	lightmap.clear(sf::Color(255, 255, 255, 0));
	states.shader = &context.black;
	for (const auto& it : loaded_levels)
	{
		states.transform *= sf::Transform().translate({ level_size.x * it->global_pos.x,level_size.y * it->global_pos.y });
		for (const auto& it2 : it->drawables)
		{
			lightmap.draw(*it2, states);
		}
		for (const auto& it2 : it->texturables)
		{
			lightmap.draw(*it2, states);
		}
		states.transform *= sf::Transform().translate({ -1 * level_size.x * it->global_pos.x,-1 * level_size.y * it->global_pos.y });
	}
	lightmap.display();
	return lightmap.getTexture();
}

void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	const float* matrix = states.transform.getMatrix();
	Vectorf move = { matrix[12] / context.parrallax, matrix[13] / context.parrallax };
	sf::RenderStates bg_states = states;
	sf::RenderTexture lightmap, lm2, lm3, final_map;
	if (!lightmap.create(1024, 576))
	{
		std::cout << "aaaa" << std::endl;
	}
	if (!lm2.create(1024, 576))
	{
		std::cout << "aaaa2" << std::endl;
	}
	if (!lm3.create(1024, 576))
	{
		std::cout << "aaaa3" << std::endl;
	}
	if (!final_map.create(1024, 576))
	{
		std::cout << "final aaaa" << std::endl;
	}
	lightmap.clear(sf::Color(255, 255, 255, 255));
	lm2.clear();
	lm3.clear();
	final_map.clear();
	bg_states.transform.translate(move);
	final_map.draw(background, bg_states);
	sf::RenderStates states_black = states;
	states_black.shader = &context.black;
	for (const auto& it : loaded_levels)
	{
		states_black.transform *= sf::Transform().translate({ level_size.x * it->global_pos.x,level_size.y * it->global_pos.y });
		for (const auto& it2 : it->texturables)
		{
			lightmap.draw(*it2, states_black);
		}
		states_black.transform *= sf::Transform().translate({ -1 * level_size.x * it->global_pos.x,-1 * level_size.y * it->global_pos.y });
	}
	lightmap.display();
	Vectorf source = { 0.7f - move.x / 1024.0f, 0.7f - move.y / 576.0f };
	sf::Texture tex = lightmap.getTexture();
	sf::Sprite s;
	s.setTexture(tex);
	s.setPosition(0, 0);
	sf::RenderStates map_states;
	context.generate_map.setUniform("texture", sf::Shader::CurrentTexture);
	context.generate_map.setUniform("light_pos", source);
	context.generate_map.setUniform("samples", 100.0f);
	map_states.shader = &context.generate_map;
	lm2.draw(s, map_states);
	lm2.display();
	sf::Texture tex2 = lm2.getTexture();
	s.setTexture(tex2);
	sf::RenderStates blur_states;
	context.blur.setUniform("sigma", 5.0f);
	context.blur.setUniform("blurSize", 1.0f/1024.0f);
	context.blur.setUniform("blurSampler", sf::Shader::CurrentTexture);
	blur_states.shader = &context.blur;
	lm3.draw(s, blur_states);
	lm3.display();
	sf::Texture tex3 = lm3.getTexture();
	for (const auto& it : loaded_levels)
	{
		states.transform *= sf::Transform().translate({ level_size.x * it->global_pos.x,level_size.y * it->global_pos.y });
		for (const auto& it2 : it->drawables)
		{
			final_map.draw(*it2, states);
		}
		for (const auto& it2 : it->texturables)
		{
			final_map.draw(*it2, states);
		}
		states.transform *= sf::Transform().translate({ -1 * level_size.x * it->global_pos.x,-1 * level_size.y * it->global_pos.y });
	}
	sf::RenderStates final_states;
	final_map.display();
	s.setTexture(final_map.getTexture());
	context.blend.setUniform("texture", sf::Shader::CurrentTexture);
	context.blend.setUniform("light", tex3);
	final_states.shader = &context.blend;
	target.draw(s, final_states);
}


void Map::update(float dt)
{
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
			if (abs(tmp.y) > abs(maxv.y))
				maxv = tmp;
		}

	}
}