#include "level.h"

void Tile::update(float dt)
{
	for (auto& it : updatables)
	{
		it->update(dt);
	}
	for (auto& it : animatables)
	{
		it->next_frame(dt);
	}
}

void Tile::draw_bottom_layers(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const auto& it : bottom_layers)
	{
		for (const auto& it2 : it)
		{
			target.draw(*it2, states);
		}
	}
}

void Tile::draw_middle_layers(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const auto& it : middle_layers)
	{
		for (const auto& it2 : it)
		{
			target.draw(*it2, states);
		}
	}
}

void Tile::draw_top_layers(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const auto& it : top_layers)
	{
		for (const auto& it2 : it)
		{
			target.draw(*it2, states);
		}
	}
}

Update_chunk::Update_chunk(std::vector<std::shared_ptr<Updatable>>&& content_) :
	content(std::move(content_)) {}

void Update_chunk::update(float dt)
{
	for (auto& it : content)
	{
		it->update(dt);
	}
}

sf::FloatRect Moving_element::get_bounding_rect() const
{
	return updatable->get_bounding_rect();
}

void Moving_element::update(float dt)
{
	updatable->update(dt);
}

void Moving_element::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (is_drawable)
	{
		target.draw(*renderable, states);
	}
}

Moving_element::Moving_element(std::shared_ptr<Updatable> updatable_, int layer_) :
	updatable(std::move(updatable_)), layer(layer_)
{
	renderable = std::dynamic_pointer_cast<Renderable>(updatable);
	is_drawable = (renderable == nullptr);
}

Level::Level(std::vector<std::vector<Tile>>&& tiles_,
	std::vector<Moving_element>&& moving_, Vectori pos,
	Vectori size) : tiles(std::move(tiles_)), moving(std::move(moving)),
	global_pos(pos), global_size(size) {}

void Level::update(float dt, Vectorf player_pos, sf::FloatRect screen_rect)
{
	player_tile = { (int)player_pos.x % context.level_tile_size,
	(int)player_pos.y % context.level_tile_size };
	for (int i = -3; i < 4; i++)
	{
		for (int j = -2; j < 3; j++)
		{
			if (player_tile.x + i >= tiles_n.x || player_tile.x + i < 0 ||
				player_tile.y + j >= tiles_n.y || player_tile.y + j < 0)
			{
				continue;
			}
			tiles[player_tile.x + i][player_tile.y + j].update(dt);
		}
	}
	for (auto& it : moving)
	{
		if (it.get_bounding_rect().intersects(screen_rect))
		{
			it.on_screen = true;
			it.update(dt);
		}
		else
		{
			it.on_screen = false;
		}
	}
}

void Level::draw_bottom_layers(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (int i = -3; i < 4; i++)
	{
		for (int j = -2; j < 3; j++)
		{
			if (player_tile.x + i >= tiles_n.x || player_tile.x + i < 0 ||
				player_tile.y + j >= tiles_n.y || player_tile.y + j < 0)
			{
				continue;
			}
			tiles[player_tile.x + i][player_tile.y + j].draw_bottom_layers(target, states);
		}
	}
}

void Level::draw_middle_layers(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (int i = -3; i < 4; i++)
	{
		for (int j = -2; j < 3; j++)
		{
			if (player_tile.x + i >= tiles_n.x || player_tile.x + i < 0 ||
				player_tile.y + j >= tiles_n.y || player_tile.y + j < 0)
			{
				continue;
			}
			tiles[player_tile.x + i][player_tile.y + j].draw_middle_layers(target, states);
		}
	}
}

void Level::draw_top_layers(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (int i = -3; i < 4; i++)
	{
		for (int j = -2; j < 3; j++)
		{
			if (player_tile.x + i >= tiles_n.x || player_tile.x + i < 0 ||
				player_tile.y + j >= tiles_n.y || player_tile.y + j < 0)
			{
				continue;
			}
			tiles[player_tile.x + i][player_tile.y + j].draw_top_layers(target, states);
		}
	}
}