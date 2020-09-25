#include "map.h"

void Map::update_levels(float dt, sf::FloatRect screen_rect)
{
	call_on_considered_levels(std::bind(&Level::update, std::placeholders::_1, dt, screen_rect));
}

void Map::resolve_collisions()
{
	call_on_considered_levels(std::bind(&Level::resolve_collisions, std::placeholders::_1, physicals));
}

void Map::call_on_considered_levels(std::function<void(Level&)> foo)
{
	for (int i = -1; i < 2; i++)
	{
		int y = current_pos.y + i;
		if (y < 0 || y >= size.y)
		{
			continue;
		}
		for (int j = -1; j < 2; j++)
		{
			int x = current_pos.x + j;
			if (x < 0 || x >= size.x)
			{
				continue;
			}
			foo(levels[x][y]);
		}
	}
}

void Map::call_on_considered_levels(std::function<void(const Level&)> foo) const
{
	for (int i = -1; i < 2; i++)
	{
		int y = current_pos.y + i;
		if (y < 0 || y >= size.y)
		{
			continue;
		}
		for (int j = -1; j < 2; j++)
		{
			int x = current_pos.x + j;
			if (x < 0 || x >= size.x)
			{
				continue;
			}
			foo(levels[x][y]);
		}
	}
}

Map::Map(Vectori size_, Vectori pos) : size(size_), current_pos(pos) {}

void Map::add_level(Level&& lvl, Vectori pos)
{
	try
	{
		levels.at(pos.x).at(pos.y) = lvl;
	}
	catch(std::out_of_range e)
	{
		std::cout << "Incorrect level position in map" << std::endl;
		throw std::invalid_argument("Invalid level position");
	}
}

void Map::draw_bottom_layers(sf::RenderTarget& target, sf::RenderStates states) const
{
	call_on_considered_levels(std::bind(&Level::draw_bottom_layers, std::placeholders::_1, target, states));
}

void Map::draw_middle_layers(sf::RenderTarget& target, sf::RenderStates states) const
{
	call_on_considered_levels(std::bind(&Level::draw_middle_layers, std::placeholders::_1, target, states));
}

void Map::draw_top_layers(sf::RenderTarget& target, sf::RenderStates states) const
{
	call_on_considered_levels(std::bind(&Level::draw_top_layers, std::placeholders::_1, target, states));
}

void Map::update(float dt, Vectorf player_pos, sf::FloatRect screen_rect)
{
	Vectori player_pos_on_map = { player_pos.x / context.level_size.x,
		player_pos.y / context.level_size.y };
	if (player_pos_on_map != current_pos)
	{
		current_pos = player_pos_on_map;
	}
	update_levels(dt, screen_rect);
	resolve_collisions();
	//TODO: Dealing damage from enviroment
}
