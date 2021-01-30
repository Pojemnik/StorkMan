#pragma once
#include <SFML/Graphics.hpp>

class Gbuffer
{
	std::array< const sf::Texture*, 3> arr;

public:
	sf::RenderTexture albedo;
	sf::RenderTexture normal;
	sf::RenderTexture position;

	Gbuffer(int x, int y)
	{
		for (int i = 0; i < 3; i++)
		{
			if (!at(i).create(x, y))
				throw std::runtime_error("Failed constructing Gbuffer");
			arr[i] = &at(i).getTexture();
		}
	}

	sf::RenderTexture& at(int i)
	{
		if (i == 0)
			return albedo;
		else if (i == 1)
			return normal;
		else if (i == 2)
			return position;
		else
			throw std::runtime_error("tex type out of range");
	}

	sf::RenderTexture& operator[](int i)
	{
		return at(i);
	}

	const std::array<const sf::Texture*, 3>* const getTexture()
	{
		return &arr;
	}

	void clear()
	{
		albedo.clear();
		normal.clear();
		position.clear();
	}

	void display()
	{
		albedo.display();
		normal.display();
		position.display();
	}
};

class Map_object
{
public:
	virtual sf::FloatRect get_bounding_rect() const = 0;
};

class Physical_updatable
{
public:
	virtual void update_physics(float dt) = 0;
	virtual void reset_physics() = 0;
};

class Graphical_updatable
{
public:
	virtual void update_graphics(float dt) = 0;
	virtual void reset_graphics() = 0;
};

class Renderable
{
public:
	virtual void draw(Gbuffer& target, sf::RenderStates states) = 0;
};

class Interactive
{
public:
	virtual std::vector<int> get_events() = 0;
	virtual void clicked(Vectori mouse_pos, int button) = 0;
};