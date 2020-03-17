#include "game.h"

Platform::Platform(Vectorf p, const sf::Texture* t, std::vector<sf::Vertex> points) : Texturable(p, t, points)
{
	float maxx, maxy, miny, minx;
	maxx = minx = vertices[0].position.x;
	maxy = miny = vertices[0].position.y;
	for (auto it : vertices)
	{
		mesh_collision.push_back({ it.position.x + pos.x, it.position.y + pos.y});
		if (it.position.x < minx)
			minx = it.position.x;
		if (it.position.y < miny)
			miny = it.position.y;
		if (it.position.x > maxx)
			maxx = it.position.x;
		if (it.position.y > maxy)
			maxy = it.position.y;
	}
	rect_collision = sf::FloatRect(minx + p.x, miny + p.y, maxx - minx, maxy - miny);
}

Player::Player(Vectorf p, std::vector<const Animation* > t, float h, float gs, float m) : Entity(p, t, h, gs, m) {}

Entity::Entity(Vectorf p, std::vector<const Animation* > t, float h, float gs, float m) : Animatable(p, t[0], h, gs), animations(t)
{
	status = Entity_status::IDLE;
	rect_collision = sf::FloatRect(tex->rect_collision.left * scale + p.x, tex->rect_collision.top * scale + p.y, tex->rect_collision.width * scale, tex->rect_collision.height * scale);
	mesh_collision = std::vector<Vectorf>();
	mesh_collision.push_back({ tex->rect_collision.left * scale + p.x, tex->rect_collision.top * scale + p.y });
	mesh_collision.push_back({ tex->rect_collision.left * scale + p.x + tex->rect_collision.width * scale, tex->rect_collision.top * scale + p.y });
	mesh_collision.push_back({ tex->rect_collision.left * scale + p.x + tex->rect_collision.width * scale, tex->rect_collision.top * scale + p.y + tex->rect_collision.height * scale });
	mesh_collision.push_back({ tex->rect_collision.left * scale + p.x, tex->rect_collision.top * scale + p.y + tex->rect_collision.height * scale });
	mass = m;
}

void Entity::move(Vectorf delta)
{
	speed += delta;
}

void Entity::jump()
{
	if (colision_direction.y == 1)
	{
		apply_force({ 0, -20 });
		if (status == Entity_status::IDLE || status == Entity_status::JUMP_IDLE)
		{
			status = Entity_status::JUMP_IDLE;
			reset_animation = true;
		}
		if (status == Entity_status::MOVE || status == Entity_status::JUMP_RUN)
		{
			status = Entity_status::JUMP_RUN;
			reset_animation = true;
		}

	}
}

void Entity::next_frame()
{
	if (&*tex != &*animations[status] || reset_animation)
	{
		set_animation(animations[status]);
		sf::Vector2u size = it->getSize();
		sprite.setTextureRect(sf::IntRect(0, 0, size.x, size.y));
		reset_animation = false;
	}
	if (++it == tex->end())
	{
		if (status != Entity_status::MOVE)
		{
			status = Entity_status::IDLE;
			set_animation(animations[status]);
		}
		else
			it = tex->begin();
	}
	sprite.setTexture(*it);
}

void Entity::update()
{
	int s = sgn(speed.x);
	if (direction != s && s != 0)
	{
		Vectorf tmp = sprite.getScale();
		tmp.x *= -1;
		scale = -scale;
		speed = { speed.x * 2, speed.y * 2 };
		if (s == -1)
		{
			sprite.setOrigin(sprite.getLocalBounds().width, 0);
			sprite.setScale(-1, 1);
		}
		else
		{
			sprite.setOrigin(0, 0);
			sprite.setScale(1, 1);
		}
		sprite.setScale(tmp);
		direction = s;
	}
	if (force.x > max_force)
		force.x = max_force;
	if (force.y > max_force)
		force.y = max_force;
	speed += force;
	last_speed = speed;
	update_position();
	if (last_speed.x != 0 && last_speed.y >= 0 && colision_direction.y == 1)
	{
		status = Entity_status::MOVE;
	}
	if (last_speed.x == 0 && last_speed.y >= 0 && colision_direction.y == 1)
	{
		status = Entity_status::IDLE;
	}
	colision_direction = { 0,0 };
}

