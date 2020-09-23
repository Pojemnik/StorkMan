#include "worldparts.h"

const Vectorf fliptab[] = { {1,1},{-1,1},{1,-1},{-1,-1} };

Object::Object(Vectorf pos_, const sf::Texture* const texture_, float height_,
	int flip_, float angle_) : pos(pos_), height(height_), flip(flip_), sprite(*texture_)
{
	sprite.setScale(fliptab[flip] * height);
	sprite.setRotation(angle_);
	sprite.setPosition(pos * context.global_scale);
}

void Object::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(sprite, states);
}

Moving_object::Moving_object(Vectorf pos_, const sf::Texture* texture_, float height_,
	std::unique_ptr<Simple_AI> ai_, int flip_ = 0, float angle_ = 0) :
	Object(pos_, texture_, height_, flip_, angle_), ai(std::move(ai_)) {}

void Moving_object::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= ai->get_pos();
	Object::draw(target, states);
}

sf::FloatRect Moving_object::get_bounding_rect() const
{
	return sprite.getGlobalBounds();
}

void Moving_object::update(float dt)
{
	ai->calc_pos(dt);
}

Animated_object::Animated_object(Vectorf pos_, std::unique_ptr<Animation>&& animation_,
	float height_, int flip_, float angle_) :
	Object(pos_, nullptr, height_, flip_, angle_), animation(std::move(animation_))
{}

void Animated_object::update_frame()
{
	sprite.setTexture(*animation->get_texture());
}

void Animated_object::next_frame(float dt)
{
	animation->next_frame(dt);
}

Moving_animated_object::Moving_animated_object(Vectorf pos_,
	std::unique_ptr<Animation> animation_, float height_,
	std::unique_ptr<Simple_AI> ai_, int flip_ = 0,
	float angle_ = 0) :
	Animated_object(pos_, std::move(animation_), height_, flip_, angle_), ai(std::move(ai_))
{}

void Moving_animated_object::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= ai->get_pos();
	Object::draw(target, states);
}

sf::FloatRect Moving_animated_object::get_bounding_rect() const
{
	return sprite.getGlobalBounds();
}

void Moving_animated_object::update(float dt)
{
	ai->calc_pos(dt);
}

Zone::Zone(const std::vector<Vectorf>& vert, Vectorf p) : vertices(vert), pos(p)
{
	static int next_id(0);
	id = next_id++;
	center = { 0,0 };
	max_x = -INFINITY;
	for (auto it : vertices)
	{
		if (it.x > max_x)
			max_x = it.x;
		center += it;
	}
	max_x += pos.x;
	center = { center.x / vertices.size(), center.y / vertices.size() };
}

Zone::Zone(std::vector<Vectorf>& vert, Vectorf p) : Zone((const std::vector<Vectorf>)vert, p)
{}

bool Zone::contains(Vectorf p)
{
	return(util::contained_in_polygon(p - pos, max_x, vertices));
}

Damage_zone::Damage_zone(std::vector<Vectorf>& vert, Vectorf p,
	std::vector<std::pair<int, int>>& dmg) : Zone(vert, p), damage(dmg),
	bounds(util::mesh_to_rect(vert))
{
	current_damage = damage.begin();
}

sf::FloatRect Damage_zone::get_bounding_rect() const
{
	return bounds;
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