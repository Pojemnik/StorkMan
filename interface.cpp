#include "interface.h"

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
	top(_top_bar, _top_content)
{
	top.bar.setPosition(pos);
	mid.bar.setPosition(pos.x, pos.y + 47);
	bot.bar.setPosition(pos.x, context.resolution.y - 47);
	mid.bar.setScale(scale_factor_x, ((float)context.resolution.y - 2 * 47) / 34.f);
	top.content.setPosition(pos);
	mid.content.setPosition(pos.x, pos.y + 47);
	bot.content.setPosition(pos.x, context.resolution.y - 47);
	default_mid_scale = ((float)context.resolution.y - 2 * 47) / 34.f;
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
	mid.content.setScale(scale_factor_x, hp_percent * default_mid_scale);
	mid.content.setPosition(pos.x, context.resolution.y - 47 -
		default_mid_scale * hp_percent * 34);
	top.content.setPosition(pos.x, context.resolution.y - 2 * 47 -
		default_mid_scale * hp_percent * 34);
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