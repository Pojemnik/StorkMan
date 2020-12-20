#include "entities.h"

Entity::Entity(std::unique_ptr<Animation>&& animation_, Physical& physical_,
	std::unique_ptr<Entity_state_machine>&& state_,
	std::unique_ptr<Controller>&& controller_, std::pair<float, int> height_,
	int health_, Message_sender_type type)
	: animation(std::move(animation_)), physical(physical_),
	state_machine(std::move(state_)), controller(std::move(controller_)),
	health(health_), height(height_), Message_sender(type)
{
	set_max_health(health);
	Frame_info info = animation->get_frame_info();
	sprite.setOrigin({ info.character_position.x + info.part_size.x / 2,
			info.character_position.y + info.part_size.y / 2 });
	scale = context.global_scale * height.first / height.second;
	sprite.setScale(scale, scale);
	sf::FloatRect rect = physical.get_bounding_rect();
	coll_shape.setFillColor(sf::Color::White);
	coll_shape.setSize({ rect.width, rect.height });
	coll_shape.setPosition({ rect.left, rect.top });
}

void Entity::set_draw_collision(bool draw)
{
	draw_collision = draw;
}

void Entity::set_jump_force(float new_force)
{
	jump_force = new_force;
}

void Entity::set_move_speed(float new_speed)
{
	move_speed = new_speed;
}

const Collision* const Entity::get_collision() const
{
	return physical.get_collision();
}

void Entity::resolve_collision(const std::vector<std::shared_ptr<const Collidable>>& others)
{
	physical.resolve_collision(others);
}

void Entity::resolve_collision(const Collidable& other)
{
	physical.resolve_collision(other);
}

void Entity::set_animation(Animation_index a)
{
	animation->set_animation(a);
}

Animation_index Entity::get_current_animation()
{
	return animation->get_current_animation();
}

void Entity::move(int dir, float dt)
{
	physical.move({ move_speed * dir*dt, 0.f });
	if (last_direction != dir)
	{
		flip(dir);
		last_direction = dir;
	}
}

void Entity::apply_force(Vectorf force, float dt)
{
	physical.apply_force(force*dt);
}

void Entity::jump(float dt)
{
	physical.apply_force({ 0, -jump_force * dt});
}

void Entity::flip(int dir)
{
	Frame_info info = animation->get_frame_info();
	if (dir == -1)
	{
		sprite.setOrigin(Vectorf(float(info.character_position.x) - float(info.part_size.x )/ 2,
			float(info.offset.y) + float(info.part_size.y) / 2));
	}
	else
	{
		sprite.setOrigin(Vectorf(float(info.character_position.x) + float(info.part_size.x) / 2,
			float(info.offset.y) + float(info.part_size.y) / 2));
	}
	sprite.scale(-1, 1);
}

void Entity::update_physics(float dt)
{
	auto temp = physical.get_collision_info();
	collision_vector = temp.first;
	if (temp.second != surface)
	{
		surface = temp.second;
		send_message<int>(Message::Message_type::MOVED, static_cast<int>(surface));
	}
	on_ground = physical.is_on_ground();
	controller->update();
	state_machine->update(*this, dt);
	physical.set_fallthrough(fallthrough);
	physical.update_physics(dt);
	
}
void Entity::update_graphics(float dt)
{
	sprite.setPosition(physical.get_pos());
	animation->next_frame(dt);
	sprite.setTexture(*animation->get_texture());
	if (draw_collision)
	{
		sf::FloatRect rect = physical.get_bounding_rect();
		coll_shape.setPosition({ rect.left, rect.top });
	}
}

void Entity::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (draw_collision)
	{
		target.draw(coll_shape, states);
	}
	target.draw(sprite, states);
}

void Entity::push_state(Entity_state* state)
{
	state_machine->push_state(state, this);
}

Vectorf Entity::get_position()
{
	return physical.get_pos();
}

void Entity::set_position(Vectorf new_position)
{
	physical.set_position(new_position);
}

void Entity::deal_damage(int amount)
{
	if (!context.god_mode)
	{
		health -= amount;
		if (health <= 0)
		{
			health = 0;
		}
	}
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

void Entity::set_textures_set(int set)
{
	animation->change_textures_set(set);
}