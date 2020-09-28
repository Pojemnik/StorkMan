#pragma once
#include <algorithm>
#include "animations.h"

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
	std::vector<const Dynamic_animation_struct*> dynamic_animations;
	std::map<std::string, std::vector<sf::Texture>> animations;
	sf::Texture* bg = nullptr;
	sf::Texture* layer2 = nullptr;
	sf::Texture* light = nullptr;
	sf::Texture* console_bg = nullptr;
	Animation_tree stork_tree;
	sf::Shader blurv, blurh, white;
	sf::Font storkfont, consola;
	sf::Image icon;
	std::vector<sf::SoundBuffer> sounds;
	Animation_index reverse_animation_index[10] = { Animation_index::DEFAULT,
		Animation_index::IDLE, Animation_index::MOVE, Animation_index::JUMP_IDLE,
		Animation_index::JUMP_RUN, Animation_index::PUNCH_1, Animation_index::PUNCH_2,
		Animation_index::DIE, Animation_index::HIT, Animation_index::ADDITONAL_1 };

	void load_assets();
	void load_additional_texture(std::string path, std::string name, int repeat);
	void load_additional_animation(string path, string name, Vectori n,
		Vectori size);

private:
	const int MAX_ADDITIONAL_TEXTURES = 200;
	void load_texture(sf::Texture& t, sf::Image& img, int y, int x, int sx, int sy, bool rep);
	void load_texture(sf::Texture& t, string path, bool rep);
	void load_textures(std::vector<sf::Texture>& v, std::string path, bool rep);
	void load_shaders();
	void load_animation(std::vector<sf::Texture>& a, sf::Image& img, int x, int y,
		int sx, int sy);//a musi byæ zaalokowane wczeœniej
	Dynamic_animation_struct* load_dynamic_animation(std::string path);
	Animation_tree load_animation_tree(std::string path);
	void load_sound(sf::SoundBuffer& buf, string path);
	void load_sounds();
	void load_hp_bar();
};
