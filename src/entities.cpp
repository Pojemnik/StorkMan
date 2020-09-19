#include "entities.h"

Entity::Entity(Vectorf p, std::unique_ptr<Animation>&& animation_,
	sf::FloatRect collision_, float height_, float mass_, int health_)
	: health(health_), height(height_), mass(mass_)
{
	set_max_health(health);
	animation_status = Animation_status::A_IDLE;
	rect_collision = rc;
	col_height = rc.height;
	mesh = Collision(rect_collision);
	mass = m;
	sprite.setOrigin({ actual_frame[0] + 64, actual_frame[1] + 64 });
}

void Entity::set_animation(Animation_index a)
{
	animation->set_animation(a);
}

Animation_index Entity::get_animation_info()
{
	return animation->get_animation_info();
}

void Entity::move(Vectorf delta)
{
	if (util::sgn(delta.x) > 0 && run_block == Run_block::RIGHT ||
		util::sgn(delta.x) < 0 && run_block == Run_block::LEFT)
	{
		return;
	}
	move_force += delta;
	if (util::sq(move_speed.x) + util::sq(move_speed.y) <
		util::sq(context.min_move_speed) ||
		util::vector_dot_product(move_speed, delta) < 0)
	{
		move_speed = util::normalize(delta, context.min_move_speed);
	}
	if (status == IDLE)
		status = Entity_status::MOVE;
	if (collision_direction.y == 1 &&
		animation_status != Animation_status::A_JUMP_RUN &&
		animation_status != Animation_status::A_JUMP_RUN2)
	{
		animation_status = Animation_status::A_MOVE;
	}
	if (util::round_and_compare(last_pos, pos) &&
		util::sgn(move_force.x) == util::sgn(last_move_force.x))
	{
		last_move_force = move_force;
		status = IDLE;
		animation_status = Animation_status::A_IDLE;
		move_speed = { 0,0 };
		move_force = { 0,0 };
	}
	else
	{
		last_move_force = move_force;
	}
	last_pos = pos;
}

void Entity::move_angled(int dir)
{
	if (dir == 1)
		move(util::rotate_vector(
			{ -context.player_move_speed.y, -context.player_move_speed.x },
			maxcollisionvector));
	else if (dir == -1)
		move(util::rotate_vector(
			{ -context.player_move_speed.y,context.player_move_speed.x },
			maxcollisionvector));
}

void Entity::move(int direction)
{
	physical.move({ move_speed * direction, 0.f });
}

void Entity::jump()
{
	physical.apply_force({ 0, -jump_force });
}

void Entity::jump(bool move)
{
	if (edge_jump_buf < max_edge_jump && context.jump_available)
	{
		if (animation_status == Animation_status::A_MOVE ||
			((animation_status == Animation_status::A_JUMP_RUN ||
				animation_status == Animation_status::A_JUMP_RUN2)
				&& last_status == IN_AIR))
		{
			if (key == 1 || key == 2 || key == 3)
				animation_status = Animation_status::A_JUMP_RUN;
			else
				animation_status = Animation_status::A_JUMP_RUN2;
			status = Entity_status::JUMP_RUN;
			reset_animation = true;
			context.jump_available = false;
			context.jump_run.play();
		}
		if (animation_status == Animation_status::A_IDLE ||
			(animation_status == Animation_status::A_JUMP_IDLE &&
				last_status == IN_AIR))
		{
			animation_status = Animation_status::A_JUMP_IDLE;
			status = Entity_status::JUMP_IDLE;
			reset_animation = true;
			context.jump_available = false;
			context.jump_idle.play();
		}
	}
}

void Entity::stop_jump()
{
	if (status == Entity_status::IN_AIR)
	{
		apply_force({ 0, jump_force_sum * 0.8f });
		jump_force_sum = 0;
	}
}

void Entity::set_idle()
{
	animation_status = Animation_status::A_IDLE;
	status = Entity_status::IDLE;
	reset_animation = true;
}

void Entity::flip_if_needed()
{
	int x_speed_sign = util::sgn(move_speed.x);
	if (x_speed_sign != 0)
	{
		if (x_speed_sign == -1)
		{
			sprite.setOrigin({ actual_frame[0] - 64, 192 + 64 });
		}
		else
		{
			sprite.setOrigin({ actual_frame[0] + 64, 192 + 64 });
		}
		if (direction != x_speed_sign)
		{
			scale = -scale;
			sprite.scale(-1, 1);
			direction = x_speed_sign;
		}
	}
}

