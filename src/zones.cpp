#include "zones.h"

Zone::Zone(const std::vector<Vectorf>& vert, Vectorf pos_) : pos(pos_),
collision(vert, pos_), buffer(sf::TriangleFan, sf::VertexBuffer::Static)
{
	std::vector<sf::Vertex> vertices;
	for (const auto& it : vert)
	{
		vertices.emplace_back(it + pos, sf::Color(255, 255, 255, 200));
	}
	buffer.create(vertices.size());
	buffer.update(vertices.data());
}

sf::FloatRect Zone::get_bounding_rect() const
{
	return collision.rect;
}

void Zone::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(buffer, states);
}

Damage_zone::Damage_zone(std::vector<Vectorf>& vert, Vectorf p,
	std::vector<std::pair<int, float>>& dmg) : Zone(vert, p), damage(dmg)
{
	current_damage = damage.begin();
}

void Damage_zone::interact(Entity& entity)
{
	if (get_bounding_rect().intersects(entity.get_collision()->rect))
	{
		if (coll::test_bollean(*entity.get_collision(), collision))
		{
			bool present = !contained.insert(&entity).second;
			if (changed_damage || !present)
			{
				entity.deal_damage(current_damage->first);
			}
			return;
		}
	}
	contained.erase(&entity);
}

void Damage_zone::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(buffer, states);
}

void Damage_zone::reset_physics()
{
	current_damage = damage.begin();
}

void Damage_zone::update_physics(float dt)
{
	time += dt;
	changed_damage = false;
	while (time >= current_damage->second)
	{
		time -= current_damage->second;
		current_damage = util::increment_iterator(current_damage, damage);
		changed_damage = true;
	}
}

Moving_damage_zone::Moving_damage_zone(std::vector<Vectorf>& vert, Vectorf p,
	std::vector<std::pair<int, float>>& dmg, std::unique_ptr<Simple_AI>&& ai_) :
	Damage_zone(vert, p, dmg), ai(std::move(ai_))
{
	base_rect = collision.rect;
	base_mesh = collision.mesh;
}

void Moving_damage_zone::update_physics(float dt)
{
	ai->calc_pos(dt);
	Damage_zone::update_physics(dt);
	sf::Transform new_pos = ai->get_pos();
	for (int i = 0; i < base_mesh.size(); i++)
	{
		collision.mesh[i] = new_pos * base_mesh[i];
	}
	collision.rect = new_pos.transformRect(base_rect);
	collision.calculate_min_max_arr();
}

void Moving_damage_zone::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= ai->get_pos();
	Damage_zone::draw(target, states);
}

void Moving_damage_zone::reset_physics()
{
	ai->reset();
	Damage_zone::reset_physics();
}

Event_zone::Event_zone(const std::vector<Vectorf>& vert, Vectorf p,
	std::vector<int>&& events_, bool player_only_) : Zone(vert, p),
	events(events_), player_only(player_only_) {}

void Event_zone::interact(Entity& entity)
{
	if (player_only && entity.id.get_type() != Message_sender_type::PLAYER)
	{
		return;
	}
	if (get_bounding_rect().intersects(entity.get_collision()->rect))
	{
		if (coll::test_bollean(*entity.get_collision(), collision))
		{
			bool present = !contained.insert(&entity).second;
			if (!present)
			{
				send_next_time = true;
			}
			return;
		}
	}
	contained.erase(&entity);
}

std::vector<int> Event_zone::get_events()
{
	if (send_next_time)
	{
		send_next_time = false;
		return events;
	}
	return std::vector<int>();
}

Clickable_zone::Clickable_zone(const std::vector<Vectorf>& vert, Vectorf p, std::vector<int>&& events_)
	: Event_zone(vert, p, std::move(events_), false) {}

void Clickable_zone::interact(Entity& entity)
{
	if (entity.id.get_type() != Message_sender_type::PLAYER)
	{
		return;
	}
	if (get_bounding_rect().intersects(entity.get_collision()->rect))
	{
		if (coll::test_bollean(*entity.get_collision(), collision))
		{
			player_inside = true;
			return;
		}
	}
	player_inside = false;
}

void Clickable_zone::clicked(Vectori mouse_pos, int button)
{
	if (player_inside && util::contained_in_polygon(static_cast<Vectorf>(mouse_pos), 1000000.f, collision.mesh))
	{
		send_next_time = true;
	}
}
