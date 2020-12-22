#include "level.h"
#include "zones.h"

Level::Level(std::vector<std::unique_ptr<Chunk>>&& chunks_,
	std::vector<Map_sound>&& sounds_, Vectori pos, int code_) : chunks(std::move(chunks_)),
	sounds(sounds_), global_pos(pos), code(code_),
	sound_borders(sf::PrimitiveType::Lines, sf::VertexBuffer::Usage::Static),
	Message_sender(Message_sender_type::LEVEL)
{
	std::vector<Vectorf> vertices;
	std::vector<std::pair<Vectorf, Vectorf>> edges;
	for (auto& it : chunks)
	{
		auto v = it->get_chunk_vertices();
		if (v.first.size() != 0)
		{
			vertices.insert(vertices.end(), v.first.begin(), v.first.end());
			edges.insert(edges.end(), v.second.begin(), v.second.end());
		}
		add_receiver(&*it);
	}
	static util::Color_generator colors("data/colors.txt");
	std::vector<sf::Vertex> sound_borders_vect;
	for (auto& it : sounds)
	{
		if (!it.is_initialized())
		{
			it.update_collision(edges, vertices);
		}
		sf::Color color = colors.get_color();
		auto mesh = it.get_collision().mesh;
		sound_borders_vect.push_back(sf::Vertex(mesh[0], color));
		for (int i = 1; i < mesh.size(); i++)
		{
			sound_borders_vect.push_back(sf::Vertex(mesh[i], color));
			sound_borders_vect.push_back(sf::Vertex(mesh[i], color));
		}
		sound_borders_vect.push_back(sf::Vertex(mesh[0], color));
		sound_sources.push_back(sf::CircleShape(5));
		sound_sources.back().setFillColor(color);
		sound_sources.back().setPosition(it.get_pos() - Vectorf(5, 5));
	}
	sound_borders.create(sound_borders_vect.size());
	sound_borders.update(sound_borders_vect.data());
	chunks_on_screen.resize(chunks.size());
}

void Level::update_graphics(float dt, sf::FloatRect screen_rect)
{
	int i = 0;
	for (auto& it : chunks)
	{
		if (it->get_bounding_rect().intersects(screen_rect))
		{
			chunks_on_screen[i] = true;
			//Maybe check if there is something to update in chunk
			it->update_graphics(dt);
		}
		else
		{
			chunks_on_screen[i] = false;
		}
		i++;
	}
}

void Level::update_physics(float dt, sf::FloatRect screen_rect, std::vector<int>& msg_up)
{
	int i = 0;
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
	for (auto& it : chunks)
	{
		if (it->get_bounding_rect().intersects(screen_rect))
		{
			chunks_on_screen[i] = true;
			//Maybe check if there is something to update in chunk
			it->update_physics(dt, msg_up);
		}
		else
		{
			chunks_on_screen[i] = false;
		}
		i++;
	}
}

void Level::draw_bottom_layers(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (int i = 0; i < BOTTOM_LAYERS; i++)
	{
		int j = 0;
		for (const auto& it : chunks)
		{
			if (chunks_on_screen[j])
			{
				it->draw_layer(target, states, i);
			}
			j++;
		}
	}
}

void Level::draw_middle_layers(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (int i = BOTTOM_LAYERS; i < BOTTOM_LAYERS + MIDDLE_LAYERS; i++)
	{
		int j = 0;
		for (const auto& it : chunks)
		{
			if (chunks_on_screen[j])
			{
				it->draw_layer(target, states, i);
			}
			j++;
		}
		if (draw_sound_sources)
		{
			for (const auto& it : sound_sources)
			{
				target.draw(it, states);
			}
			target.draw(sound_borders, states);
		}
	}
}

void Level::draw_top_layers(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (int i = BOTTOM_LAYERS + MIDDLE_LAYERS; i < TOTAL_LAYERS; i++)
	{
		int j = 0;
		for (const auto& it : chunks)
		{
			if (chunks_on_screen[j])
			{
				it->draw_layer(target, states, i);
			}
			j++;
		}
	}
	for (const auto& it : chunks)
	{
		if (draw_chunks_borders)
		{
			it->draw_border(target, states);
		}
	}
}

void Level::resolve_collisions(std::vector<Entity*>& entities)
{
	for (auto& it : entities)
	{
		for (const auto& chunk_it : chunks)
		{
			if (it->get_collision()->rect.intersects(chunk_it->get_bounding_rect()))
			{
				chunk_it->resolve_collisions(*it);
			}
		}
	}
}

void Level::set_draw_border(bool draw)
{
	draw_border = draw;
}

void Level::set_draw_chunks_borders(bool draw)
{
	draw_chunks_borders = draw;
}

void Level::set_draw_sound_sources(bool draw)
{
	draw_sound_sources = draw;
}

void Level::make_zones_interactions(std::vector<Entity*>& entities)
{
	for (auto& it : entities)
	{
		for (const auto& chunk_it : chunks)
		{
			chunk_it->make_zones_interactions(*it);
		}
	}
}

void Level::draw_moving_collisions(sf::RenderTarget& target, sf::RenderStates states) const
{
	int i = 0;
	for (const auto& it : chunks)
	{
		if (chunks_on_screen[i])
		{
			it->draw_moving_collisions(target, states);
		}
		i++;
	}
}

void Level::draw_static_collisions(sf::RenderTarget& target, sf::RenderStates states) const
{
	int i = 0;
	for (const auto& it : chunks)
	{
		if (chunks_on_screen[i])
		{
			it->draw_static_collisions(target, states);
		}
		i++;
	}
}

void Level::draw_zones(sf::RenderTarget& target, sf::RenderStates states) const
{
	int i = 0;
	for (const auto& it : chunks)
	{
		if (chunks_on_screen[i])
		{
			it->draw_zones(target, states);
		}
		i++;
	}
	//Add, when moving damage zones are added
	//for (const auto& it : moving)
	//{
	//	it.draw_zones(target, states);
	//}
}

std::unordered_set<const Map_sound*, std::hash<const Map_sound*>, Map_sound_compare>
Level::get_current_map_sounds(Vectorf player_pos) const
{
	std::unordered_set<const Map_sound*, std::hash<const Map_sound*>, Map_sound_compare>
		current_sounds;
	sf::FloatRect player_rect(player_pos, { 1.f,1.f });
	std::vector<Vectorf> player_mesh = { Vectorf(0,0), Vectorf(0,1),
		Vectorf(1,1), Vectorf(1,0) };
	Collision player_col(player_mesh, player_pos);
	for (const auto& it : sounds)
	{
		Collision sound_col = it.get_collision();
		if (player_rect.intersects(sound_col.rect))
		{
			if (util::contained_in_polygon(player_pos, it.get_max_x() + 1.f, sound_col.mesh))
			{
				current_sounds.insert(&it);
			}
		}
	}
	return current_sounds;
}

Vectori Level::get_global_pos() const
{
	return global_pos;
}
