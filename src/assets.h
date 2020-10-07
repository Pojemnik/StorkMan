#pragma once
#include "animations.h"

struct Entity_config
{
	std::vector<Vectorf> mesh;
	std::vector<string> animation_files;
	string tree_file;
	int max_hp;
	std::pair<float, int> height;
};

class Assets
{
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
	std::vector<sf::Texture> map_textures;
	std::vector<sf::Texture> enemy_textures;
	sf::Texture* pieces = nullptr;
	std::vector<sf::IntRect> pieces_rect;
	std::map<std::string, const sf::Texture*> textures;
	std::map<std::string, std::vector<sf::Texture>> animations;
	sf::Texture* bg = nullptr;
	sf::Texture* layer2 = nullptr;
	sf::Texture* light = nullptr;
	sf::Texture* console_bg = nullptr;
	sf::Font storkfont, consola;
	sf::Image icon;
	std::vector<sf::SoundBuffer> sounds;

	void load_assets();
	void load_additional_texture(std::string path, std::string name, int repeat);
	void load_additional_animation(string path, string name, Vectori n,
		Vectori size);
	std::vector<const Dynamic_animation_struct*>* load_dynamic_animations(std::vector<string> paths);
	const Animation_tree load_animation_tree(std::string path);
	void parse_additional_textures(std::string path);
	void parse_additional_animations(std::string path);

private:
	std::vector<const Dynamic_animation_struct*> dynamic_animations;

	const int MAX_ADDITIONAL_TEXTURES = 200;
	void load_texture(sf::Texture& t, sf::Image& img, int y, int x, int sx, int sy, bool rep);
	void load_texture(sf::Texture& t, string path, bool rep);
	void load_textures(std::vector<sf::Texture>& v, std::string path, bool rep);
	void load_animation(std::vector<sf::Texture>& a, sf::Image& img, int x, int y,
		int sx, int sy);//a musi byæ zaalokowane wczeœniej
	Dynamic_animation_struct* load_dynamic_animation(std::string path);
	void load_sound(sf::SoundBuffer& buf, string path);
	void load_sounds();
	void load_hp_bar();
};