#pragma once
#include "core.h"
#include "assets.h"

class Platform : public Texturable, public Colidable
{
public:
	Platform(Vectorf p, const sf::Texture* t, std::vector<sf::Vertex> points);
};

class Entity : public Animatable, public Physical
{
private:
	std::vector<const Animation*> animations;
	bool reset_animation = false;

public:
	Entity_status status;
	void set_animation(const Animation* t);
	Entity(Vectorf p, std::vector<const Animation*> t, float h, float gs, float m);
	void move(Vectorf delta);
	void jump();
	void next_frame();
	void update();
	void update_position();
	Vectorf get_position();
};

class Player : public Entity
{
public:
	Player(Vectorf p, std::vector<const Animation* > t, float h, float gs, float m);
};

class New_animatable : public sf::Drawable
{
private:
	Vectorf pos;
	std::vector<sf::Sprite> parts;
	Vectorf count_pos(int x, int y, int size1, int size2,
		int translation_x1, int translation_y1, float angle1,
		int translation_x2, int translation_y2, float angle2);
public:
	void animate(int x, int y, float r, float KLArGLO, float BRZrKLA,
		float MIErBRZ, float KLArPRA, float PRArPPR, float PPRrPDL,
		float KLArLRA, float LRArLPR, float LPRrLDL, float MIErPUD,
		float PUDrPLY, float PLYrPST, float MIErLUD, float LUDrLLY,
		float LLYrLST, float PPRrSKP, float LPRrSKL, float MIErOGO);
	New_animatable(Assets &a, Vectorf p);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};