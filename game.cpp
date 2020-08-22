#include "game.h"

Wall::Wall(Vectorf p, const sf::Texture* t, std::vector<sf::Vertex> points,
	int layer)
	: Texturable(p, t, points, layer) {}

Object::Object(Vectorf p, const sf::Texture* t, float h, int layer)
	: Renderable(p, t, h, layer) {}

Object::Object(Vectorf p, const sf::Texture* t, float h, int layer, int flip,
	float ang)
	: Renderable(p, t, h, layer, flip, ang) {}
void Moving_object::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= sf::Transform().translate(move_pos);
	Renderable::draw(target, states);
}
void Moving_object::update(float dt)
{
	time += dt;
	while (time > move_data.it->second)
	{
		time -= move_data.it->second;
		move_data.it = util::increment_iterator(move_data.it, move_data.points);
	}
	auto next = util::increment_iterator(move_data.it, move_data.points);
	float a = time / move_data.it->second;
	move_pos = (1.0f - a) * move_data.it->first + a * next->first;
}
Moving_object::Moving_object(Vectorf p, const sf::Texture* t, float h, int layer, int flip,
	float ang, Linear_move path) : Object(p, t, h, layer, flip, ang), move_data(path) {
	move_data.it = move_data.points.begin();
}

Moving_object::Moving_object(const Moving_object& mo) : Object(mo), move_data(mo.move_data)
{
	move_data.it = move_data.points.begin();
}

Animated_object::Animated_object(Vectorf p,
	const std::vector<sf::Texture>* a, float h, int layer, int fr)
	: Animatable(p, a, h, layer), frames_diff(fr) {}

Animated_object::Animated_object(Vectorf p,
	const std::vector<sf::Texture>* a, float h, int layer, int fr,
	int flip, float ang, int frame_delta)
	: Animatable(p, a, h, layer, flip, ang), frames_diff(fr)
{
	for (int i = 0; i < frame_delta / frames_diff; i++)
	{
		Animatable::next_frame();
	}
	frame_counter = frame_delta % frames_diff;
}

void Animated_object::next_frame()
{
	if (++frame_counter == frames_diff)
	{
		frame_counter = 0;
		Animatable::next_frame();
	}
}

Player::Player(Vectorf p, sf::Texture* texture, std::vector<sf::IntRect>& v,
	std::vector<const Dynamic_animation*> a, sf::FloatRect rc,
	Animation_tree t, float h, float gs, float m, int hp)
	: Dynamic_entity(p, texture, v, a, rc, t, h, gs, m, hp) {}

void Player::attack(int attack_type)
{
	switch (attack_type)
	{
	case 1:
		if (animation_status == Entity_status::IDLE ||
			animation_status == Entity_status::JUMP_IDLE)
		{
			status = Entity_status::PUNCH_1;
			animation_status = Animation_status::A_PUNCH_1;
			reset_animation = true;
		}
		break;
	case 2:
		if (animation_status == Entity_status::IDLE ||
			animation_status == Entity_status::JUMP_IDLE)
		{
			status = Entity_status::PUNCH_2;
			animation_status = Animation_status::A_PUNCH_2;
			reset_animation = true;
		}
		break;
	}
}

Dynamic_entity::Dynamic_entity(Vectorf p, sf::Texture* texture,
	std::vector<sf::IntRect>& v, std::vector<const Dynamic_animation*> a,
	sf::FloatRect rc, Animation_tree t, float h, float gs, float m, int hp)
	: Dynamic_animatable(texture, v, p, a, t, h, gs), health(hp), MAX_HEALTH(hp)
{
	animation_status = Animation_status::A_IDLE;
	rect_collision = rc;
	col_height = rc.height;
	mesh = Mesh_collision(rect_collision);
	mass = m;
	sprite.setOrigin({ actual_frame[0] + 64, actual_frame[1] + 64 });
}

void Dynamic_entity::move(Vectorf delta)
{
	move_force += delta;
	if (move_speed.x * move_speed.x + move_speed.y * move_speed.y <
		context.min_move_speed * context.min_move_speed ||
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
	if (util::round_and_compare(last_pos, pos))
	{
		status = IDLE;
		animation_status = Animation_status::A_IDLE;
	}
	last_pos = pos;
}

void Dynamic_entity::move_angled(int dir)
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

void Dynamic_entity::jump(bool move)
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

void Dynamic_entity::stop_jump()
{
	if (status == Entity_status::IN_AIR)
	{
		apply_force({ 0, jump_force_sum * 0.8f });
		jump_force_sum = 0;
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
			apply_force({ 0, -context.jump_force - edge_jump_buf * context.gravity });
			jump_force_sum = context.jump_force;
			edge_jump_buf = max_edge_jump;
		}
		status = IN_AIR;
	}
	int x_speed_sign = util::sgn(move_speed.x);
	Vectorf acc = force / mass;
	total_speed += acc * dt;
	total_speed += move_speed;
	if (x_speed_sign != 0)
	{
		flip(x_speed_sign);
	}
	total_speed = util::saturate(total_speed, context.max_speed);
	force = util::saturate(force, context.max_force);
	update_position(dt);
	last_animation_status = animation_status;
	last_status = status;
	move_force = { 0,0 };
	collision_direction = { 0,0 };
	force.x = 0;
}

void Dynamic_entity::update_position(float dt)
{
	const float scale_factor = 32 / context.global_scale;
	pos += total_speed * dt;
	sprite.setPosition(pos);
	rect_collision = sf::FloatRect(pos.x - 18 / scale_factor,
		pos.y - 37 / scale_factor, 15 / scale_factor,
		rect_collision.height);
	Mesh_collision mesh_vect(rect_collision);
	mesh = mesh_vect;
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

void Dynamic_entity::set_position(Vectorf new_position)
{
	pos = new_position;
	force = { 0,0 };
	total_speed = { 0,0 };
	external_speed = { 0,0 };
	move_speed = { 0,0 };
}

void Dynamic_entity::rescale(float new_scale)
{
	float ratio = new_scale / (scale * 335 / height);
	Dynamic_animatable::rescale(ratio);
	Collidable::rescale(ratio);
}

void Dynamic_entity::deal_damage(int amount)
{
	health -= amount;
	if (health <= 0)
	{
		health = 0;
		die();
	}
}

void Dynamic_entity::die()
{
	//set_animation(death);
	set_position({ 20, 20 });
	context.aaa.play();
	context.console->log << "zgon" << '\n';
	health = 100;
}
