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

template <typename T> inline int sgn(T val)
{
	return (T(0) < val) - (val < T(0));
}

struct xyr
{
	Vectorf pos;
	float r;
};

Vectorf saturate(Vectorf val, const Vectorf max_val);

float rdn(float s);

static const struct Stork_var
{
	float GLOxKLA = 36;
	float GLOyKLA = 96;
	float KLAxGLO = 60;
	float KLAyGLO = 42;
	float KLAxBRZ = 72;
	float KLAyBRZ = 76;
	float KLAxPRA = 19;
	float KLAyPRA = 56;
	float KLAxLRA = 89;
	float KLAyLRA = 49;
	float BRZxKLA = 70;
	float BRZyKLA = 36;
	float BRZxMIE = 70;
	float BRZyMIE = 98;
	float MIExBRZ = 70;
	float MIEyBRZ = 62;
	float MIExPUD = 44;
	float MIEyPUD = 87;
	float MIExLUD = 78;
	float MIEyLUD = 88;
	float MIExOGO = 51;
	float MIEyOGO = 77;
	float PUDxMIE = 63;
	float PUDyMIE = 28;
	float PUDxPLY = 66;
	float PUDyPLY = 99;
	float PLYxPUD = 68;
	float PLYyPUD = 26;
	float PLYxPST = 70;
	float PLYyPST = 99;
	float PSTxPLY = 53;
	float PSTyPLY = 59;
	float PRAxKLA = 58;
	float PRAyKLA = 29;
	float PRAxPPR = 58;
	float PRAyPPR = 100;
	float PPRxPRA = 58;
	float PPRyPRA = 30;
	float PPRxPDL = 60;
	float PPRyPDL = 105;
	float PPRxSK1 = 59;
	float PPRySK1 = 34;
	float PPRxSK2 = 59;
	float PPRySK2 = 49;
	float PPRxSK3 = 59;
	float PPRySK3 = 64;
	float PPRxSK4 = 59;
	float PPRySK4 = 79;
	float PPRxSK5 = 59;
	float PPRySK5 = 94;
	float PDLxPPR = 61;
	float PDLyPPR = 53;
	float LUDxMIE = 63;
	float LUDyMIE = 28;
	float LUDxLLY = 66;
	float LUDyLLY = 99;
	float LLYxLUD = 68;
	float LLYyLUD = 26;
	float LLYxLST = 70;
	float LLYyLST = 99;
	float LSTxLLY = 53;
	float LSTyLLY = 59;
	float LRAxKLA = 58;
	float LRAyKLA = 29;
	float LRAxLPR = 58;
	float LRAyLPR = 100;
	float LPRxLRA = 58;
	float LPRyLRA = 30;
	float LPRxLDL = 60;
	float LPRyLDL = 105;
	float LPRxSK1 = 59;
	float LPRySK1 = 34;
	float LPRxSK2 = 59;
	float LPRySK2 = 49;
	float LPRxSK3 = 59;
	float LPRySK3 = 64;
	float LPRxSK4 = 59;
	float LPRySK4 = 79;
	float LPRxSK5 = 59;
	float LPRySK5 = 94;
	float LDLxLPR = 63;
	float LDLyLPR = 50;
	float SKRxPRZ = 64;
	float SKRyPRZ = 11;
	float OGOxMIE = 63;
	float OGOyMIE = 15;
} stork_var;