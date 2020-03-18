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

Vectorf New_animatable::count_pos(Vectorf start, float size1, float size2,
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
	return Vectorf({ start.x + Lx1 - Lx2 + size1 / 2 - size2 / 2, start.y + Ly1 - Ly2 + size1 / 2 - size2 / 2 });
}

void New_animatable::animate(xyr start, float KLArGLO, float BRZrKLA,
	float MIErBRZ, float KLArPRA, float PRArPPR, float PPRrPDL,
	float KLArLRA, float LRArLPR, float LPRrLDL, float MIErPUD,
	float PUDrPLY, float PLYrPST, float MIErLUD, float LUDrLLY,
	float LLYrLST, float PPRrSKP, float LPRrSKL, float MIErOGO)
{
	xyr MIE, BRZ, KLA, GLO, LRA, PRA, PLY, LLY, PUD, LUD, LPR, PPR,
		LST, PST, LDL, PDL, SL1, SL2, SL3, SL4, SL5, SP1, SP2, SP3,
		SP4, SP5, OGO;
	MIE.r = start.r;
	MIE.pos.x = start.pos.x;
	MIE.pos.y = start.pos.y;
	BRZ.r = MIE.r + MIErBRZ;
	BRZ.pos = count_pos(MIE.pos, 128, 128, stork_var.MIExBRZ, stork_var.MIEyBRZ, rdn(MIE.r), stork_var.BRZxMIE, stork_var.BRZyMIE, rdn(BRZ.r));
	KLA.r = BRZ.r + BRZrKLA;
	KLA.pos = count_pos(BRZ.pos, 128, 128, stork_var.BRZxKLA, stork_var.BRZyKLA, rdn(BRZ.r), stork_var.KLAxBRZ, stork_var.KLAyBRZ, rdn(KLA.r));
	GLO.r = KLA.r + KLArGLO;
	GLO.pos = count_pos(KLA.pos, 128, 128, stork_var.KLAxGLO, stork_var.KLAyGLO, rdn(KLA.r), stork_var.GLOxKLA, stork_var.GLOyKLA, rdn(GLO.r));
	PRA.r = KLA.r + KLArPRA;
	PRA.pos = count_pos(KLA.pos, 128, 128, stork_var.KLAxPRA, stork_var.KLAyPRA, rdn(KLA.r), stork_var.PRAxKLA, stork_var.PRAyKLA, rdn(PRA.r));
	LRA.r = KLA.r + KLArLRA;
	LRA.pos = count_pos(KLA.pos, 128, 128, stork_var.KLAxLRA, stork_var.KLAyLRA, rdn(KLA.r), stork_var.LRAxKLA, stork_var.LRAyKLA, rdn(LRA.r));
	PPR.r = PRA.r + PRArPPR;
	PPR.pos = count_pos(PRA.pos, 128, 128, stork_var.PRAxPPR, stork_var.PRAyPPR, rdn(PRA.r), stork_var.PPRxPRA, stork_var.PPRyPRA, rdn(PPR.r));
	LPR.r = LRA.r + LRArLPR;
	LPR.pos = count_pos(LRA.pos, 128, 128, stork_var.LRAxLPR, stork_var.LRAyLPR, rdn(LRA.r), stork_var.LPRxLRA, stork_var.LPRyLRA, rdn(LPR.r));
	PDL.r = PPR.r + PPRrPDL;
	PDL.pos = count_pos(PPR.pos, 128, 128, stork_var.PPRxPDL, stork_var.PPRyPDL, rdn(PPR.r), stork_var.PDLxPPR, stork_var.PDLyPPR, rdn(PDL.r));
	LDL.r = LPR.r + LPRrLDL;
	LDL.pos = count_pos(LPR.pos, 128, 128, stork_var.LPRxLDL, stork_var.LPRyLDL, rdn(LPR.r), stork_var.LDLxLPR, stork_var.LDLyLPR, rdn(LDL.r));
	PUD.r = MIE.r + MIErPUD;
	PUD.pos = count_pos(MIE.pos, 128, 128, stork_var.MIExPUD, stork_var.MIEyPUD, rdn(MIE.r), stork_var.PUDxMIE, stork_var.PUDyMIE, rdn(PUD.r));
	LUD.r = MIE.r + MIErLUD;																												  
	LUD.pos = count_pos(MIE.pos, 128, 128, stork_var.MIExLUD, stork_var.MIEyLUD, rdn(MIE.r), stork_var.LUDxMIE, stork_var.LUDyMIE, rdn(LUD.r));
	PLY.r = PUD.r + PUDrPLY;																												  
	PLY.pos = count_pos(PUD.pos, 128, 128, stork_var.PUDxPLY, stork_var.PUDyPLY, rdn(PUD.r), stork_var.PLYxPUD, stork_var.PLYyPUD, rdn(PLY.r));
	LLY.r = LUD.r + LUDrLLY;																												  
	LLY.pos = count_pos(LUD.pos, 128, 128, stork_var.LUDxLLY, stork_var.LUDyLLY, rdn(LUD.r), stork_var.LLYxLUD, stork_var.LLYyLUD, rdn(LLY.r));
	PST.r = PLY.r + PLYrPST;																												  
	PST.pos = count_pos(PLY.pos, 128, 128, stork_var.PLYxPST, stork_var.PLYyPST, rdn(PLY.r), stork_var.PSTxPLY, stork_var.PSTyPLY, rdn(PST.r));
	LST.r = LLY.r + LLYrLST;																												  
	LST.pos = count_pos(LLY.pos, 128, 128, stork_var.LLYxLST, stork_var.LLYyLST, rdn(LLY.r), stork_var.LSTxLLY, stork_var.LSTyLLY, rdn(LST.r));
	SP1.r = PPR.r + PPRrSKP;																												  
	SP1.pos = count_pos(PPR.pos, 128, 128, stork_var.PPRxSK1, stork_var.PPRySK1, rdn(PPR.r), stork_var.SKRxPRZ, stork_var.SKRyPRZ, rdn(SP1.r));
	SP2.r = PPR.r + PPRrSKP;																												  
	SP2.pos = count_pos(PPR.pos, 128, 128, stork_var.PPRxSK2, stork_var.PPRySK2, rdn(PPR.r), stork_var.SKRxPRZ, stork_var.SKRyPRZ, rdn(SP2.r));
	SP3.r = PPR.r + PPRrSKP;																												  
	SP3.pos = count_pos(PPR.pos, 128, 128, stork_var.PPRxSK3, stork_var.PPRySK3, rdn(PPR.r), stork_var.SKRxPRZ, stork_var.SKRyPRZ, rdn(SP3.r));
	SP4.r = PPR.r + PPRrSKP;																												  
	SP4.pos = count_pos(PPR.pos, 128, 128, stork_var.PPRxSK4, stork_var.PPRySK4, rdn(PPR.r), stork_var.SKRxPRZ, stork_var.SKRyPRZ, rdn(SP4.r));
	SP5.r = PPR.r + PPRrSKP;																												  
	SP5.pos = count_pos(PPR.pos, 128, 128, stork_var.PPRxSK5, stork_var.PPRySK5, rdn(PPR.r), stork_var.SKRxPRZ, stork_var.SKRyPRZ, rdn(SP5.r));
	SL1.r = LPR.r + LPRrSKL;																												  
	SL1.pos = count_pos(LPR.pos, 128, 128, stork_var.LPRxSK1, stork_var.LPRySK1, rdn(LPR.r), stork_var.SKRxPRZ, stork_var.SKRyPRZ, rdn(SL1.r));
	SL2.r = LPR.r + LPRrSKL;																												  
	SL2.pos = count_pos(LPR.pos, 128, 128, stork_var.LPRxSK2, stork_var.LPRySK2, rdn(LPR.r), stork_var.SKRxPRZ, stork_var.SKRyPRZ, rdn(SL2.r));
	SL3.r = LPR.r + LPRrSKL;																												  
	SL3.pos = count_pos(LPR.pos, 128, 128, stork_var.LPRxSK3, stork_var.LPRySK3, rdn(LPR.r), stork_var.SKRxPRZ, stork_var.SKRyPRZ, rdn(SL3.r));
	SL4.r = LPR.r + LPRrSKL;																														  
	SL4.pos = count_pos(LPR.pos, 128, 128, stork_var.LPRxSK4, stork_var.LPRySK4, rdn(LPR.r), stork_var.SKRxPRZ, stork_var.SKRyPRZ, rdn(SL4.r));
	SL5.r = LPR.r + LPRrSKL;																	  
	SL5.pos = count_pos(LPR.pos, 128, 128, stork_var.LPRxSK5, stork_var.LPRySK5, rdn(LPR.r), stork_var.SKRxPRZ, stork_var.SKRyPRZ, rdn(SL5.r));
	OGO.r = MIE.r + MIErOGO;																	  
	OGO.pos = count_pos(MIE.pos, 128, 128, stork_var.MIExOGO, stork_var.MIEyOGO, rdn(MIE.r), stork_var.OGOxMIE, stork_var.OGOyMIE, rdn(OGO.r));

	parts[BELLY].setRotation(BRZ.r);
	parts[L_HAND].setRotation(LDL.r);
	parts[R_HAND].setRotation(PDL.r);
	parts[HEAD].setRotation(GLO.r);
	parts[CHEST].setRotation(KLA.r);
	parts[L_CALF].setRotation(LLY.r);
	parts[R_CALF].setRotation(PLY.r);
	parts[PELVIS].setRotation(MIE.r);
	parts[L_FOREARM].setRotation(LPR.r);
	parts[R_FOREARM].setRotation(PPR.r);
	parts[L_ARM].setRotation(LRA.r);
	parts[R_ARM].setRotation(PRA.r);
	parts[L_FOOT].setRotation(LST.r);
	parts[R_FOOT].setRotation(PST.r);
	parts[L_TIGH].setRotation(LUD.r);
	parts[R_TIGH].setRotation(PUD.r);
	parts[L_WING_1].setRotation(SL1.r);
	parts[L_WING_2].setRotation(SL2.r);
	parts[L_WING_3].setRotation(SL3.r);
	parts[L_WING_4].setRotation(SL4.r);
	parts[L_WING_5].setRotation(SL5.r);
	parts[R_WING_1].setRotation(SP1.r);
	parts[R_WING_2].setRotation(SP2.r);
	parts[R_WING_3].setRotation(SP3.r);
	parts[R_WING_4].setRotation(SP4.r);
	parts[R_WING_5].setRotation(SP5.r);
	parts[TAIL].setRotation(OGO.r);

	parts[BELLY].setPosition(BRZ.pos.x, BRZ.pos.y);
	parts[L_HAND].setPosition(LDL.pos.x, LDL.pos.y);
	parts[R_HAND].setPosition(PDL.pos.x, PDL.pos.y);
	parts[HEAD].setPosition(GLO.pos.x, GLO.pos.y);
	parts[CHEST].setPosition(KLA.pos.x, KLA.pos.y);
	parts[L_CALF].setPosition(LLY.pos.x, LLY.pos.y);
	parts[R_CALF].setPosition(PLY.pos.x, PLY.pos.y);
	parts[PELVIS].setPosition(MIE.pos.x, MIE.pos.y);
	parts[L_FOREARM].setPosition(LPR.pos.x, LPR.pos.y);
	parts[R_FOREARM].setPosition(PPR.pos.x, PPR.pos.y);
	parts[L_ARM].setPosition(LRA.pos.x, LRA.pos.y);
	parts[R_ARM].setPosition(PRA.pos.x, PRA.pos.y);
	parts[L_FOOT].setPosition(LST.pos.x, LST.pos.y);
	parts[R_FOOT].setPosition(PST.pos.x, PST.pos.y);
	parts[L_TIGH].setPosition(LUD.pos.x, LUD.pos.y);
	parts[R_TIGH].setPosition(PUD.pos.x, PUD.pos.y);
	parts[R_WING_1].setPosition(SP1.pos.x, SP1.pos.y);
	parts[R_WING_2].setPosition(SP2.pos.x, SP2.pos.y);
	parts[R_WING_3].setPosition(SP3.pos.x, SP3.pos.y);
	parts[R_WING_4].setPosition(SP4.pos.x, SP4.pos.y);
	parts[R_WING_5].setPosition(SP5.pos.x, SP5.pos.y);
	parts[L_WING_1].setPosition(SL1.pos.x, SL1.pos.y);
	parts[L_WING_2].setPosition(SL2.pos.x, SL2.pos.y);
	parts[L_WING_3].setPosition(SL3.pos.x, SL3.pos.y);
	parts[L_WING_4].setPosition(SL4.pos.x, SL4.pos.y);
	parts[L_WING_5].setPosition(SL5.pos.x, SL5.pos.y);
	parts[TAIL].setPosition(OGO.pos.x, OGO.pos.y);
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