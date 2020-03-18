#include "core.h"

float rdn(float s)
{
	return(s / 180 * PI);
}

float stp(float r)
{
	return(r * 180 / PI);
}

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
	return false;
}

Vectorf New_animatable::count_pos(float x, float y, float size1, float size2,
	float translation_x1, float translation_y1, float angle1,
	float translation_x2, float translation_y2, float angle2)
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
	return Vectorf({ x + Lx1 - Lx2 + size1 / 2 - size2 / 2, y + Ly1 - Ly2 + size1 / 2 - size2 / 2 });
}

void New_animatable::animate(float x, float y, float r, float KLArGLO, float BRZrKLA,
	float MIErBRZ, float KLArPRA, float PRArPPR, float PPRrPDL,
	float KLArLRA, float LRArLPR, float LPRrLDL, float MIErPUD,
	float PUDrPLY, float PLYrPST, float MIErLUD, float LUDrLLY,
	float LLYrLST, float PPRrSKP, float LPRrSKL, float MIErOGO)
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
	float SP1r = PPRr + PPRrSKP;
	float SP1x = count_pos(PPRx, PPRy, 128, 128, stork_var.PPRxSK1, stork_var.PPRySK1, rdn(PPRr), stork_var.SKRxPRZ, stork_var.SKRyPRZ, rdn(SP1r)).x;
	float SP1y = count_pos(PPRx, PPRy, 128, 128, stork_var.PPRxSK1, stork_var.PPRySK1, rdn(PPRr), stork_var.SKRxPRZ, stork_var.SKRyPRZ, rdn(SP1r)).y;
	float SP2r = PPRr + PPRrSKP;
	float SP2x = count_pos(PPRx, PPRy, 128, 128, stork_var.PPRxSK2, stork_var.PPRySK2, rdn(PPRr), stork_var.SKRxPRZ, stork_var.SKRyPRZ, rdn(SP2r)).x;
	float SP2y = count_pos(PPRx, PPRy, 128, 128, stork_var.PPRxSK2, stork_var.PPRySK2, rdn(PPRr), stork_var.SKRxPRZ, stork_var.SKRyPRZ, rdn(SP2r)).y;
	float SP3r = PPRr + PPRrSKP;
	float SP3x = count_pos(PPRx, PPRy, 128, 128, stork_var.PPRxSK3, stork_var.PPRySK3, rdn(PPRr), stork_var.SKRxPRZ, stork_var.SKRyPRZ, rdn(SP3r)).x;
	float SP3y = count_pos(PPRx, PPRy, 128, 128, stork_var.PPRxSK3, stork_var.PPRySK3, rdn(PPRr), stork_var.SKRxPRZ, stork_var.SKRyPRZ, rdn(SP3r)).y;
	float SP4r = PPRr + PPRrSKP;
	float SP4x = count_pos(PPRx, PPRy, 128, 128, stork_var.PPRxSK4, stork_var.PPRySK4, rdn(PPRr), stork_var.SKRxPRZ, stork_var.SKRyPRZ, rdn(SP4r)).x;
	float SP4y = count_pos(PPRx, PPRy, 128, 128, stork_var.PPRxSK4, stork_var.PPRySK4, rdn(PPRr), stork_var.SKRxPRZ, stork_var.SKRyPRZ, rdn(SP4r)).y;
	float SP5r = PPRr + PPRrSKP;
	float SP5x = count_pos(PPRx, PPRy, 128, 128, stork_var.PPRxSK5, stork_var.PPRySK5, rdn(PPRr), stork_var.SKRxPRZ, stork_var.SKRyPRZ, rdn(SP5r)).x;
	float SP5y = count_pos(PPRx, PPRy, 128, 128, stork_var.PPRxSK5, stork_var.PPRySK5, rdn(PPRr), stork_var.SKRxPRZ, stork_var.SKRyPRZ, rdn(SP5r)).y;
	float SL1r = LPRr + LPRrSKL;
	float SL1x = count_pos(LPRx, LPRy, 128, 128, stork_var.LPRxSK1, stork_var.LPRySK1, rdn(LPRr), stork_var.SKRxPRZ, stork_var.SKRyPRZ, rdn(SL1r)).x;
	float SL1y = count_pos(LPRx, LPRy, 128, 128, stork_var.LPRxSK1, stork_var.LPRySK1, rdn(LPRr), stork_var.SKRxPRZ, stork_var.SKRyPRZ, rdn(SL1r)).y;
	float SL2r = LPRr + LPRrSKL;
	float SL2x = count_pos(LPRx, LPRy, 128, 128, stork_var.LPRxSK2, stork_var.LPRySK2, rdn(LPRr), stork_var.SKRxPRZ, stork_var.SKRyPRZ, rdn(SL2r)).x;
	float SL2y = count_pos(LPRx, LPRy, 128, 128, stork_var.LPRxSK2, stork_var.LPRySK2, rdn(LPRr), stork_var.SKRxPRZ, stork_var.SKRyPRZ, rdn(SL2r)).y;
	float SL3r = LPRr + LPRrSKL;
	float SL3x = count_pos(LPRx, LPRy, 128, 128, stork_var.LPRxSK3, stork_var.LPRySK3, rdn(LPRr), stork_var.SKRxPRZ, stork_var.SKRyPRZ, rdn(SL3r)).x;
	float SL3y = count_pos(LPRx, LPRy, 128, 128, stork_var.LPRxSK3, stork_var.LPRySK3, rdn(LPRr), stork_var.SKRxPRZ, stork_var.SKRyPRZ, rdn(SL3r)).y;
	float SL4r = LPRr + LPRrSKL;
	float SL4x = count_pos(LPRx, LPRy, 128, 128, stork_var.LPRxSK4, stork_var.LPRySK4, rdn(LPRr), stork_var.SKRxPRZ, stork_var.SKRyPRZ, rdn(SL4r)).x;
	float SL4y = count_pos(LPRx, LPRy, 128, 128, stork_var.LPRxSK4, stork_var.LPRySK4, rdn(LPRr), stork_var.SKRxPRZ, stork_var.SKRyPRZ, rdn(SL4r)).y;
	float SL5r = LPRr + LPRrSKL;
	float SL5x = count_pos(LPRx, LPRy, 128, 128, stork_var.LPRxSK5, stork_var.LPRySK5, rdn(LPRr), stork_var.SKRxPRZ, stork_var.SKRyPRZ, rdn(SL5r)).x;
	float SL5y = count_pos(LPRx, LPRy, 128, 128, stork_var.LPRxSK5, stork_var.LPRySK5, rdn(LPRr), stork_var.SKRxPRZ, stork_var.SKRyPRZ, rdn(SL5r)).y;
	float OGOr = MIEr + MIErOGO;
	float OGOx = count_pos(MIEx, MIEy, 128, 128, stork_var.MIExOGO, stork_var.MIEyOGO, rdn(MIEr), stork_var.OGOxMIE, stork_var.OGOyMIE, rdn(OGOr)).x;
	float OGOy = count_pos(MIEx, MIEy, 128, 128, stork_var.MIExOGO, stork_var.MIEyOGO, rdn(MIEr), stork_var.OGOxMIE, stork_var.OGOyMIE, rdn(OGOr)).y;

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
	parts[L_WING_1].setRotation(SL1r);
	parts[L_WING_2].setRotation(SL2r);
	parts[L_WING_3].setRotation(SL3r);
	parts[L_WING_4].setRotation(SL4r);
	parts[L_WING_5].setRotation(SL5r);
	parts[R_WING_1].setRotation(SP1r);
	parts[R_WING_2].setRotation(SP2r);
	parts[R_WING_3].setRotation(SP3r);
	parts[R_WING_4].setRotation(SP4r);
	parts[R_WING_5].setRotation(SP5r);
	parts[TAIL].setRotation(OGOr);

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
	parts[R_WING_1].setPosition(SP1x, SP1y);
	parts[R_WING_2].setPosition(SP2x, SP2y);
	parts[R_WING_3].setPosition(SP3x, SP3y);
	parts[R_WING_4].setPosition(SP4x, SP4y);
	parts[R_WING_5].setPosition(SP5x, SP5y);
	parts[L_WING_1].setPosition(SL1x, SL1y);
	parts[L_WING_2].setPosition(SL2x, SL2y);
	parts[L_WING_3].setPosition(SL3x, SL3y);
	parts[L_WING_4].setPosition(SL4x, SL4y);
	parts[L_WING_5].setPosition(SL5x, SL5y);
	parts[TAIL].setPosition(OGOx, OGOy);
}

