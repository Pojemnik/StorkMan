#pragma once
#include "animations.h"
#include <unordered_map>
#include <array>

enum class Tex_type : int { ALBEDO = 0, NORMAL, HEIGHT };

struct Entity_config
{
	std::vector<Vectorf> mesh;
	std::vector<string> animation_files;
	std::vector<std::array<string, 3>> texture_paths;
	Vectori textures_n;
	Vectori texture_size;
	string tree_file;
	int max_hp = 1000;
	std::pair<float, int> height;
	std::vector<string> sounds;
	int type = 0;
};

class Assets
{
	std::vector<const Dynamic_animation_struct*> dynamic_animations;
	const sf::Texture* load_texture(sf::Image& img, Vectori pos, Vectori size, bool repeat);
	const sf::Texture* load_texture(string path, bool repeat);
	std::vector<const sf::Texture*> load_textures(std::string path, Vectori n,
		Vectori size, bool repeat);
	std::vector<const sf::Texture*> load_textures(sf::Image& img, Vectori frames,
		Vectori size, bool repeat);
	Dynamic_animation_struct* load_dynamic_animation(std::string path);
	void load_sound(sf::SoundBuffer& buf, string path);
	void load_hp_bar();
	std::tuple<Vectori, Vectori, std::vector<string>> load_texture_file_config(string path);
	void load_concatenated_texture_file(string path, Tex_type type);
	void load_concatenated_texture_file_group(string path, Tex_type type);
	std::vector<const std::array<const sf::Texture*, 3>*> load_entity_texture_set(std::array<string, 3> paths, Vectori tex_n, Vectori tex_size);

public:
	struct Hp_bar
	{
		std::shared_ptr<sf::Texture> top;
		std::shared_ptr<sf::Texture> mid;
		std::shared_ptr<sf::Texture> bot;
		std::shared_ptr<std::vector<sf::Texture>> content_top;
		std::shared_ptr<std::vector<sf::Texture>> content_mid;
		std::shared_ptr<std::vector<sf::Texture>> content_bot;
	} hp_bar;
	std::vector<std::vector<const std::array<const sf::Texture*, 3>*>> pieces;
	std::unordered_map<std::string, std::array<const sf::Texture*, 3>> textures;
	std::unordered_map<std::string, std::vector<const std::array<const sf::Texture*, 3>*>> animations;
	std::unordered_map<std::string, const sf::Texture*> backgrounds;
	const sf::Texture* console_bg = nullptr;
	sf::Font storkfont, consola;
	sf::Image icon;
	std::unordered_map<int, std::vector<string>> entity_sounds;
	string color_path = "data/colors.txt";

	void load_assets();
	void add_entity_sounds(int type, std::vector<string>& paths);
	std::vector<const Dynamic_animation_struct*>* load_dynamic_animations(std::vector<string> paths);
	const Animation_tree load_animation_tree(std::string path);
	void parse_additional_textures(std::string path, Tex_type type);
	void parse_additional_animations(std::string path);
	std::vector<std::vector<const std::array<const sf::Texture*, 3>*>>
		load_entity_textures(
			std::vector<std::array<string, 3>> paths, Vectori textures_n, Vectori texture_size);
};