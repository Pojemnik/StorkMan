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
			bool present = contained.insert(&entity).second;
			if (changed_damage || present)
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

void Damage_zone::update(float dt)
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

void Moving_damage_zone::update(float dt)
{
	ai->calc_pos(dt);
	Damage_zone::update(dt);
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
