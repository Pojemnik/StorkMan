#include "util.h"

struct Context context;

std::stringstream util::remove_comments(std::ifstream& file)
{
	std::stringstream ss;
	while (!file.eof())
	{
		string s;
		std::getline(file, s);
		const char* white_space = " \t\v\r\n";
		std::size_t start = s.find_first_not_of(white_space);
		if (start == string::npos)
			continue;
		s = s.substr(start);
		if (s[0] != '#')
		{
			ss << s << '\n';
		}
	}
	return ss;
}

std::string util::pass_or_default(std::string val, std::string default_val)
{
	return val == "" ? default_val : val;
}

sf::Vector2f util::normalize(sf::Vector2f x, float l)
{
	return x / std::hypot(x.x, x.y) * l;
}

sf::Vector2f util::get_axis_normal(const std::vector<sf::Vector2f>& a,
	size_t i)
{
	Vectorf p1 = a[i];
	Vectorf p2 = (i >= a.size() - 1) ? a[0] : a[i + 1];
	return util::normalize({ p1.y - p2.y,p2.x - p1.x }, 1);
}

float util::vector_dot_product(sf::Vector2f a, sf::Vector2f b)
{
	return a.x * b.x + a.y * b.y;
}

float util::vector_cross_product(sf::Vector2f a, sf::Vector2f b)
{
	return a.x * b.y - a.y * b.x;
}

Vectorf util::saturate(Vectorf val, const Vectorf max_val)
{
	if (val.x > max_val.x)
		val.x = max_val.x;
	if (val.x < -max_val.x)
		val.x = -max_val.x;
	if (val.y > max_val.y)
		val.y = max_val.y;
	if (val.y < -max_val.y)
		val.y = -max_val.y;
	return val;
}

float util::deg_to_rad(float s)
{
	return(s / 180 * PI);
}

float util::rad_to_deg(float rdn)
{
	return rdn / PI * 180;
}

float util::ang_reduce(float ang)
{
	ang = fmod(ang, 360.0f);
	if (ang < 0)
		ang += 360.0f;
	return ang;
}

Vectorf util::rotate_vector(Vectorf vec, float ang)
{
	float COS = cos(ang);
	float SIN = sin(ang);
	return { vec.x * COS -
			vec.y * SIN,
			vec.x * SIN +
			vec.y * COS };
}
Vectorf util::rotate_vector(Vectorf vec, Vectorf normalized_rotation_vector)
{
	return { vec.x * normalized_rotation_vector.x -
			vec.y * normalized_rotation_vector.y,
			vec.x * normalized_rotation_vector.y +
			vec.y * normalized_rotation_vector.x };
}

bool util::vectorf_compare(const Vectorf& a, const Vectorf& b)
{
	if (a.x != b.x)
	{
		return a.x > b.x;
	}
	else
	{
		return a.y > b.y;
	}
}

bool util::vectorf_binary_predicate(const Vectorf& a,
	const Vectorf& b)
{
	return fabs(a.x - b.x) < 1 && fabs(a.y - b.y) < 1;
}

float util::convert_vector(const Vectorf& vec)
{
	float SIN = vec.y / hypot(vec.x, vec.y);
	if (vec.x > 0)
		return SIN - 1;
	else
		return 1 - SIN;
}

bool util::round_and_compare(Vectorf a, Vectorf b)
{
	a = { float(round(a.x)), float(round(a.y)) };
	b = { float(round(b.x)), float(round(b.y)) };
	return a == b;
}

Vectorf util::intersection_point(std::pair<Vectorf, Vectorf> a, std::pair<Vectorf, Vectorf> b)
{
	float a1 = a.second.y - a.first.y;
	float b1 = a.first.x - a.second.x;
	float c1 = a1 * (a.first.x) + b1 * (a.first.y);

	float a2 = b.second.y - b.first.y;
	float b2 = b.first.x - b.second.x;
	float c2 = a2 * (b.first.x) + b2 * (b.first.y);

	float determinant = a1 * b2 - a2 * b1;

	if (determinant == 0)
	{
		return Vectorf(FLT_MAX, FLT_MAX);
	}
	else
	{
		float x = (b2 * c1 - b1 * c2) / determinant;
		float y = (a1 * c2 - a2 * c1) / determinant;
		if (x >= std::min(a.first.x, a.second.x) && x <= std::max(a.first.x, a.second.x) &&
			x >= std::min(b.first.x, b.second.x) && x <= std::max(b.first.x, b.second.x) &&
			y >= std::min(a.first.y, a.second.y) && y <= std::max(a.first.y, a.second.y) &&
			y >= std::min(b.first.y, b.second.y) && y <= std::max(b.first.y, b.second.y))
		{
			return Vectorf(x, y);
		}
		else
		{
			return Vectorf(FLT_MAX, FLT_MAX);
		}
	}
}

