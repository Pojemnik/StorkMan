#include "game.h"


Platform::Platform(Vectorf p, const sf::Texture* t, std::vector<sf::Vertex> points) : Texturable(p, t, points)
{
	float maxx, maxy, miny, minx;
	maxx = minx = vertices[0].position.x;
	maxy = miny = vertices[0].position.y;
	for (auto it : vertices)
	{
		mesh.vertices.push_back({ it.position.x + pos.x, it.position.y + pos.y });
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
void Platform::rescale(float ratio)
{
	Texturable::rescale(ratio);
	Colidable::rescale(ratio);
}
Player::Player(Vectorf p, sf::Texture* texture, std::vector<sf::IntRect>& v,
	std::vector<const Dynamic_animation*> a, sf::FloatRect rc, Animation_tree t, float h, float gs, float m)
	: Dynamic_entity(p, texture, v, a, rc, t, h, gs, m) {}

Entity::Entity(Vectorf p, std::vector<const Animation* > t, float h, float gs, float m) : Animatable(p, t[0], h, gs), animations(t)
{
	animation_status = Entity_status::IDLE;
	rect_collision = sf::FloatRect(tex->rect_collision.left * scale + p.x, tex->rect_collision.top * scale + p.y, tex->rect_collision.width * scale, tex->rect_collision.height * scale);
	mesh = Mesh_collision(tex->rect_collision, scale, p);
	mass = m;
}

void Entity::move(Vectorf delta)
{
	move_force += delta;
	if (move_speed.x * move_speed.x + move_speed.y * move_speed.x < context.min_move_speed * context.min_move_speed)
	{
		move_speed = util::normalize(delta, context.min_move_speed);
	}
	if (colision_direction.y == 1)
	{
		animation_status = Entity_status::MOVE;
	}
}

void Player::attack(int attack_type)
{
	switch(attack_type)
	{
	case 1:
		if (animation_status == Entity_status::IDLE || animation_status == Entity_status::JUMP_IDLE)
		{
			status = Entity_status::PUNCH_1;
			animation_status = Animation_status::A_PUNCH_1;
			reset_animation = true;
		}
		break;
	case 2:
		if (animation_status == Entity_status::IDLE || animation_status == Entity_status::JUMP_IDLE)
		{
			status = Entity_status::PUNCH_2;
			animation_status = Animation_status::A_PUNCH_2;
			reset_animation = true;
		}
		break;
	}
}

void Entity::jump()
{
	if (colision_direction.y == 1)
	{
		apply_force({ 0, -20 });
		if (animation_status == Entity_status::IDLE || animation_status == Entity_status::JUMP_IDLE)
		{
			animation_status = Entity_status::JUMP_IDLE;
			reset_animation = true;
		}
		if (animation_status == Entity_status::MOVE || animation_status == Entity_status::JUMP_RUN)
		{
			animation_status = Entity_status::JUMP_RUN;
			reset_animation = true;
		}
	}
}

void Entity::next_frame()
{
	if (&*tex != &*animations[animation_status] || reset_animation)
	{
		set_animation(animations[animation_status]);
		sf::Vector2u size = it->getSize();
		sprite.setTextureRect(sf::IntRect(0, 0, size.x, size.y));
		reset_animation = false;
	}
	if (++it == tex->end())
	{
		if (animation_status != Entity_status::MOVE)
		{
			animation_status = Entity_status::IDLE;
			set_animation(animations[animation_status]);
		}
		else
			it = tex->begin();
	}
	sprite.setTexture(*it);
}

void Entity::update(float dt)
{
	move_speed += move_force;
	move_speed = util::saturate(move_speed, context.max_move_speed);
	total_speed += force;
	last_speed = total_speed;
	if (move_force == Vectorf(0, 0))
	{
		if (move_speed.x > 0)
			move_speed.x -= context.move_speed_reduction.x;
		if (move_speed.x < 0)
			move_speed.x += context.move_speed_reduction.x;
		if (move_speed.y > 0)
			move_speed.y -= context.move_speed_reduction.y;
		if (move_speed.y < 0)
			move_speed.y += context.move_speed_reduction.y;
		if (fabs(move_speed.x) < 1)
		{
			move_speed.x = 0;
			if (animation_status == Entity_status::MOVE)
				animation_status = Entity_status::IDLE;
		}
		if (fabs(move_speed.y) < 1)
		{
			move_speed.y = 0;
			if (animation_status == Entity_status::MOVE)
				animation_status = Entity_status::IDLE;
		}
	}
	total_speed += move_speed;
	int s = util::sgn(total_speed.x);
	if (direction != s && s != 0)
	{
		scale = -scale;
		if (s == -1)
		{
			sprite.setOrigin(sprite.getLocalBounds().width, 0);
		}
		else
		{
			sprite.setOrigin(0, 0);
		}
		sprite.setScale(-1, 1);
		direction = s;
	}
	force = util::saturate(force, context.max_force);
	if (colision_direction.y == 1 && (animation_status == Entity_status::JUMP_IDLE || animation_status == Entity_status::JUMP_RUN))
		animation_status = Entity_status::IDLE;
	update_position();
	move_force = { 0,0 };
	colision_direction = { 0,0 };
}

void Entity::update_position()
{
	pos += total_speed;
	sprite.setPosition(pos);
	if (scale > 0)
	{
		rect_collision = sf::FloatRect(tex->rect_collision.left * scale + pos.x, tex->rect_collision.top * scale + pos.y, tex->rect_collision.width * scale, tex->rect_collision.height * scale);
	}
	else
	{
		rect_collision = sf::FloatRect((tex->rect_collision.left) * -scale + pos.x, (540 - tex->rect_collision.top) * -scale + pos.y, tex->rect_collision.width * -scale, tex->rect_collision.height * scale);
	}
	mesh = Mesh_collision(rect_collision);
	total_speed = { 0,0 };
}

Vectorf Entity::get_position()
{
	return pos;
}

void Entity::set_animation(const Animation* t)
{
	//this->pos += -(t->center - tex->center) * this->scale;
	//this->sprite.move(-(t->center - tex->center) * this->scale);
	rect_collision = sf::FloatRect(tex->rect_collision.left * scale + pos.x, tex->rect_collision.top * scale + pos.y, tex->rect_collision.width * scale, tex->rect_collision.height * scale);
	mesh = Mesh_collision(rect_collision);
	tex = t;
	it = tex->begin();
}

Dynamic_entity::Dynamic_entity(Vectorf p, sf::Texture* texture, std::vector<sf::IntRect>& v,
	std::vector<const Dynamic_animation*> a, sf::FloatRect rc, Animation_tree t, float h, float gs, float m)
	: Dynamic_animatable(texture, v, p, a, t, h, gs)
{
	animation_status = Animation_status::A_IDLE;
	rect_collision = rc;
	mesh = Mesh_collision(rect_collision);
	mass = m;
	sprite.setOrigin({ actual_frame[0] + 64, actual_frame[1] + 64 });
}

void Dynamic_entity::move(Vectorf delta)
{
	//if (util::sgn(delta.x) != colision_direction.x)
	{
		move_force += delta;
		if (move_speed.x * move_speed.x + move_speed.y * move_speed.x < context.min_move_speed * context.min_move_speed||util::vector_dot_product(move_speed,delta)<0)
		{
			move_speed = util::normalize(delta, context.min_move_speed);
		}
		if (status == IDLE)
			status = Entity_status::MOVE;
		if (colision_direction.y == 1 && animation_status != Animation_status::A_JUMP_RUN && animation_status
			!= Animation_status::A_JUMP_RUN2)
		{
			animation_status = Animation_status::A_MOVE;
		}
	}
}

void Dynamic_entity::jump(bool move)
{
	if (colision_direction.y == 1)
	{
		if (move)
		{
			if (animation_status == Animation_status::A_MOVE ||
				((animation_status == Animation_status::A_JUMP_RUN || animation_status == Animation_status::A_JUMP_RUN2)
					&& last_status == IN_AIR))
			{
				if (key == 1 || key == 2 || key == 3)
					animation_status = Animation_status::A_JUMP_RUN;
				else
					animation_status = Animation_status::A_JUMP_RUN2;
				status = Entity_status::JUMP_RUN;
				reset_animation = true;
			}
		}
		else
		{
			if (animation_status == Animation_status::A_IDLE ||
				(animation_status == Animation_status::A_JUMP_IDLE && last_status == IN_AIR))
			{
				animation_status = Animation_status::A_JUMP_IDLE;
				status = Entity_status::JUMP_IDLE;
				reset_animation = true;
			}
		}
	}
}

void Dynamic_entity::flip(int sign)
{
	if (sign == -1)
	{
		sprite.setOrigin({ actual_frame[0] - 64, 192 + 64 });
	}
	else
	{
		sprite.setOrigin({ actual_frame[0] + 64, 192 + 64 });
	}
	if (direction != sign)
	{
		scale = -scale;
		sprite.scale(-1, 1);
		direction = sign;
	}
}

void Dynamic_entity::set_idle()
{
	animation_status = Animation_status::A_IDLE;
	status = Entity_status::IDLE;
	reset_animation = true;
}

void Dynamic_entity::update(float dt)
{
	Vectorf move_acc = move_force / mass;
	move_speed += move_acc * dt;
	move_speed = util::saturate(move_speed, context.max_move_speed);
	if (move_force == Vectorf(0, 0))
	{
		if (move_speed.x > 0)
			move_speed.x -= context.move_speed_reduction.x;
		if (move_speed.x < 0)
			move_speed.x += context.move_speed_reduction.x;
		if (move_speed.y > 0)
			move_speed.y -= context.move_speed_reduction.y;
		if (move_speed.y < 0)
			move_speed.y += context.move_speed_reduction.y;
		if (fabs(move_speed.x) < 1)
		{
			move_speed.x = 0;
			if (animation_status == Animation_status::A_MOVE)
			{
				set_idle();
			}
		}
		if (fabs(move_speed.y) < 1)
		{
			move_speed.y = 0;
			if (animation_status == Animation_status::A_MOVE)
			{
				set_idle();
			}
		}
	}
	if (colision_direction.y == 1 && status == IN_AIR && last_status == status)
	{
		set_idle();
	}
	if (animation_status == Animation_status::A_JUMP_IDLE && key == 2 && frames_delta == 15)
	{
		if (colision_direction.y == 1)
			apply_force({ 0, -context.jump_force });
		status = IN_AIR;
	}
	if ((animation_status == Animation_status::A_JUMP_RUN ||
		animation_status == Animation_status::A_JUMP_RUN2) && key == 2 && frames_delta == 1)
	{
		if (colision_direction.y == 1)
			apply_force({ 0, -context.jump_force });
		status = IN_AIR;
	}
	Vectorf acc = force / mass;
	total_speed += acc * dt;
	last_speed = total_speed;
	total_speed += move_speed * dt;
	int x_speed_sign = util::sgn(total_speed.x);
	if (x_speed_sign != 0)
	{
		flip(x_speed_sign);
	}
	force = util::saturate(force, context.max_force);
	update_position(dt);
	last_animation_status = animation_status;
	last_status = status;
	move_force = { 0,0 };
	colision_direction = { 0,0 };
}

void Dynamic_entity::update_position(float dt)
{
	pos += total_speed * dt;	//ogarn�� to co�!!!
	sprite.setPosition(pos);
	rect_collision = sf::FloatRect(pos.x - 20,
		pos.y-47, 20,
		rect_collision.height);
	//rect_collision = sf::FloatRect(rect_collision.left + total_speed.x,
	//	rect_collision.top + total_speed.y, rect_collision.width, rect_collision.height);
	mesh = Mesh_collision(rect_collision);
	total_speed = { 0,0 };
}

void Dynamic_entity::next_frame()
{
	if (reset_animation)
	{
		set_animation(animation_status);
		reset_animation = false;
	}

	Dynamic_animatable::next_frame();
}

Vectorf Dynamic_entity::get_position()
{
	return pos;
}

void Dynamic_entity::rescale(float new_scale)
{
	float ratio=new_scale/(scale * 350 / height);
	Dynamic_animatable::rescale(new_scale);
	Colidable::rescale(ratio);
}
