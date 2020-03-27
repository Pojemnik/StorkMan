#include "map.h"

const sf::Vector2f level_size = { 1024,1024 };

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

Map::Map(Vectori dimensions, std::vector<Level>& lvls, Vectori start_pos) : size(dimensions), current_pos(start_pos)
{
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
			for (auto& vertex_it : col_it->mesh_collision)
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
	lvl=loaded_levels.erase(lvl);
}

void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
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
}


void Map::update()
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
			if ((*level_it)->global_pos.x +(*level_it)->global_size.x < current_pos.x - 1 || (*level_it)->global_pos.x > current_pos.x + 1 || (*level_it)->global_pos.y + (*level_it)->global_size.y < current_pos.y - 1 || (*level_it)->global_pos.y > current_pos.y + 1)
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
				if (current_pos.x + x < size.x && current_pos.y + y < size.y && current_pos.x + x >= 0 && current_pos.y + y >= 0 && !level_placement[current_pos.x+x][current_pos.y+y]->is_loaded)
					load_level(Vectori(current_pos.x + x, current_pos.y + y));
			}
		}
	}
	for (auto& level_it : loaded_levels)
	{
		for (auto& physical_it : level_it->physicals)
		{
			physical_it->update();
			for (auto& colidable_it : level_it->colidables)
			{
				physical_it->uncolide(colidable_it);
			}
		}
		for (auto& colidable_it : level_it->colidables)
		{
			player->uncolide(colidable_it);
		}
	}
}