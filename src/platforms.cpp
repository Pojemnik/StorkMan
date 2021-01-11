#include "platforms.h"

Textured_polygon::Textured_polygon(Vectorf pos, const std::array<const sf::Texture*, 3>* texture_,
	std::vector<sf::Vertex> points, sf::Color color) : texture(texture_)
{
	for (auto& it : points)
	{
		it.position += pos;
		it.color = color;
	}
	polygon = sf::VertexBuffer(sf::TrianglesFan, sf::VertexBuffer::Static);
	polygon.create(points.size());
	polygon.update(&points[0]);
	bound = util::mesh_to_rect(points);
}

sf::FloatRect Textured_polygon::get_bounding_rect() const
{
	return bound;
}

void Textured_polygon::draw(Gbuffer& target, sf::RenderStates states)
{
	states.texture = texture->at(0);
	target.albedo.draw(polygon, states);
	states.texture = texture->at(1);
	target.normal.draw(polygon, states);
	states.texture = texture->at(2);
	target.position.draw(polygon, states);
}

const Collision* const Platform::get_collision() const
{
	return &collision;
}

Platform::Platform(Vectorf pos_, const std::array<const sf::Texture*, 3>* texture_,
	std::vector<sf::Vertex> points_, int surface_, bool one_sided,
	sf::Color color) :
	collision(points_, pos_, surface_), Textured_polygon(pos_, texture_,
		std::move(std::vector<sf::Vertex>(points_)), color)
{
	collision.one_sided = one_sided;
}

sf::FloatRect Platform::get_bounding_rect() const
{
	return collision.rect;
}

const Collision* const Moving_platform::get_collision() const
{
	return &collision;
}

Moving_platform::Moving_platform(Vectorf pos_, const std::array<const sf::Texture*, 3>* texture_,
	std::vector<sf::Vertex> points_, std::unique_ptr<Simple_AI> ai_,
	int surface_, sf::Color color) : 
	Platform({ 0,0 }, texture_, points_, surface_, false, color),
	ai(std::move(ai_)), vertex(sf::LineStrip, sf::VertexBuffer::Static),
	pos(pos_)
{
	base_rect = collision.rect;
	base_mesh = collision.mesh;
	collision.rect.left += pos.x;
	collision.rect.top += pos.y;
	vertex.create(collision.mesh.size() + 1);
	sf::Vertex* tmp = new sf::Vertex[collision.mesh.size() + 1];
	for (int i = 0; i < collision.mesh.size(); i++)
	{
		collision.mesh[i] += pos;
		tmp[i] = sf::Vertex(base_mesh[i], sf::Color::White);
	}
	tmp[base_mesh.size()] = sf::Vertex(base_mesh[0], sf::Color::White);
	vertex.update(tmp);
}

void Moving_platform::update_physics(float dt)
{
	ai->calc_pos(dt);
	sf::Transform new_pos = sf::Transform().translate(pos)*ai->get_pos();
	Vectorf old_pos = { collision.rect.left, collision.rect.top };
	for (int i = 0; i < base_mesh.size(); i++)
	{
		collision.mesh[i] = new_pos * base_mesh[i];
	}
	collision.rect = new_pos.transformRect(base_rect);
	collision.calculate_min_max_arr();
	speed = (Vectorf(collision.rect.left, collision.rect.top) - old_pos) / dt;
}

void Moving_platform::draw(Gbuffer& target, sf::RenderStates states)
{
	states.transform.translate(pos);
	states.transform *= ai->get_pos();
	Platform::draw(target, states);
}

void Moving_platform::draw_dynamic_collision(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform.translate(pos);
	states.transform *= ai->get_pos();
	target.draw(vertex, states);
}

Vectorf Moving_platform::get_speed() const
{
	return speed;
}

void Moving_platform::reset_physics()
{
	ai->reset();
	update_physics(0.f);
}

