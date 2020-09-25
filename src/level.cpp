#include "level.h"

void Map_chunk::draw_bottom_layers(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (!on_screen)
	{
		return;
	}
	for (const auto& it : bottom_layers)
	{
		for (const auto& it2 : it)
		{
			target.draw(*it2, states);
		}
	}
}

void Map_chunk::draw_middle_layers(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (!on_screen)
	{
		return;
	}
	for (const auto& it : middle_layers)
	{
		for (const auto& it2 : it)
		{
			target.draw(*it2, states);
		}
	}
}

void Map_chunk::draw_top_layers(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (!on_screen)
	{
		return;
	}
	for (const auto& it : top_layers)
	{
		for (const auto& it2 : it)
		{
			target.draw(*it2, states);
		}
	}
}

Map_chunk::Map_chunk(std::vector<std::shared_ptr<Updatable>>&& updatables_,
	std::vector<std::pair<int, std::shared_ptr<Renderable>>>&& drawables_,
	sf::FloatRect bound_)
	: updatables(std::move(updatables_)), bound(bound_)
{
	for (auto& it : drawables_)
	{
		if (it.first < BOTTOM_LAYERS)
		{
			bottom_layers[it.first].push_back(it.second);
			continue;
		}
		if (it.first < BOTTOM_LAYERS + MIDDLE_LAYERS)
		{
			middle_layers[it.first - BOTTOM_LAYERS].push_back(it.second);
			continue;
		}
		top_layers[it.first - BOTTOM_LAYERS - MIDDLE_LAYERS].push_back(it.second);
	}
}

void Map_chunk::update(float dt)
{
	for (auto& it : updatables)
	{
		it->update(dt);
	}
}

sf::FloatRect Map_chunk::get_bounding_rect() const
{
	return bound;
}

sf::FloatRect Moving_element::get_bounding_rect() const
{
	return object->get_bounding_rect();
}

void Moving_element::update(float dt)
{
	updatable->update(dt);
}

void Moving_element::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (is_drawable && on_screen)
	{
		target.draw(*renderable, states);
	}
}

Moving_element::Moving_element(std::shared_ptr<Updatable> updatable_, int layer_) :
	updatable(std::move(updatable_)), layer(layer_)
{
	renderable = std::dynamic_pointer_cast<Renderable>(updatable);
	is_drawable = (renderable == nullptr);
	object = std::dynamic_pointer_cast<Map_object>(updatable);
}

Level::Level(std::vector<Map_chunk>&& chunks_,
	std::vector<Moving_element>&& moving_, Vectori pos)
	: chunks(std::move(chunks_)), moving(std::move(moving)),
	global_pos(pos) {}

void Level::update(float dt, Vectorf player_pos, sf::FloatRect screen_rect)
{
	for (auto& it : chunks)
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
	for (const auto& it : chunks)
	{
		it.draw_bottom_layers(target, states);
	}
	for (const auto& it : moving)
	{
		if (it.layer < BOTTOM_LAYERS)
		{
			target.draw(it);
		}
	}
}

void Level::draw_middle_layers(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const auto& it : chunks)
	{
		it.draw_middle_layers(target, states);
	}
	for (const auto& it : moving)
	{
		if (it.layer < BOTTOM_LAYERS + MIDDLE_LAYERS && it.layer >= BOTTOM_LAYERS)
		{
			target.draw(it);
		}
	}
}

void Level::draw_top_layers(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const auto& it : chunks)
	{
		it.draw_top_layers(target, states);
	}
	for (const auto& it : moving)
	{
		if (it.layer < BOTTOM_LAYERS + MIDDLE_LAYERS + TOP_LAYERS &&
			it.layer >= BOTTOM_LAYERS + MIDDLE_LAYERS)
		{
			target.draw(it);
		}
	}
}