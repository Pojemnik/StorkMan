#include "worldparts.h"

const Vectorf fliptab[] = { {1,1},{-1,1},{1,-1},{-1,-1} };

Object::Object(Vectorf pos_, const sf::Texture* const texture_, float height_,
	int flip_, float angle_, sf::Color color, float parallax_) : pos(pos_),
	height(height_), flip(flip_), sprite(*texture_), parallax(parallax_)
{
	float scale = height * context.global_scale / texture_->getSize().y;
	sprite.setScale(fliptab[flip] * scale);
	sprite.setRotation(angle_);
	sprite.setPosition(pos_);
	sprite.setColor(color);
}

void Object::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	auto matrix = states.transform.getMatrix();
	float px, py;
	px = matrix[12] * parallax;
	py = matrix[13] * parallax;
	states.transform.translate(px, py);
	target.draw(sprite, states);
}

sf::FloatRect Object::get_bounding_rect() const
{
	return sprite.getGlobalBounds();
}

Moving_object::Moving_object(Vectorf pos_, const sf::Texture* texture_, float height_,
	std::unique_ptr<Simple_AI> ai_, int flip_, float angle_, sf::Color color,
	float parallax_) : Object({ 0,0 }, texture_, height_, flip_, angle_, color, parallax_),
	ai(std::move(ai_))
{
	pos = pos_;
}

void Moving_object::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	auto matrix = states.transform.getMatrix();
	float px, py;
	px = matrix[12] * parallax;
	py = matrix[13] * parallax;
	states.transform.translate(px, py);
	states.transform.translate(pos);
	states.transform *= ai->get_pos();
	target.draw(sprite, states);
}

sf::FloatRect Moving_object::get_bounding_rect() const
{
	sf::FloatRect rect = sprite.getGlobalBounds();
	rect.left += pos.x;
	rect.top += pos.y;
	return rect;
}

void Moving_object::reset_graphics()
{
	ai->reset();
}

void Moving_object::update_graphics(float dt)
{
	ai->calc_pos(dt);
}

Animated_object::Animated_object(Vectorf pos_, std::unique_ptr<Animation>&& animation_,
	float height_, int flip_, float angle_, sf::Color color, float parallax_) :
	Object(pos_, animation_->get_texture(), height_, flip_, angle_, color, parallax_),
	animation(std::move(animation_))
{
	update_frame();
}

void Animated_object::update_frame()
{
	sprite.setTexture(*animation->get_texture());
}

void Animated_object::next_frame(float dt)
{
	animation->next_frame(dt);
}

void Animated_object::update_graphics(float dt)
{
	next_frame(dt);
	update_frame();
}

void Animated_object::reset_graphics()
{
	animation->reset();
	update_frame();
}

Moving_animated_object::Moving_animated_object(Vectorf pos_,
	std::unique_ptr<Animation> animation_, float height_,
	std::unique_ptr<Simple_AI> ai_, int flip_, float angle_, sf::Color color,
	float parallax_) : Animated_object({ 0,0 }, std::move(animation_), height_, flip_, angle_, color, parallax_),
	ai(std::move(ai_))
{
	pos = pos_;
}

void Moving_animated_object::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	auto matrix = states.transform.getMatrix();
	float px, py;
	px = matrix[12] * parallax;
	py = matrix[13] * parallax;
	states.transform.translate(px, py);
	states.transform.translate(pos);
	states.transform *= ai->get_pos();
	target.draw(sprite, states);
}

sf::FloatRect Moving_animated_object::get_bounding_rect() const
{
	sf::FloatRect rect = sprite.getGlobalBounds();
	rect.left += pos.x;
	rect.top += pos.y;
	return rect;
}

void Moving_animated_object::reset_graphics()
{
	Animated_object::reset_graphics();
	ai->reset();
}

void Moving_animated_object::update_graphics(float dt)
{
	Animated_object::update_graphics(dt);
	ai->calc_pos(dt);
}
