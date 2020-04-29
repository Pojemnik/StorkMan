#pragma once
#include "util.h"

const int light_const = 500050;//default for dynamic light: 500050

struct Light_source
{
	const sf::Texture* texture;
	Vectorf pos;
	sf::Color color;
	float intensity;

	Light_source();
	Light_source(Vectorf p, const sf::Texture* t, sf::Color c, float i);
};

class Light
{
private:
	sf::Texture lightmap_texture;
	sf::RenderTexture* target;
	sf::Texture* light_texture;
	sf::RenderStates states;

	std::pair<float, Vectorf>cast_ray(Vectorf source, Vectorf alfa, std::vector<std::pair<Vectorf, Vectorf>>& map_edges);
	std::vector<std::pair<float, Vectorf>> calc_light_source(Light_source source, std::vector<std::pair<Vectorf, Vectorf>>& map_edges, std::vector<Vectorf>& map_vertices);
	int add_light_edges(Vectorf source_pos, std::vector<std::pair<Vectorf,
		Vectorf>>&map_edges, std::vector<Vectorf>& map_vertices);
	void remove_light_edges(int number, std::vector<std::pair<Vectorf,
		Vectorf>>&map_edges, std::vector<Vectorf>& map_vertices);
public:
	sf::Sprite lightmap;

	Light() = default;
	Light(Vectorf level_size, sf::Texture* light_tex);
	void calc_light(std::vector<Light_source>& sources, sf::Transform transform, std::vector<std::pair<Vectorf, Vectorf>>& map_edges, std::vector<Vectorf>& map_vertices);
};