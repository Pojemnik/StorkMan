
#include "map.h"
const sf::Vector2f level_size = { 500,500 };
Level::Level()
{
}

void Level::addRenderable(Renderable d)
{
	drawables.push_back(d);
}

void Level::addTexturable(Texturable t)
{
	texturables.push_back(t);
}

Map::Map(Vectori dimensions, std::unique_ptr<std::vector<Level>> lvls, Vectori start_pos) : size(dimensions), current_pos(start_pos)
{
	level_placement = new std::shared_ptr<Level> * [size.y];
	for (int i = 0; i < size.y; i++)
	{
		level_placement[i] = new std::shared_ptr<Level>[size.x];
	}
	levels = *lvls.release();
	for (const auto& it : levels)
	{
		for (int i = 0; i < it.global_size.x; i++)
		{
			for (int j = 0; j < it.global_size.y; j++)
			{
				level_placement[it.global_pos.x + i][it.global_pos.y + j] = std::make_shared<Level>(it);
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
		//texturables.push_back(std::make_shared<std::vector<Texturable>>(level_placement[pos.x][pos.y]->texturables));
		//drawables.push_back(std::make_shared<std::vector<Renderable>>(level_placement[pos.x][pos.y]->drawables));
		level_placement[pos.x][pos.y]->is_loaded = true;
		loaded_levels.push_back(level_placement[pos.x][pos.y]);
	}
}

void Map::unload_level(Vectori pos)
{
	if (level_placement[pos.x][pos.y]->is_loaded)
	{
		//texturables.remove_if([=](const std::shared_ptr<std::vector<Texturable>>& a) {return &*a == &level_placement[pos.x][pos.y]->texturables; });
		//drawables.remove_if([=](const std::shared_ptr<std::vector<Renderable>>& a) {return &*a == &level_placement[pos.x][pos.y]->drawables; });
		level_placement[pos.x][pos.y]->is_loaded = false;
		auto x = &*(level_placement[pos.x][pos.y]);
		loaded_levels.remove_if([=](const std::shared_ptr<Level>& a) {return &*a == (const Level*)&x; });
	}
}

void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	/*
	for (const auto& it : drawables)
	{
		for (const auto& it2 : *it)
		{
			target.draw(it2, states);
		}
	}
	for (const auto& it : texturables)
	{
		for (const auto& it2 : *it)
		{
			target.draw(it2, states);
		}
	}
	*/
	for(const auto& it : loaded_levels)
	{
		states.transform*=sf::Transform().translate({level_size.x*it->global_pos.x,level_size.y*it->global_pos.y});
		for(const auto& it2: it->drawables)
		{
			target.draw(it2, states);
		}
		for (const auto& it2 : it->texturables)
		{
			target.draw(it2, states);
		}
		states.transform*=sf::Transform().translate({-1*level_size.x*it->global_pos.x,-1*level_size.y*it->global_pos.y});
	}
}


void Map::update()
{
}


