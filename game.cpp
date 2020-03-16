#include "game.h"

Platform::Platform(Vectorf p, const sf::Texture* t, std::vector<sf::Vertex> points) : Texturable(p, t, points)
{
	float maxx, maxy, miny, minx;
	maxx = minx = vertices[0].position.x;
	maxy = miny = vertices[0].position.y;
	for (auto it : vertices)
	{
		mesh_collision.push_back({ it.position.x, it.position.y });
		if (it.position.x < minx)
			minx = it.position.x;
		if (it.position.y < miny)
			miny = it.position.y;
		if (it.position.x > maxx)
			maxx = it.position.x;
		if (it.position.y > maxy)
			maxy = it.position.y;
	}
	rect_collision = sf::FloatRect(minx + p.x, miny + p.y, maxx - minx, maxy - miny);
}

Player::Player(Vectorf p, std::vector<const Animation* > t, float h, float gs, float m) : Entity(p, t, h, gs, m) {}

Entity::Entity(Vectorf p, std::vector<const Animation* > t, float h, float gs, float m) : Animatable(p, t[0], h, gs), animations(t)
{
	status = Entity_status::IDLE;
	rect_collision = sprite.getGlobalBounds();
	mass = m;
}

void Entity::move(Vectorf delta)
{
	speed += delta;
}

void Entity::jump()
{
	if (colision_direction.y == 1)
	{
		apply_force({ 0, -20 });
		if (status == Entity_status::IDLE)
			status = Entity_status::JUMP_IDLE;
		if (status == Entity_status::MOVE)
			status = Entity_status::JUMP_RUN;
	}
}

void Entity::next_frame()
{
	if (&*tex != &*animations[status])
	{
		set_animation(animations[status]);
		sf::Vector2u size = it->getSize();
		sprite.setTextureRect(sf::IntRect(0, 0, size.x, size.y));
	}
	if (++it == tex->end())
	{
		if (status != Entity_status::MOVE)
		{
			status = Entity_status::IDLE;
			set_animation(animations[status]);
		}
		else
			it = tex->begin();
	}
	sprite.setTexture(*it);
}

void Entity::update()
{
	int s = sgn(speed.x);
	if (direction != s && s != 0)
	{
		Vectorf tmp = sprite.getScale();
		tmp.x *= -1;
		scale = -scale;
		speed = {speed.x*2, speed.y*2};
		if (s == -1)
		{
			sprite.setOrigin(sprite.getLocalBounds().width, 0);
			sprite.setScale(-1, 1);
		}
		else
		{
			sprite.setOrigin(0, 0);
			sprite.setScale(1, 1);
		}
		sprite.setScale(tmp);
		direction = s;
	}
	if (force.x > max_force)
		force.x = max_force;
	if (force.y > max_force)
		force.y = max_force;
	speed += force;
	last_speed = speed;
	update_position();
	if (last_speed.x != 0 && last_speed.y >= 0 && colision_direction.y == 1)
	{
		status = Entity_status::MOVE;
	}
	if (last_speed.x == 0 && last_speed.y >= 0 && colision_direction.y == 1)
	{
		status = Entity_status::IDLE;
	}
	colision_direction = { 0,0 };
}

void Entity::update_position()
{
	pos += speed;
	sprite.setPosition(pos);
	rect_collision = sprite.getGlobalBounds();
	speed = { 0,0 };
}

void Entity::set_animation(const Animation* t)
{
	this->pos += -(t->center - tex->center) * this->scale;
	this->sprite.move(-(t->center - tex->center) * this->scale);
	tex = t;
	it = tex->begin();
}