void Entity::update_position()
{
	pos += speed;
	sprite.setPosition(pos);
	if (scale > 0)
	{
		rect_collision = sf::FloatRect(tex->rect_collision.left * scale + pos.x, tex->rect_collision.top * scale + pos.y, tex->rect_collision.width * scale, tex->rect_collision.height * scale);
	}
	else
	{
		rect_collision = sf::FloatRect((tex->rect_collision.left) * -scale + pos.x, (540 - tex->rect_collision.top) * -scale + pos.y, tex->rect_collision.width * -scale, tex->rect_collision.height * scale);
	}	
	mesh_collision = std::vector<Vectorf>();
	mesh_collision.push_back({ rect_collision.left, rect_collision.top});
	mesh_collision.push_back({ rect_collision.left + rect_collision.width, rect_collision.top});
	mesh_collision.push_back({ rect_collision.left + rect_collision.width, rect_collision.top + rect_collision.height});
	mesh_collision.push_back({ rect_collision.left, rect_collision.top + rect_collision.height });
	speed = { 0,0 };
}
Vectorf Entity::get_position()
{
	return pos;
}
void Entity::set_animation(const Animation* t)
{
	//this->pos += -(t->center - tex->center) * this->scale;
	//this->sprite.move(-(t->center - tex->center) * this->scale);
	rect_collision = sf::FloatRect(tex->rect_collision.left * scale + pos.x, tex->rect_collision.top * scale + pos.y, tex->rect_collision.width * scale, tex->rect_collision.height * scale);
	mesh_collision = std::vector<Vectorf>();
	mesh_collision.push_back({ rect_collision.left, rect_collision.top });
	mesh_collision.push_back({ rect_collision.left + rect_collision.width, rect_collision.top });
	mesh_collision.push_back({ rect_collision.left + rect_collision.width, rect_collision.top + rect_collision.height });
	mesh_collision.push_back({ rect_collision.left, rect_collision.top + rect_collision.height });
	tex = t;
	it = tex->begin();
}


Vectorf New_animatable::count_pos(int x, int y, int size1, int size2, int translation_x1, int translation_y1, float angle1, int translation_x2, int translation_y2, float angle2)
{
	float d1 = sqrt(pow(translation_x1 - size1 / 2, 2) + pow(translation_y1 - size1 / 2, 2));
	float d2 = sqrt(pow(translation_x2 - size2 / 2, 2) + pow(translation_y2 - size2 / 2, 2));
	float sinalfa = (translation_y1 - size1 / 2) / d1;
	float cosalfa = (translation_x1 - size1 / 2) / d1;
	float sinbeta = (translation_y2 - size2 / 2) / d2;
	float cosbeta = (translation_x2 - size2 / 2) / d2;
	float Lx1 = (cosalfa * cos(angle1) - sinalfa * sin(angle1)) * d1;
	float Ly1 = (sinalfa * cos(angle1) + cosalfa * sin(angle1)) * d1;
	float Lx2 = (cosbeta * cos(angle2) - sinbeta * sin(angle2)) * d2;
	float Ly2 = (sinbeta * cos(angle2) + cosbeta * sin(angle2)) * d2;
	return Vectorf({ x + Lx1 - Lx2 + size1 / 2 - size2 / 2 , y + Ly1 - Ly2 + size1 / 2 - size2 / 2 });
}

