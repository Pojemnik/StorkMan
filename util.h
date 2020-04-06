#pragma once
#include <SFML/Graphics.hpp>

typedef sf::Vector2f Vectorf;
typedef sf::Vector2i Vectori;

const float PI = 3.1415927f;

enum Entity_status { IDLE = 0, MOVE, JUMP_IDLE, JUMP_RUN, ATTACK, HIT, IN_AIR };
enum Animation_status { A_IDLE = 0, A_MOVE, A_JUMP_IDLE, A_JUMP_RUN, A_JUMP_RUN2, A_ATTACK, A_HIT };
enum Colidable_type { GROUND, ENEMY, OTHER };
enum Stork_parts {
	BELLY = 0, L_HAND, R_HAND, HEAD, CHEST, L_CALF, R_CALF, PELVIS, L_FOREARM,
	R_FOREARM, L_ARM, R_ARM, L_FOOT, R_FOOT, L_TIGH, R_TIGH, R_WING_1, R_WING_2,
	R_WING_3, R_WING_4, R_WING_5, L_WING_1, L_WING_2, L_WING_3, L_WING_4, L_WING_5,
	TAIL
};

const int map_stork_drawing_sequence_to_enum[] = {10, 21, 22, 23, 24, 25, 8, 1,
	7, 0, 4, 3, 5, 12, 14, 26, 6, 13, 15, 16, 17, 18, 19, 20, 9, 11, 2};

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