#pragma once
#include <SFML/Graphics.hpp>
#include "util.h"

class Hp_bar : public sf::Drawable
{
private:
	struct Bar_part
	{
		std::shared_ptr<sf::Texture> bar_tex;
		std::shared_ptr<std::vector<sf::Texture>> content_tex;
		std::vector<sf::Texture>::iterator it;
		sf::Sprite bar;
		sf::Sprite content;

		Bar_part(std::shared_ptr<sf::Texture> _bar_tex,
			std::shared_ptr<std::vector<sf::Texture>> _content_tex);
	}top, bot, mid;
	Vectorf pos = { 0,0 };
	int fpf = 10;
	int frame_counter = 0;
	int max_hp;
	float mid_ratio;
	float scale_factor_x = 0.35;
	const float top_bot_tex_height = 47;
	const float mid_tex_height = 34;

	void update(int current_hp);
	void next_frame();

public:
	Hp_bar(std::shared_ptr<sf::Texture> _bot_bar, std::shared_ptr<sf::Texture> _mid_bar,
		std::shared_ptr<sf::Texture> _top_bar,
		std::shared_ptr<std::vector<sf::Texture>> _bot_content,
		std::shared_ptr<std::vector<sf::Texture>> _mid_content, 
		std::shared_ptr<std::vector<sf::Texture>> _top_content, int _max_hp);

	void scale_x(float factor);
	void pre_draw(int current_hp);
	void set_max_hp(int new_max_hp);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};