bool util::on_segment(Vectorf p, Vectorf q, Vectorf r)
{
	if (q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) &&
		q.y <= std::max(p.y, r.y) && q.y >= std::min(p.y, r.y))
		return true;

	return false;
}

bool util::are_colinear(Vectorf p, Vectorf q, Vectorf r, float epsilon)
{
	const Vectorf v1 = { r.x - q.x, r.y - q.y };
	const Vectorf v2 = { p.x - q.x, p.y - q.y };
	const float area = v1.x * v2.y - v1.y * v2.x;
	return (fabs(area) < epsilon);
}

int util::orientation(Vectorf p, Vectorf q, Vectorf r)
{
	int val = (q.y - p.y) * (r.x - q.x) -
		(q.x - p.x) * (r.y - q.y);

	if (val == 0) return 0;

	return (val > 0) ? 1 : 2;
}

bool util::intersection(std::pair<Vectorf, Vectorf> a, std::pair<Vectorf, Vectorf> b)
{
	int o1 = orientation(a.first, a.second, b.first);
	int o2 = orientation(a.first, a.second, b.second);
	int o3 = orientation(b.first, b.second, a.first);
	int o4 = orientation(b.first, b.second, a.second);

	if (o1 != o2 && o3 != o4)
		return true;

	if (o1 == 0 && on_segment(a.first, b.first, a.second)) return true;
	if (o2 == 0 && on_segment(a.first, b.second, a.second)) return true;
	if (o3 == 0 && on_segment(b.first, a.first, b.second)) return true;
	if (o4 == 0 && on_segment(b.first, a.second, b.second)) return true;

	return false;
}

void util::save_texture(std::string path, sf::Texture* texture)
{
	sf::Image im = texture->copyToImage();
	im.saveToFile(path);
}

sf::FloatRect util::mesh_to_rect(const std::vector<Vectorf>& vertices)
{
	float maxx, maxy, miny, minx;
	maxx = minx = vertices[0].x;
	maxy = miny = vertices[0].y;
	for (const auto& it : vertices)
	{
		if (it.x < minx)
			minx = it.x;
		if (it.y < miny)
			miny = it.y;
		if (it.x > maxx)
			maxx = it.x;
		if (it.y > maxy)
			maxy = it.y;
	}
	return sf::FloatRect(minx, miny, maxx - minx, maxy - miny);
}

sf::FloatRect util::mesh_to_rect(const std::vector<sf::Vertex>& vertices)
{
	float maxx, maxy, miny, minx;
	maxx = minx = vertices[0].position.x;
	maxy = miny = vertices[0].position.y;
	for (const auto& it : vertices)
	{
		if (it.position.x < minx)
			minx = it.position.x;
		if (it.position.y < miny)
			miny = it.position.y;
		if (it.position.x > maxx)
			maxx = it.position.x;
		if (it.position.y > maxy)
			maxy = it.position.y;
	}
	return sf::FloatRect(minx, miny, maxx - minx, maxy - miny);
}

sf::FloatRect util::merge_bounds(const sf::FloatRect& first, const sf::FloatRect& second)
{
	sf::FloatRect bound = first;
	float r_bound = bound.left + bound.width;
	float r_obj = second.left + second.width;
	float b_bound = bound.top + bound.height;
	float b_obj = second.top + second.height;
	bound.left = std::min(bound.left, second.left);
	bound.top = std::min(bound.top, second.top);
	bound.width = std::max(r_bound, r_obj) - bound.left;
	bound.height = std::max(b_bound, b_obj) - bound.top;
	return bound;
}

bool util::contained_in_polygon(Vectorf point, float max_x, const std::vector<Vectorf>& polygon)
{
	Vectorf outside_p = { max_x + 1, point.y };
	auto section = std::make_pair(point, outside_p);
	int n = 0;
	for (int i = 0; i < polygon.size() - 1; i++)
	{
		if (util::intersection(
			std::make_pair(polygon[i], polygon[i + 1]), section))
		{
			n++;
		}
	}
	if (util::intersection(
		std::make_pair(polygon[0], polygon[polygon.size() - 1]), section))
		n++;
	if (n % 2)
		return true;
	return false;
}

util::Color_generator::Color_generator(string path)
{
	std::ifstream color_file;
	color_file.open(path);
	if (!color_file.good())
	{
		throw std::runtime_error("Can't open file");
	}
	int r, g, b;
	while (color_file >> r >> g >> b)
	{
		colors.push_back(sf::Color(r, g, b));
	}
	it = colors.cbegin();
}

sf::Color util::Color_generator::get_color()
{
	sf::Color val = *it;
	it = increment_iterator(it, colors);
	return val;
}
