#include "map.h"

Level::Level()
{
}

void Level::load()
{
}

void Level::unload()
{
}

void Level::addDrawable(Drawable d)
{
	drawables.push_back(d);
}

void Level::addTexturable(Texturable t)
{
	texturables.push_back(t);
}

void Map::update()
{
}

void Map::draw()
{
}
