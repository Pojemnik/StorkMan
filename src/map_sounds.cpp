#include "map_sounds.h"

Map_sound::Map_sound(std::vector<Vectorf>&& mesh, Map_sound_info info_,
	std::unordered_map<int, bool>&& events_)
	: collision(mesh, info_.pos), info(info_), initialized(true),
	events(events_)
{
	for (const auto& it : collision.mesh)
	{
		if (it.x > max_x)
		{
			max_x = it.x;
		}
	}
	if (events.contains(0))
	{
		enabled = events[0];
	}
}

Map_sound::Map_sound(Map_sound_info info_,
	std::unordered_map<int, bool>&& events_) : info(info_),
	collision(sf::FloatRect(info_.pos, { 0,0 })), events(events_)
{
	if (events.contains(0))
	{
		enabled = events[0];
	}
}

void Map_sound::update_collision(std::vector<std::pair<Vectorf, Vectorf>>& map_edges, std::vector<Vectorf>& map_vertices)
{
	collision.mesh.clear();
	const auto polygon = Polygon_generator::calc_polygon(info.pos,
		info.range, map_edges, map_vertices);
	collision.mesh = polygon;
	collision.rect = util::mesh_to_rect(collision.mesh);
	collision.calculate_min_max_arr();
	for (const auto& it : collision.mesh)
	{
		if (it.x > max_x)
		{
			max_x = it.x;
		}
	}
}

Collision Map_sound::get_collision() const
{
	return collision;
}

int Map_sound::get_id() const
{
	return info.id;
}

Vectorf Map_sound::get_pos() const
{
	return info.pos;
}

Map_sound_info Map_sound::get_info() const
{
	return info;
}

bool Map_sound::is_initialized() const
{
	return initialized;
}

float Map_sound::get_max_x() const
{
	return max_x;
}

bool Map_sound::update_and_get_state(std::vector<int> current_events)
{
	for (const auto& it : current_events)
	{
		if (events.contains(it))
		{
			enabled = events[it];
		}
	}
	return enabled;
}
