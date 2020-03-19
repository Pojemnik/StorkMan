#include "collisions.h"
inline sf::Vector2f normalize(sf::Vector2f x,float l)
{
	return x / sqrt(x.x * x.x + x.y * x.y)* l;
}
inline sf::Vector2f getAxisNormal(const std::vector<sf::Vector2f>* a, int i) {
	sf::Vector2f p1 = (*a)[i], p2=(i>=a->size()-1)?(*a)[0]:(*a)[i+1];
	return normalize({ p1.y - p2.y,p2.x - p1.x }, 1);
}
inline float vectorDotProduct(sf::Vector2f a, sf::Vector2f b)
{
	return a.x * b.x + a.y * b.y;
}
bool testBollean(const std::vector<sf::Vector2f>* a, const std::vector<sf::Vector2f>* b)
{
	if (!checkPolygonsForSAT(a, b, 0, 0).collides)
		return 0;
	if (!checkPolygonsForSAT(b, a, 1, 0).collides)
		return 0;
	return 1;
}
CollisionInfo checkPolygonsForSAT(const std::vector<sf::Vector2f>* a, const std::vector<sf::Vector2f>* b, bool flip, bool docalc)
{
	float min0, max0, min1, max1;
	sf::Vector2f vAxis;
	float sOffset;
	float t;
	int i, j;
	CollisionInfo result;
	result.collides = 1;
	float distmin;
	float distminABS;
	float shortestDist = FLT_MAX;
	result.shapeAContained = 1;
	result.shapeBContained = 1;
	for (i = 0; i < a->size(); i++)
	{
		vAxis = getAxisNormal(a, i);
		max0=min0 = vectorDotProduct(vAxis, (*a)[0]);
		for (int j = 1; j < a->size(); j++)
		{
			t = vectorDotProduct(vAxis, (*a)[j]);
			if (t < min0)
				min0 = t;
			if (t > max0)
				max0 = t;
		}
		max1 = min1 = vectorDotProduct(vAxis, (*b)[0]);
		for (int j = 1; j < b->size(); j++)
		{
			t = vectorDotProduct(vAxis, (*b)[j]);
			if (t < min1)
				min1 = t;
			if (t > max1)
				max1 = t;
		}
		if (min0>max1 || min1>max0)
			return CollisionInfo(false);
		if (docalc)
		{/*
			if (!flip) {
				if (max0 > max1 || min0 < min1) result.shapeAContained = false;
				if (max1 > max0 || min1 < min0) result.shapeBContained = false;
				
			}
			else {
				if (max0 < max1 || min0 > min1) result.shapeAContained = false;
				if (max1 < max0 || min1 > min0) result.shapeBContained = false;
			}
			*/
			distmin = min0 - max1;
			if (flip) distmin *= -1;
			distminABS = abs(distmin);
			if (distminABS < shortestDist)
			{
				result.distance = distmin;
				result.vector = vAxis;
				shortestDist = distminABS;
			}
		}
	}
	return result;
}

sf::Vector2f test( const std::vector<sf::Vector2f>* a, const std::vector<sf::Vector2f>* b)
{
	CollisionInfo result1 = checkPolygonsForSAT(a, b, 0, 1);
	if (!result1.collides)
		return sf::Vector2f(0, 0);
	CollisionInfo result2 = checkPolygonsForSAT(b, a, 1, 1);
	if (!result2.collides)
		return sf::Vector2f(0, 0);
	if (abs(result1.distance) < abs(result2.distance))
		return result1.vector * result1.distance;
	else
		return result2.vector * result2.distance;
	
}

CollisionInfo::CollisionInfo(bool c)
{
	collides = c;
}