#pragma once
#include <SFML/Graphics.hpp>

typedef sf::Vector2f Vectorf;
typedef sf::Vector2i Vectori;

const float PI = 3.1415927f;

enum Entity_status { IDLE = 0, MOVE, JUMP_IDLE, JUMP_RUN, PUNCH_1, HIT, IN_AIR };
enum Animation_status { A_IDLE = 0, A_MOVE, A_JUMP_IDLE, A_JUMP_RUN, A_JUMP_RUN2, A_PUNCH_1, A_HIT };
enum Colidable_type { GROUND, ENEMY, OTHER };

const Vectorf max_force = { 1000.f, 1000.0f };
const float max_run_speed = 5.f;

//Maybe some namespace?
struct xyr
{
	Vectorf pos;
	float r;
};

template <typename T> inline int sgn(T val)
{
	return (T(0) < val) - (val < T(0));
}

Vectorf saturate(Vectorf val, const Vectorf max_val);

float rdn(float s);