New_animatable::New_animatable(std::vector<sf::Texture>& v, Vectorf p, float h, float gs) : pos(p), height(h)
{
	for (int i = 0; i < v.size(); i++)
	{
		parts.push_back(sf::Sprite(v[i]));
		parts[i].setOrigin(64, 64);
	}
	scale = gs * height / 500;
	if (!tex.create(500, 500))
		return;
}

void New_animatable::update()
{
	tex.clear(sf::Color());
	tex.draw(parts[L_ARM]);
	tex.draw(parts[L_WING_1]);
	tex.draw(parts[L_WING_2]);
	tex.draw(parts[L_WING_3]);
	tex.draw(parts[L_WING_4]);
	tex.draw(parts[L_WING_5]);
	tex.draw(parts[L_FOREARM]);
	tex.draw(parts[L_HAND]);
	tex.draw(parts[PELVIS]);
	tex.draw(parts[BELLY]);
	tex.draw(parts[CHEST]);
	tex.draw(parts[HEAD]);
	tex.draw(parts[L_CALF]);
	tex.draw(parts[L_FOOT]);
	tex.draw(parts[L_TIGH]);
	tex.draw(parts[TAIL]);
	tex.draw(parts[R_CALF]);
	tex.draw(parts[R_FOOT]);
	tex.draw(parts[R_TIGH]);
	tex.draw(parts[R_WING_1]);
	tex.draw(parts[R_WING_2]);
	tex.draw(parts[R_WING_3]);
	tex.draw(parts[R_WING_4]);
	tex.draw(parts[R_WING_5]);
	tex.draw(parts[R_FOREARM]);
	tex.draw(parts[R_ARM]);
	tex.draw(parts[R_HAND]);
	tex.display();
	sprite.setTexture(tex.getTexture());
	sprite.setScale(scale, scale);
}

void New_animatable::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(sprite, states);
}