void Entity::update(float dt)
{
	assert(state_stack.size() > 0);
	std::pair<Entity_state*, Entity_state_info> state = state_stack.top()->update(*this, dt);
	switch (state.second)
	{
	case Entity_state_info::REPLACE:
		state_stack.top()->exit(*this);
		delete state_stack.top();
		state_stack.pop();
		//No break
	case Entity_state_info::PUSH:
		state_stack.push(state.first);
		state_stack.top()->enter(*this);
		break;
	case Entity_state_info::POP:
		state_stack.top()->exit(*this);
		delete state_stack.top();
		state_stack.pop();
		break;
	case Entity_state_info::NONE:
		break;
	}
	//---------------------------------------------------
	edge_jump_update();
	total_speed += external_speed;
	external_speed *= 0.95f;
	if (fabs(external_speed.x) < MIN_EXTERNAL_SPEED)
	{
		external_speed.x = 0;
	}
	if (fabs(external_speed.y) < MIN_EXTERNAL_SPEED)
	{
		external_speed.y = 0;
	}
	apply_force(context.gravity * maxcollisionvector);
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
		}
	}
	if (status == IN_AIR)
	{
		if (collision_direction.y == 1 && last_status == status)
		{
			set_idle();
		}
		jump_force_sum -= context.gravity;
		if (jump_force_sum < 0)
		{
			jump_force_sum = 0;
		}
	}
	if (animation_status == Animation_status::A_JUMP_RUN ||
		animation_status == Animation_status::A_JUMP_RUN2 ||
		animation_status == Animation_status::A_JUMP_IDLE)
	{
		if (edge_jump_buf < max_edge_jump)
		{
			if (force.y < 0)
				force.y = 0;
			apply_force({ 0, -context.jump_force - edge_jump_buf * context.gravity });
			jump_force_sum = context.jump_force;
			edge_jump_buf = max_edge_jump;
		}
		status = IN_AIR;
	}
	Vectorf acc = force / mass;
	total_speed += acc * dt;
	total_speed += move_speed;
	total_speed = util::saturate(total_speed, context.max_speed);
	force = util::saturate(force, context.max_force);
	flip_if_needed();
	update_position(dt);
	last_animation_status = animation_status;
	last_status = status;
	move_force = { 0,0 };
	collision_direction = { 0,0 };
	force.x = 0;
	run_block = Run_block::NONE;
}

void Entity::edge_jump_update()
{
	if (collision_direction.y == 1)
	{
		edge_jump_buf = 0;
	}
	else
	{
		if (++edge_jump_buf > max_edge_jump)
		{
			edge_jump_buf = max_edge_jump;
		}
	}
}

void Entity::update_position(float dt)
{
	const float scale_factor = 32 / context.global_scale;
	pos += total_speed * dt;
	sprite.setPosition(pos);
	rect_collision = sf::FloatRect(pos.x - 18 / scale_factor,
		pos.y - 37 / scale_factor, 15 / scale_factor,
		rect_collision.height);
	Collision mesh_vect(rect_collision);
	mesh = mesh_vect;
	total_speed = { 0,0 };
}

void Entity::next_frame()
{
	if (reset_animation)
	{
		set_animation(animation_status);
		reset_animation = false;
	}
	Dynamic_animation::next_frame();
}

Vectorf Entity::get_position()
{
	return pos;
}

void Entity::set_position(Vectorf new_position)
{
	pos = new_position;
	force = { 0,0 };
	total_speed = { 0,0 };
	external_speed = { 0,0 };
	move_speed = { 0,0 };
}

void Entity::rescale(float new_scale)
{
	float ratio = new_scale / (scale * 335 / height);
	Dynamic_animation::rescale(ratio);
	old_Collidable::rescale(ratio);
}

void Entity::deal_damage(int amount)
{
	if (!context.god_mode)
	{
		health -= amount;
		if (health <= 0)
		{
			health = 0;
			die();
		}
	}
}

void Entity::die()
{
	status = Entity_status::DIE;
	animation_status = Animation_status::A_DIE;
	set_animation(animation_status);
	context.aaa.play();
}

void Entity::set_max_health(int val)
{
	max_health = val;
	if (health > max_health)
		health = max_health;
}

int Entity::get_max_health()
{
	return max_health;
}

void Entity::heal(int amount)
{
	health += amount;
	if (health > max_health)
		health = max_health;
}

void Entity::post_death()
{
	animation_status = Animation_status::A_IDLE;
	status = Entity_status::IDLE;
}
