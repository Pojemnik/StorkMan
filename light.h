#pragma once
#include "util.h"

class Light
{
private:
	sf::Texture lightmap_texture;
	sf::RenderTexture* target;
	sf::Texture* light_texture;
	sf::RenderStates states;

	std::pair<float, Vectorf>cast_ray(Vectorf source, Vectorf alfa, std::vector<std::pair<Vectorf, Vectorf>>& map_edges);
	std::vector<std::pair<float, Vectorf>> calc_light_source(Vectorf source, std::vector<std::pair<Vectorf, Vectorf>>& map_edges, std::vector<Vectorf>& map_vertices);

public:
	sf::Sprite lightmap;

	Light() = default;
	Light(Vectorf level_size, sf::Texture* light_tex);
	void calc_light(std::vector<Vectorf>& sources, sf::Transform transform, std::vector<std::pair<Vectorf, Vectorf>>& map_edges, std::vector<Vectorf>& map_vertices);
};