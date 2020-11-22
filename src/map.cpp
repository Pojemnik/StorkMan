#include "map.h"

void Map::init()
{
	send_message<int>(Message::Message_type::CHANGED_LEVEL, levels[current_pos.x][current_pos.y]->code);
}

void Map::get_considered_levels()
{
	considered_levels.clear();
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
			considered_levels.push_back(&*levels[x][y]);
		}
	}
}

Map::Map(Vectori size_, Vectori pos, const sf::Texture* bg_tex) : size(size_),
current_pos(pos), background(*bg_tex), Message_sender(Message_sender_type::MAP)
{
	background.setPosition(-1000, -2500);
	levels.resize(size.x);
	for (auto& it : levels)
	{
		it.resize(size.y);
	}
}

void Map::add_level(std::unique_ptr<Level>&& lvl)
{
	Vectori pos = lvl->get_global_pos();
	try
	{
		levels.at(pos.x).at(pos.y) = std::move(lvl);
	}
	catch (std::out_of_range e)
	{
		std::cout << "Incorrect level position in map" << std::endl;
		throw std::invalid_argument("Invalid level position");
	}
}

void Map::draw_bottom_layers(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(background);
	for (auto& it : considered_levels)
	{
		it->draw_bottom_layers(target, states);
	}
}

void Map::draw_middle_layers(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (auto& it : considered_levels)
	{
		it->draw_middle_layers(target, states);
	}
}

void Map::draw_top_layers(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (auto& it : considered_levels)
	{
		it->draw_top_layers(target, states);
	}
}

void Map::draw_vertices(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (auto& it : considered_levels)
	{
		it->draw_moving_collisions(target, states);
		it->draw_static_collisions(target, states);
	}
}

void Map::draw_zones(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (auto& it : considered_levels)
	{
		it->draw_zones(target, states);
	}
}

void Map::update(float dt, Vectorf player_pos, sf::FloatRect screen_rect)
{
	Vectori player_pos_on_map = Vectori(
		int(player_pos.x / context.global_scale / context.level_size.x),
		int(player_pos.y / context.global_scale / context.level_size.y));
	bool out_of_map = false;
	if (player_pos_on_map != current_pos)
	{
		current_pos = player_pos_on_map;
		try
		{
			send_message<int>(Message::Message_type::CHANGED_LEVEL, levels.at(current_pos.x).at(current_pos.y)->code);
		}
		catch (const std::out_of_range& e)
		{
			send_message<string>(Message::Message_type::ERROR, "Level out of range");
			out_of_map = true;
		}
		last_map_sounds.clear();
	}
	get_considered_levels();
	for (auto& it : considered_levels)
	{
		it->update(dt, screen_rect);
		it->resolve_collisions(entities);
		it->make_zones_interactions(entities);
	}
	if (!out_of_map)
	{
		auto current_map_sounds = levels.at(current_pos.x).at(current_pos.y)->get_current_map_sounds(player_pos);
		for (const auto& it : current_map_sounds)
		{
			if (!last_map_sounds.contains(it))
			{
				const Vectorf pos = it->get_pos() / context.global_scale;
				send_message(Message::Message_type::ENTERED_SOUND,
					std::make_tuple(it->get_sound(), it->get_id(), pos));
			}
		}
		for (const auto& it : last_map_sounds)
		{
			if (!current_map_sounds.contains(it))
			{
				const Vectorf pos = it->get_pos() / context.global_scale;
				send_message(Message::Message_type::LEFT_SOUND,
					std::make_tuple(it->get_sound(), it->get_id(), pos));
			}
		}
		last_map_sounds = current_map_sounds;
	}
}

void Map::add_entity(Entity* entity)
{
	entities.push_back(entity);
}

void Map::set_draw_chunks_borders(bool draw)
{
	for (auto& it : levels)
	{
		for (auto& it2 : it)
		{
			it2->set_draw_chunks_borders(draw);
		}
	}
}