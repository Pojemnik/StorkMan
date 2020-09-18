#include "ui.h"

Hp_bar::Bar_part::Bar_part(std::shared_ptr<sf::Texture> _bar_tex,
	std::shared_ptr<std::vector<sf::Texture>> _content_tex) : bar_tex(_bar_tex),
	content_tex(_content_tex)
{
	it = content_tex->begin();
	bar.setTexture(*bar_tex);
	content.setTexture(*it);
}

Hp_bar::Hp_bar(std::shared_ptr<sf::Texture> _bot_bar,
	std::shared_ptr<sf::Texture> _mid_bar,
	std::shared_ptr<sf::Texture> _top_bar,
	std::shared_ptr<std::vector<sf::Texture>> _bot_content,
	std::shared_ptr<std::vector<sf::Texture>> _mid_content,
	std::shared_ptr<std::vector<sf::Texture>> _top_content, int _max_hp) :
	max_hp(_max_hp), bot(_bot_bar, _bot_content), mid(_mid_bar, _mid_content),
	top(_top_bar, _top_content), bar_height(context.resolution.y - 2*top_bot_space),
	default_resolution(context.resolution)
{
	top.bar.setPosition(pos);
	mid.bar.setPosition(pos.x, pos.y + top_bot_tex_height);
	bot.bar.setPosition(pos.x, default_resolution.y - top_bot_tex_height);
	mid_ratio = (bar_height - 2 * top_bot_tex_height)
		/ bar_height;
	mid.bar.setScale(scale_factor_x,
		((float)default_resolution.y - 2 * top_bot_tex_height) / mid_tex_height);
	mid.content.setScale(scale_factor_x,
		(bar_height - 2 * top_bot_tex_height) / mid_tex_height);
	top.content.setPosition(pos.x, pos.y + top_bot_space);
	mid.content.setPosition(pos.x, pos.y + top_bot_tex_height + top_bot_space);
	bot.content.setPosition(pos.x, default_resolution.y - top_bot_tex_height - top_bot_space);
	scale_x(scale_factor_x);
}

void Hp_bar::next_frame()
{
	if (frame_counter++ >= fpf)
	{
		frame_counter = 0;
		top.it = util::increment_iterator(top.it, *top.content_tex);
		mid.it = util::increment_iterator(mid.it, *mid.content_tex);
		bot.it = util::increment_iterator(bot.it, *bot.content_tex);
		top.content.setTexture(*top.it);
		mid.content.setTexture(*mid.it);
		bot.content.setTexture(*bot.it);
	}
}

void Hp_bar::update(int current_hp)
{
	float hp_percent = (float)current_hp / max_hp;
	float current_bar_height = bar_height * hp_percent;
	float mid_height = current_bar_height * mid_ratio;
	float top_bot_height = (1.f - mid_ratio) * current_bar_height / 2;
	mid.content.setScale(scale_factor_x, mid_height / mid_tex_height);
	bot.content.setScale(scale_factor_x, hp_percent);
	top.content.setScale(scale_factor_x, hp_percent);
	bot.content.setPosition(pos.x, (float)default_resolution.y -
		top_bot_height - top_bot_space);
	mid.content.setPosition(pos.x, (float)default_resolution.y -
		top_bot_height - mid_height - top_bot_space);
	top.content.setPosition(pos.x, (float)default_resolution.y
		- top_bot_height * 2 - mid_height - top_bot_space);
}

void Hp_bar::scale_x(float factor)
{
	scale_factor_x = factor;
	top.bar.setScale(scale_factor_x, top.bar.getScale().y);
	mid.bar.setScale(scale_factor_x, mid.bar.getScale().y);
	bot.bar.setScale(scale_factor_x, bot.bar.getScale().y);
	top.content.setScale(scale_factor_x, top.content.getScale().y);
	mid.content.setScale(scale_factor_x, mid.content.getScale().y);
	bot.content.setScale(scale_factor_x, bot.content.getScale().y);
}

void Hp_bar::pre_draw(int current_hp)
{
	update(current_hp);
	next_frame();
}

void Hp_bar::set_max_hp(int new_max_hp)
{
	max_hp = new_max_hp;
}

void Hp_bar::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(bot.bar);
	target.draw(mid.bar);
	target.draw(top.bar);
	target.draw(bot.content);
	target.draw(mid.content);
	target.draw(top.content);
}