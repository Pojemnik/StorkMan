#include "core.h"
#include "collisions.h"
Renderable::Renderable(Vectorf p, sf::Texture* t, float h) : tex(t), pos(p), height(h)
{
	sprite = sf::Sprite(*tex);
	sprite.setPosition(pos);
}

void Renderable::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(sprite, states);
}

void Renderable::rescale(float gs)
{
	float scale = gs * height / sprite.getTexture()->getSize().y;
	sprite.setScale(scale, scale);
}

Texturable::Texturable(Vectorf p, const sf::Texture* t, std::vector<sf::Vertex> points) : tex(t), pos(p), vertices(points)
{
	shape = sf::VertexBuffer(sf::TrianglesFan, sf::VertexBuffer::Stream);
	shape.create(vertices.size());
	shape.update(&vertices[0]);
}

void Texturable::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= sf::Transform().translate(pos);
	states.texture = &*tex;
	target.draw(shape, states);
}

Animatable::Animatable(Vectorf p, const Animation* t, float h, float gs) : tex(t), pos(p), height(h)
{
	it = tex->begin();
	sprite = sf::Sprite(*it);
	sprite.setPosition(pos);
	scale = gs * height / sprite.getTexture()->getSize().y;
	sprite.setScale(scale, scale);
}

void Animatable::next_frame()
{
	if (++it == tex->end())
		it = tex->begin();
	sprite.setTexture(*it);
}

void Animatable::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(sprite, states);
}

Animation::Animation(std::vector<sf::Texture>& a, Vectorf c, sf::FloatRect rect_col) : content(std::move(a)), center(c), rect_collision(rect_col) {}

std::vector<sf::Texture>::const_iterator Animation::begin() const
{
	return content.begin();
}

std::vector<sf::Texture>::const_iterator Animation::end() const
{
	return content.end();
}

void Physical::apply_force(Vectorf f)
{
	force += f;
}

void Physical::uncolide(const Colidable* c)
{
	
	/*
	Vectorf tab[6] = { {0, -0.6f},  {0, -0.6f}, {-0.6f, 1.2}, {-0.6f, 0}, {0, -0.6f}, {0, -0.6f} };
	int i = 0;
	for (i = 0; i < 6 && test_colision(*c); i++)
	{
		move({ last_speed.x * tab[i].x, last_speed.y * tab[i].y });
		update_position();
	}
	if (i != 0)
	{
		colision_direction = { sgn(-last_speed.x * tab[i - 1].x), sgn(-last_speed.y * tab[i - 1].y) };
		if (colision_direction.x)
			force.x = 0;
		if (colision_direction.y)
			force.y = 0;
	}
	*/
	if (!rect_collision.intersects(c->rect_collision))
	{
	}
	else
	{
		sf::Vector2f tmp = test(&mesh_collision, &c->mesh_collision);
		move(tmp * -1.0f);
		colision_direction.x+= sgn(tmp.x);
		colision_direction.y+= sgn(tmp.y);
		if (abs(tmp.x) > 0.0001)
		{
			force.x = 0;
		}
		if (abs(tmp.y) > 0.0001)
		{
			force.y = 0;
		}
		update_position();
	}
	
}

Physical::Physical(sf::FloatRect rect, std::vector<Vectorf> mesh, Colidable_type t, float m) : Colidable(rect, mesh, t), mass(m)
{

}

Colidable::Colidable(sf::FloatRect rect, std::vector<Vectorf> mesh, Colidable_type t) : rect_collision(rect), mesh_collision(mesh), type(t)
{

}

bool Physical::test_colision(const Colidable& other)
{
	if (!rect_collision.intersects(other.rect_collision))
		return false;
	/*
	auto wekt = [](Vectorf& p1, Vectorf& p2, Vectorf& p3) {return (p2.x - p1.x)*(p3.y - p1.y) - (p3.x - p1.x)*(p2.y - p1.y); };
	for (int i=1;i<mesh_collision.size()+1;i++)
	{
		for (int j=1;j<other.mesh_collision.size()+1;j++)
		{
			Vectorf p1 = mesh_collision[i!=mesh_collision.size()?i:0];
			Vectorf p2 = mesh_collision[i-1];
			Vectorf p3 = other.mesh_collision[j!=other.mesh_collision.size()?j:0];
			Vectorf p4 = other.mesh_collision[j-1];
			float x1 = std::min(p1.x, p2.x);
			float y1 = std::min(p1.y, p2.y);
			float x2 = std::max(p1.x, p2.x);
			float y2 = std::max(p1.y, p2.y);
			float x3 = std::min(p3.x, p4.x);
			float y3 = std::min(p3.y, p4.y);
			float x4 = std::max(p3.x, p4.x);
			float y4 = std::max(p3.y, p4.y);
			if (x2<x3 || x1>x4 || y2<y3 || y1>y4)
				continue;
			if (wekt(p1, p3, p2)*wekt(p1, p4, p2) <= 0 && wekt(p3, p1, p4)*wekt(p3, p2, p4) <= 0)
				return true;
		}

	}
	*/
	return testBollean(&mesh_collision,&other.mesh_collision);
	return false;
}