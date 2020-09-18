#pragma once
#include "util.h"

class old_Texturable : public sf::Drawable
{
protected:
	sf::VertexBuffer shape;
	const sf::Texture* tex;

public:
	std::vector<sf::Vertex> vertices;
	Vectorf pos;
	int layer;
	old_Texturable() = default;
	old_Texturable(Vectorf p, const sf::Texture* t,
		std::vector<sf::Vertex> points, int l);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void rescale(float ratio);
};

class Renderable : public sf::Drawable
{
};

class old_Renderable : public sf::Drawable
{
protected:
	const sf::Texture* tex;
	Vectorf pos;
	float height; //[m]
	sf::Sprite sprite;
	int flipint;

public:
	int layer;
	old_Renderable() = default;
	old_Renderable(Vectorf p, const sf::Texture* t, float h, int l);
	old_Renderable(Vectorf p, const sf::Texture* t, float h, int l, int flip,
		float ang);
	void rescale(float ratio);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

class old_Animatable : public sf::Drawable
{
protected:
	const std::vector<sf::Texture>* animation;
	std::vector<sf::Texture>::const_iterator it;
	Vectorf pos;
	float height; //[m]
	sf::Sprite sprite;
	int flipint;

public:
	int layer;
	old_Animatable() = default;
	old_Animatable(Vectorf p, const std::vector<sf::Texture>* a, float h, int l);
	old_Animatable(Vectorf p, const std::vector<sf::Texture>* a, float h, int l,
		int flip, float ang);
	void next_frame();
	void rescale(float ratio);
	virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const;
};