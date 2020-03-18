#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
struct CollisionInfo {
	bool shapeAContained, shapeBContained;
	bool collides=true;
	sf::Vector2f vector;
	float distance;
	CollisionInfo(bool);
	CollisionInfo() = default;
	
	
};
bool testBollean(const std::vector<sf::Vector2f>* a, const std::vector<sf::Vector2f>* b);
CollisionInfo checkPolygonsForSAT(const std::vector<sf::Vector2f>* a,const std::vector<sf::Vector2f>* b, bool flip, bool docalc);
sf::Vector2f test(const std::vector<sf::Vector2f>* a,const std::vector<sf::Vector2f>* b);