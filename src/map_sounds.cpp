#include "map_sounds.h"

Map_sound::Map_sound(std::vector<Vectorf>&& mesh, Map_sound_info info_)
	: collision(mesh, info_.pos), info(info_) {}

void Map_sound::update_collision(std::vector<std::pair<Vectorf, Vectorf>>& map_edges, std::vector<Vectorf>& map_vertices)
{
	collision.mesh.clear();
	const auto polygon =  Polygon_generator::calc_polygon(info.pos, 1, map_edges, map_vertices);
	collision.mesh = polygon;
	collision.rect = util::mesh_to_rect(collision.mesh);
	collision.calculate_min_max_arr();
}

Collision Map_sound::get_collision() const
{
	return collision;
}

int Map_sound::get_sound() const
{
	return info.sound;
}

int Map_sound::get_id() const
{
	return info.id;
}

Vectorf Map_sound::get_pos() const
{
	return info.pos;
}

int Map_sound::get_default_volume() const
{
	return info.volume;
}

Map_sound_info Map_sound::get_info() const
{
	return info;
}