Animated_polygon::Animated_polygon(Vectorf pos,
	std::unique_ptr<Animation>&& animation_, std::vector<sf::Vertex> points,
	sf::Color color) : animation(std::move(animation_)),
	Textured_polygon(pos, nullptr, points, color)
{
	update_graphics(.0f);
}

void Animated_polygon::update_frame()
{
	texture = animation->get_texture();
}

void Animated_polygon::next_frame(float dt)
{
	animation->next_frame(dt);
}

void Animated_polygon::draw(Gbuffer& target, sf::RenderStates states)
{
	for (int i = 0; i < 3; i++)
	{
		states.texture = texture->at(i);
		target[i].draw(polygon, states);
	}
}

sf::FloatRect Animated_polygon::get_bounding_rect() const
{
	return bound;
}

void Animated_polygon::update_graphics(float dt)
{
	next_frame(dt);
	update_frame();
}

void Animated_polygon::reset_graphics()
{
	animation->reset();
	update_graphics(.0f);
}

Animated_moving_platform::Animated_moving_platform(Vectorf pos_,
	std::unique_ptr<Animation>&& animation_, std::vector<sf::Vertex> points_,
	std::unique_ptr<Simple_AI> ai_, int surface_, sf::Color color)
	: Moving_platform(pos_, nullptr, points_, std::move(ai_), surface_, color),
	animation(std::move(animation_))
{
	update_graphics(.0f);
}

void Animated_moving_platform::update_graphics(float dt)
{
	next_frame(dt);
	update_frame();
}

void Animated_moving_platform::update_physics(float dt)
{
	Moving_platform::update_physics(dt);
}

void Animated_moving_platform::update_frame()
{
	texture = animation->get_texture();
}

void Animated_moving_platform::next_frame(float dt)
{
	animation->next_frame(dt);
}

void Animated_moving_platform::draw(Gbuffer& target, sf::RenderStates states)
{
	states.transform *= ai->get_pos();
	for (int i = 0; i < 3; i++)
	{
		states.texture = texture->at(i);
		target[i].draw(polygon, states);
	}
}

void Animated_moving_platform::draw_dynamic_collision(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= ai->get_pos();
	target.draw(vertex, states);
}

void Animated_moving_platform::reset_graphics()
{
	Moving_platform::reset_physics();
	animation->reset();
	update_graphics(.0f);
}

Moving_polygon::Moving_polygon(Vectorf pos_, const std::array<const sf::Texture*, 3>* texture_,
	std::vector<sf::Vertex> points_, std::unique_ptr<Simple_AI> ai_,
	sf::Color color) : Textured_polygon(pos_, texture_, points_, color),
	ai(std::move(ai_)) {}

void Moving_polygon::update_graphics(float dt)
{
	ai->calc_pos(dt);
}

void Moving_polygon::draw(Gbuffer& target, sf::RenderStates states)
{
	states.transform *= ai->get_pos();
	Textured_polygon::draw(target, states);
}

void Moving_polygon::reset_graphics()
{
	ai->reset();
}

Animated_moving_polygon::Animated_moving_polygon(Vectorf pos_,
	std::unique_ptr<Animation>&& animation_, std::vector<sf::Vertex> points_,
	std::unique_ptr<Simple_AI> ai_, sf::Color color) : 
	Moving_polygon(pos_, nullptr, points_, std::move(ai_), color),
	animation(std::move(animation_)) {}

void Animated_moving_polygon::update_graphics(float dt)
{
	next_frame(dt);
	update_frame();
	Moving_polygon::update_graphics(dt);

}

void Animated_moving_polygon::update_frame()
{
	texture = animation->get_texture();
}

void Animated_moving_polygon::next_frame(float dt)
{
	animation->next_frame(dt);
}

void Animated_moving_polygon::draw(Gbuffer& target, sf::RenderStates states)
{
	states.transform *= ai->get_pos();
	for (int i = 0; i < 3; i++)
	{
		states.texture = texture->at(i);
		target[i].draw(polygon, states);
	}
}

void Animated_moving_polygon::reset_graphics()
{
	Moving_polygon::reset_graphics();
	animation->reset();
}
