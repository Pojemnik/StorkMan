#include "worldparts.h"

const Vectorf fliptab[] = { {1,1},{-1,1},{1,-1},{-1,-1} };

Object::Object(Vectorf pos_, const sf::Texture* const texture_, float height_,
	int flip_, float angle_) : pos(pos_), height(height_), flip(flip_), sprite(*texture_)
{
	float scale = height * context.global_scale / texture_->getSize().y;
	sprite.setScale(fliptab[flip] * scale);
	sprite.setRotation(angle_);
	sprite.setPosition(pos);
}

void Object::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(sprite, states);
}

sf::FloatRect Object::get_bounding_rect() const
{
	return sprite.getGlobalBounds();
}

Moving_object::Moving_object(Vectorf pos_, const sf::Texture* texture_, float height_,
	std::unique_ptr<Simple_AI> ai_, int flip_, float angle_) :
	Object(pos_, texture_, height_, flip_, angle_), ai(std::move(ai_)) {}

void Moving_object::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= ai->get_pos();
	Object::draw(target, states);
}

void Moving_object::update(float dt)
{
	ai->calc_pos(dt);
}

Animated_object::Animated_object(Vectorf pos_, std::unique_ptr<Animation>&& animation_,
	float height_, int flip_, float angle_) :
	Object(pos_, animation_->get_texture(), height_, flip_, angle_), animation(std::move(animation_))
{}

void Animated_object::update_frame()
{
	sprite.setTexture(*animation->get_texture());
}

void Animated_object::next_frame(float dt)
{
	animation->next_frame(dt);
}

void Animated_object::update(float dt)
{
	next_frame(dt);
	update_frame();
}

Moving_animated_object::Moving_animated_object(Vectorf pos_,
	std::unique_ptr<Animation> animation_, float height_,
	std::unique_ptr<Simple_AI> ai_, int flip_, float angle_) :
	Animated_object(pos_, std::move(animation_), height_, flip_, angle_), ai(std::move(ai_))
{}

void Moving_animated_object::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= ai->get_pos();
	Object::draw(target, states);
}

void Moving_animated_object::update(float dt)
{
	Animated_object::update(dt);
	ai->calc_pos(dt);
}

Zone::Zone(std::vector<Vectorf>& vert, Vectorf pos_) : pos(pos_),
	collision(std::move(vert), pos_) {}

sf::FloatRect Zone::get_bounding_rect() const
{
	return collision.rect;
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