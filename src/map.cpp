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

void Map::init_receivers()
{
	for (int i = 0; i < 5; i++)
	{
		players_sound_receivers.push_back(sf::RectangleShape(Vectorf(1, 1)));
		players_sound_receivers[i].setFillColor(sf::Color::Magenta);
		receivers_pos.push_back(Vectorf());
	}
}

void Map::set_receivers_pos(Vectorf pos)
{
	Vectorf tab[5] = { {-48, -48}, {48, -48}, {0, 0}, {-48, 48}, {48, 48} };
	for (int i = 0; i < 5; i++)
	{
		players_sound_receivers[i].setPosition(pos + tab[i]);
		receivers_pos[i] = pos + tab[i];
	}
}

void Map::draw_receivers(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const auto& it : players_sound_receivers)
	{
		target.draw(it, states);
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
	init_receivers();
}

void Map::add_level(std::unique_ptr<Level>&& lvl)
{
	Vectori pos = lvl->get_global_pos();
	try
	{
		levels.at(pos.x).at(pos.y) = std::move(lvl);
		add_receiver(&*levels.at(pos.x).at(pos.y));
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
	if (draw_players_receiver)
	{
		draw_receivers(target, states);
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

void Map::update_physics(float dt, Vectorf player_pos, sf::FloatRect screen_rect)
{
	Vectori player_pos_on_map = Vectori(
		int(player_pos.x / context.global_scale / context.level_size.x),
		int(player_pos.y / context.global_scale / context.level_size.y));
	bool out_of_map = false;
	Level* current_level = &*levels.at(current_pos.x).at(current_pos.y);
	if (player_pos_on_map != current_pos)
	{
		current_pos = player_pos_on_map;
		try
		{
			send_message<int>(Message::Message_type::CHANGED_LEVEL, current_level->code);
		}
		catch (const std::out_of_range&)
		{
			send_message<string>(Message::Message_type::ERROR, "Level out of range");
			out_of_map = true;
		}
	}
	get_considered_levels();
	while (message_available())
	{
		Message msg = pop_message();
		if (msg.type == Message::Message_type::MAP_EVENT)
		{
			send_message(Message::Message_type::MAP_EVENT, msg.args);
		}
		if (msg.type == Message::Message_type::MOUSE_CLICKED)
		{
			send_message(Message::Message_type::MOUSE_CLICKED, msg.args);
		}
	}
	for (auto& it : considered_levels)
	{
		it->update_physics(dt, screen_rect, msg_upstream);
		it->resolve_collisions(entities);
		it->make_zones_interactions(entities);
	}
	for (const auto& it : msg_upstream)
	{
		send_message<int>(Message::Message_type::MAP_EVENT, it);
	}
	msg_upstream.clear();
	if (!out_of_map)
	{
		Vectorf sound_player_pos = player_pos;
		sound_player_pos -= Vectorf(0.35f, 1.05f) * context.global_scale;
		set_receivers_pos(sound_player_pos);
		auto current_map_sounds = current_level->get_current_map_sounds(receivers_pos);
		for (const auto& it : current_map_sounds)
		{
			auto info = it.first->get_info();
			float volume = static_cast<float>(it.second) / 5.f;
			info.volume *= volume;
			//Enetred sound
			if (!last_map_sounds.contains(it.first))
			{
				send_message(Message::Message_type::ENTERED_SOUND, info);
			}
			else
			{
				//Volume changed
				if (last_map_sounds.at(it.first) != it.second)
				{
					send_message(Message::Message_type::ENTERED_SOUND, info);
				}
			}
		}
		for (const auto& it : last_map_sounds)
		{
			if (!current_map_sounds.contains(it.first))
			{
				send_message(Message::Message_type::LEFT_SOUND, it.first->get_info());
			}
		}
		last_map_sounds = current_map_sounds;
	}
}

void Map::update_graphics(float dt, sf::FloatRect screen_rect)
{
	for (auto& it : considered_levels)
	{
		it->update_graphics(dt, screen_rect);
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

void Map::set_draw_sound_sources(bool draw)
{
	for (auto& it : levels)
	{
		for (auto& it2 : it)
		{
			it2->set_draw_sound_sources(draw);
		}
	}
	draw_players_receiver = draw;
}