void New_animatable::animate(int x, int y, float r, float KLArGLO, float BRZrKLA, float MIErBRZ, float KLArPRA, float PRArPPR, float PPRrPDL, float KLArLRA, float LRArLPR, float LPRrLDL, float MIErPUD, float PUDrPLY, float PLYrPST, float MIErLUD, float LUDrLLY, float LLYrLST, float PPRrSKP, float LPRrSKL, float MIErOGO)
{
	float MIEr = r;
	float MIEx = x;
	float MIEy = y;
	float BRZr = MIEr + MIErBRZ;
	float BRZx = count_pos(MIEx, MIEy, 128, 128, stork_var.MIExBRZ, stork_var.MIEyBRZ, rdn(MIEr), stork_var.BRZxMIE, stork_var.BRZyMIE, rdn(BRZr)).x;
	float BRZy = count_pos(MIEx, MIEy, 128, 128, stork_var.MIExBRZ, stork_var.MIEyBRZ, rdn(MIEr), stork_var.BRZxMIE, stork_var.BRZyMIE, rdn(BRZr)).y;
	float KLAr = BRZr + BRZrKLA;
	float KLAx = count_pos(BRZx, BRZy, 128, 128, stork_var.BRZxKLA, stork_var.BRZyKLA, rdn(BRZr), stork_var.KLAxBRZ, stork_var.KLAyBRZ, rdn(KLAr)).x;
	float KLAy = count_pos(BRZx, BRZy, 128, 128, stork_var.BRZxKLA, stork_var.BRZyKLA, rdn(BRZr), stork_var.KLAxBRZ, stork_var.KLAyBRZ, rdn(KLAr)).y;
	float GLOr = KLAr + KLArGLO;
	float GLOx = count_pos(KLAx, KLAy, 128, 128, stork_var.KLAxGLO, stork_var.KLAyGLO, rdn(KLAr), stork_var.GLOxKLA, stork_var.GLOyKLA, rdn(GLOr)).x;
	float GLOy = count_pos(KLAx, KLAy, 128, 128, stork_var.KLAxGLO, stork_var.KLAyGLO, rdn(KLAr), stork_var.GLOxKLA, stork_var.GLOyKLA, rdn(GLOr)).y;
	float PRAr = KLAr + KLArPRA;
	float PRAx = count_pos(KLAx, KLAy, 128, 128, stork_var.KLAxPRA, stork_var.KLAyPRA, rdn(KLAr), stork_var.PRAxKLA, stork_var.PRAyKLA, rdn(PRAr)).x;
	float PRAy = count_pos(KLAx, KLAy, 128, 128, stork_var.KLAxPRA, stork_var.KLAyPRA, rdn(KLAr), stork_var.PRAxKLA, stork_var.PRAyKLA, rdn(PRAr)).y;
	float LRAr = KLAr + KLArLRA;
	float LRAx = count_pos(KLAx, KLAy, 128, 128, stork_var.KLAxLRA, stork_var.KLAyLRA, rdn(KLAr), stork_var.LRAxKLA, stork_var.LRAyKLA, rdn(LRAr)).x;
	float LRAy = count_pos(KLAx, KLAy, 128, 128, stork_var.KLAxLRA, stork_var.KLAyLRA, rdn(KLAr), stork_var.LRAxKLA, stork_var.LRAyKLA, rdn(LRAr)).y;
	float PPRr = PRAr + PRArPPR;
	float PPRx = count_pos(PRAx, PRAy, 128, 128, stork_var.PRAxPPR, stork_var.PRAyPPR, rdn(PRAr), stork_var.PPRxPRA, stork_var.PPRyPRA, rdn(PPRr)).x;
	float PPRy = count_pos(PRAx, PRAy, 128, 128, stork_var.PRAxPPR, stork_var.PRAyPPR, rdn(PRAr), stork_var.PPRxPRA, stork_var.PPRyPRA, rdn(PPRr)).y;
	float LPRr = LRAr + LRArLPR;
	float LPRx = count_pos(LRAx, LRAy, 128, 128, stork_var.LRAxLPR, stork_var.LRAyLPR, rdn(LRAr), stork_var.LPRxLRA, stork_var.LPRyLRA, rdn(LPRr)).x;
	float LPRy = count_pos(LRAx, LRAy, 128, 128, stork_var.LRAxLPR, stork_var.LRAyLPR, rdn(LRAr), stork_var.LPRxLRA, stork_var.LPRyLRA, rdn(LPRr)).y;
	float PDLr = PPRr + PPRrPDL;
	float PDLx = count_pos(PPRx, PPRy, 128, 128, stork_var.PPRxPDL, stork_var.PPRyPDL, rdn(PPRr), stork_var.PDLxPPR, stork_var.PDLyPPR, rdn(PDLr)).x;
	float PDLy = count_pos(PPRx, PPRy, 128, 128, stork_var.PPRxPDL, stork_var.PPRyPDL, rdn(PPRr), stork_var.PDLxPPR, stork_var.PDLyPPR, rdn(PDLr)).y;
	float LDLr = LPRr + LPRrLDL;
	float LDLx = count_pos(LPRx, LPRy, 128, 128, stork_var.LPRxLDL, stork_var.LPRyLDL, rdn(LPRr), stork_var.LDLxLPR, stork_var.LDLyLPR, rdn(LDLr)).x;
	float LDLy = count_pos(LPRx, LPRy, 128, 128, stork_var.LPRxLDL, stork_var.LPRyLDL, rdn(LPRr), stork_var.LDLxLPR, stork_var.LDLyLPR, rdn(LDLr)).y;
	float PUDr = MIEr + MIErPUD;
	float PUDx = count_pos(MIEx, MIEy, 128, 128, stork_var.MIExPUD, stork_var.MIEyPUD, rdn(MIEr), stork_var.PUDxMIE, stork_var.PUDyMIE, rdn(PUDr)).x;
	float PUDy = count_pos(MIEx, MIEy, 128, 128, stork_var.MIExPUD, stork_var.MIEyPUD, rdn(MIEr), stork_var.PUDxMIE, stork_var.PUDyMIE, rdn(PUDr)).y;
	float LUDr = MIEr + MIErLUD;
	float LUDx = count_pos(MIEx, MIEy, 128, 128, stork_var.MIExLUD, stork_var.MIEyLUD, rdn(MIEr), stork_var.LUDxMIE, stork_var.LUDyMIE, rdn(LUDr)).x;
	float LUDy = count_pos(MIEx, MIEy, 128, 128, stork_var.MIExLUD, stork_var.MIEyLUD, rdn(MIEr), stork_var.LUDxMIE, stork_var.LUDyMIE, rdn(LUDr)).y;
	float PLYr = PUDr + PUDrPLY;
	float PLYx = count_pos(PUDx, PUDy, 128, 128, stork_var.PUDxPLY, stork_var.PUDyPLY, rdn(PUDr), stork_var.PLYxPUD, stork_var.PLYyPUD, rdn(PLYr)).x;
	float PLYy = count_pos(PUDx, PUDy, 128, 128, stork_var.PUDxPLY, stork_var.PUDyPLY, rdn(PUDr), stork_var.PLYxPUD, stork_var.PLYyPUD, rdn(PLYr)).y;
	float LLYr = LUDr + LUDrLLY;
	float LLYx = count_pos(LUDx, LUDy, 128, 128, stork_var.LUDxLLY, stork_var.LUDyLLY, rdn(LUDr), stork_var.LLYxLUD, stork_var.LLYyLUD, rdn(LLYr)).x;
	float LLYy = count_pos(LUDx, LUDy, 128, 128, stork_var.LUDxLLY, stork_var.LUDyLLY, rdn(LUDr), stork_var.LLYxLUD, stork_var.LLYyLUD, rdn(LLYr)).y;
	float PSTr = PLYr + PLYrPST;
	float PSTx = count_pos(PLYx, PLYy, 128, 128, stork_var.PLYxPST, stork_var.PLYyPST, rdn(PLYr), stork_var.PSTxPLY, stork_var.PSTyPLY, rdn(PSTr)).x;
	float PSTy = count_pos(PLYx, PLYy, 128, 128, stork_var.PLYxPST, stork_var.PLYyPST, rdn(PLYr), stork_var.PSTxPLY, stork_var.PSTyPLY, rdn(PSTr)).y;
	float LSTr = LLYr + LLYrLST;
	float LSTx = count_pos(LLYx, LLYy, 128, 128, stork_var.LLYxLST, stork_var.LLYyLST, rdn(LLYr), stork_var.LSTxLLY, stork_var.LSTyLLY, rdn(LSTr)).x;
	float LSTy = count_pos(LLYx, LLYy, 128, 128, stork_var.LLYxLST, stork_var.LLYyLST, rdn(LLYr), stork_var.LSTxLLY, stork_var.LSTyLLY, rdn(LSTr)).y;
	parts[BELLY].setRotation(BRZr);
	parts[L_HAND].setRotation(LDLr);
	parts[R_HAND].setRotation(PDLr);
	parts[HEAD].setRotation(GLOr);
	parts[CHEST].setRotation(KLAr);
	parts[L_CALF].setRotation(LLYr);
	parts[R_CALF].setRotation(PLYr);
	parts[PELVIS].setRotation(MIEr);
	parts[L_FOREARM].setRotation(LPRr);
	parts[R_FOREARM].setRotation(PPRr);
	parts[L_ARM].setRotation(LRAr);
	parts[R_ARM].setRotation(PRAr);
	parts[L_FOOT].setRotation(LSTr);
	parts[R_FOOT].setRotation(PSTr);
	parts[L_TIGH].setRotation(LUDr);
	parts[R_TIGH].setRotation(PUDr);
	parts[BELLY].setPosition(BRZx, BRZy);
	parts[L_HAND].setPosition(LDLx, LDLy);
	parts[R_HAND].setPosition(PDLx, PDLy);
	parts[HEAD].setPosition(GLOx, GLOy);
	parts[CHEST].setPosition(KLAx, KLAy);
	parts[L_CALF].setPosition(LLYx, LLYy);
	parts[R_CALF].setPosition(PLYx, PLYy);
	parts[PELVIS].setPosition(MIEx, MIEy);
	parts[L_FOREARM].setPosition(LPRx, LPRy);
	parts[R_FOREARM].setPosition(PPRx, PPRy);
	parts[L_ARM].setPosition(LRAx, LRAy);
	parts[R_ARM].setPosition(PRAx, PRAy);
	parts[L_FOOT].setPosition(LSTx, LSTy);
	parts[R_FOOT].setPosition(PSTx, PSTy);
	parts[L_TIGH].setPosition(LUDx, LUDy);
	parts[R_TIGH].setPosition(PUDx, PUDy);
}

New_animatable::New_animatable(Assets& a, Vectorf p) : pos(p)
{
	parts.push_back(sf::Sprite(a.pieces.brz));
	parts.push_back(sf::Sprite(a.pieces.ldl));
	parts.push_back(sf::Sprite(a.pieces.pdl));
	parts.push_back(sf::Sprite(a.pieces.glo));
	parts.push_back(sf::Sprite(a.pieces.klp));
	parts.push_back(sf::Sprite(a.pieces.lyd));
	parts.push_back(sf::Sprite(a.pieces.lyd));
	parts.push_back(sf::Sprite(a.pieces.mie));
	parts.push_back(sf::Sprite(a.pieces.prr));
	parts.push_back(sf::Sprite(a.pieces.prr));
	parts.push_back(sf::Sprite(a.pieces.ram));
	parts.push_back(sf::Sprite(a.pieces.ram));
	parts.push_back(sf::Sprite(a.pieces.sto));
	parts.push_back(sf::Sprite(a.pieces.sto));
	parts.push_back(sf::Sprite(a.pieces.udo));
	parts.push_back(sf::Sprite(a.pieces.udo));
}

void New_animatable::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const auto& it : parts)
		target.draw(it);
}
