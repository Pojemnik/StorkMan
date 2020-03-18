#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

typedef sf::Vector2f Vectorf;
typedef sf::Vector2i Vectori;

const float PI = 3.1415927f;

enum Entity_status { IDLE = 0, MOVE, JUMP_IDLE, JUMP_RUN, ATTACK, HIT };
enum Colidable_type { GROUND, ENEMY, OTHER };
enum Stork_parts {
	BELLY = 0, L_HAND, R_HAND, HEAD, CHEST, L_CALF, R_CALF, PELVIS, L_FOREARM,
	R_FOREARM, L_ARM, R_ARM, L_FOOT, R_FOOT, L_TIGH, R_TIGH, R_WING_1, R_WING_2,
	R_WING_3, R_WING_4, R_WING_5, L_WING_1, L_WING_2, L_WING_3, L_WING_4, L_WING_5,
	TAIL
};

const float max_force = 1000.f;
const float max_run_speed = 5.f;

template <typename T> inline int sgn(T val)
{
	return (T(0) < val) - (val < T(0));
}

struct xyr
{
	float x;
	float y;
	float r;
};

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

class Animation
{
public:
	Vectorf center;
	const std::vector<sf::Texture> content;
	const sf::FloatRect rect_collision;
	std::vector<sf::Texture>::const_iterator begin() const;
	std::vector<sf::Texture>::const_iterator end() const;
	Animation() = default;
	Animation(std::vector<sf::Texture>& a, Vectorf c, sf::FloatRect rect_col);
};

class Colidable
{
public:
	sf::FloatRect rect_collision;
	std::vector<Vectorf> mesh_collision;
	Colidable_type type;
	Colidable() = default;
	Colidable(sf::FloatRect rect, std::vector<Vectorf> mesh, Colidable_type t);

};

class Transformable
{
public:
	virtual void move(Vectorf delta) = 0;
};

class Physical : public Transformable, public Colidable
{
protected:
	float mass;
	Vectorf speed;
	Vectorf last_speed = { 0,0 };
	Vectorf force;
	Vectori colision_direction = { 0,0 };
public:
	virtual void update() = 0;
	virtual void update_position() = 0;
	virtual void move(Vectorf delta) = 0;
	virtual void apply_force(Vectorf f);
	void uncolide(const Colidable* c);
	bool test_colision(const Colidable& other);
	Physical(sf::FloatRect rect, std::vector<Vectorf> mesh, Colidable_type t, float m);
	Physical() = default;
};

class Texturable : public sf::Drawable
{
protected:
	sf::VertexBuffer shape;
	const sf::Texture* tex;
	std::vector<sf::Vertex> vertices;
	Vectorf pos;

public:
	Texturable();
	Texturable(Vectorf p, const sf::Texture* t, std::vector<sf::Vertex> points);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

class Renderable : public sf::Drawable
{
protected:
	const sf::Texture* tex;
	Vectorf pos;
	float height; //[m]
	sf::Sprite sprite;

public:
	Renderable() = default;
	Renderable(Vectorf p, sf::Texture* t, float h);
	void rescale(float gs);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

class Animatable : public sf::Drawable
{
protected:
	const Animation* tex;
	Vectorf pos;
	std::vector<sf::Texture>::const_iterator it;
	sf::Sprite sprite;
	int direction = 1;//x sign
	float height; //[m]
	float scale;

public:
	Animatable();
	Animatable(Vectorf p, const Animation* t, float h, float gs);
	void next_frame();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

class New_animatable : public sf::Drawable
{
private:
	Vectorf pos;
	std::vector<sf::Sprite> parts;
	int direction = 1;//x sign
	float height; //[m]
	float scale;
	sf::RenderTexture tex;
	sf::Sprite sprite;
	Vectorf count_pos(float x, float y, float size1, float size2,
		float translation_x1, float translation_y1, float angle1,
		float translation_x2, float translation_y2, float angle2);
public:
	void animate(float x, float y, float r, float KLArGLO, float BRZrKLA,
		float MIErBRZ, float KLArPRA, float PRArPPR, float PPRrPDL,
		float KLArLRA, float LRArLPR, float LPRrLDL, float MIErPUD,
		float PUDrPLY, float PLYrPST, float MIErLUD, float LUDrLLY,
		float LLYrLST, float PPRrSKP, float LPRrSKL, float MIErOGO);
	New_animatable(std::vector<sf::Texture>& v, Vectorf p, float h, float gs);
	void update();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};