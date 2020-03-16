#include "collisions.h"

bool test_colision(colidable* a, colidable* b) 
{
	Vector2 apos = *(a->posPTR);
	Vector2 bpos = *(b->posPTR);
	if (!a->BB.intersects(b->BB))
		return 0;
	auto wekt = [](Vector2& p1, Vector2& p2, Vector2& p3) {return (p2.x - p1.x)*(p3.y - p1.y) - (p3.x - p1.x)*(p2.y - p1.y); };
	for (auto it1 = a->colisionlines.begin(); it1 != a->colisionlines.end(); it1++)
	{
		for (auto it2 = b->colisionlines.begin(); it2 != b->colisionlines.end(); it2++)
		{
			Vector2 p1 = it1->first  + apos;
			Vector2 p2 = it1->second + apos;
			Vector2 p3 = it2->first  + bpos;
			Vector2 p4 = it2->second + bpos;
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
				return 1;
		}
	}
	return 0;
}
Vector2* colidable::getPos()
{
	return NULL;
}
void colidable::setBB()
{
	posPTR = getPos();
	if (colisionlines.size() == 0)
		return;
	Vector2 higher;
	auto it1 = colisionlines.begin();
	lower.x = std::min(it1->first.x, it1->second.x);
	lower.y = std::min(it1->first.y, it1->second.y);
	higher.x = std::max(it1->first.x, it1->second.x);
	higher.y = std::max(it1->first.y, it1->second.y);
	it1++;
	for (; it1 != colisionlines.end(); it1++)
	{
		lower.x = std::min(std::min(it1->first.x, it1->second.x),lower.x);
		lower.y = std::min(std::min(it1->first.y, it1->second.y),lower.y);
		higher.x = std::max(std::max(it1->first.x, it1->second.x),higher.x);
		higher.y = std::max(std::max(it1->first.y, it1->second.y),higher.y);
	}
	BB.height = higher.y - lower.y;
	BB.width = higher.x - lower.x;
}
void colidable::updateBB()
{
	BB.left = posPTR->x + lower.x;
	BB.top = posPTR->y + lower.y;
}