#include "console.h"

Console_stream::Console_stream(Stream_color c) : color(c) {}

Console_stream& operator<<(Console_stream& stream, string& s)
{
	stream.buffer += s;
	return stream;
}

Console::Console(const sf::Texture* tex, sf::Font* f, Vectori res) : font(f),
message(Stream_color::WHITE), log(Stream_color::GREY), error(Stream_color::RED)
{
	screen_resolution = res;
	Vectorf scale = { (float)res.x / 1920.f, 1 };
	background.setTexture(*tex);
	content.setFont(*f);
	content.setCharacterSize(15);
	buffer.setFont(*f);
	buffer.setCharacterSize(15);
	input_buffer = ">";
	buffer.setString(input_buffer + cursor);
}

void Console::activate(Vectori res)
{
	screen_resolution = res;
	Vectorf scale = { (float)res.x / 1920.f, 1 };
	background.setScale(scale);
	background.setPosition(0, res.y - 180);
	active = true;
	buffer.setPosition(0, screen_resolution.y - 20);
	update_content();
}

void Console::update_content()
{
	string content_string;
	int lines = 0;
	for (int i = content_history.size() - 1 - scroll_pos; i >= 0 && lines < lines_n; i--)
	{
		content_string = content_history[i] + '\n' + content_string;
		lines++;
	}
	content.setString(content_string);
	content.setPosition(0, screen_resolution.y - (lines+1) * 18);
}

void Console::deactivate()
{
	active = false;
}

bool Console::is_active()
{
	return active;
}

void Console::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(background);
	target.draw(content);
	target.draw(buffer);
}

void Console::input_append(char c)
{
	if (isprint(c) && c != '`')
	{
		input_buffer = input_buffer + c;
		buffer.setString(input_buffer + cursor);
	}
	else
	{
		if (c == '\r')
		{
			last_line = input_buffer.substr(1);
			content_history.push_back(input_buffer);
			input_buffer = '>';
			buffer.setString(input_buffer + cursor);
			update_content();
		}
		if (c == '\b')
		{
			if (input_buffer.length() > 1)
			{
				input_buffer.erase(input_buffer.length() - 1, 1);
				buffer.setString(input_buffer + cursor);
			}
		}
	}
}

void Console::print(string s)
{
	output_buffer += s;
}

void Console::print(char c)
{
	output_buffer += c;
	if (c == '\n')
		flush();
}

void Console::flush()
{
	size_t pos = output_buffer.find('\n');
	size_t last_pos = 0;
	while (pos != string::npos)
	{
		content_history.push_back(output_buffer.substr(last_pos, pos));
		last_pos = pos + 1;
		pos = output_buffer.find('\n', last_pos);
	}
	output_buffer = "";
	scroll_pos = 0;
	update_content();
}

void Console::scroll(int delta)
{
	scroll_pos += delta;
	scroll_pos = std::max(0, scroll_pos);
	scroll_pos = std::min(scroll_pos,
		std::max(0, (int)content_history.size() - lines_n));
	update_content();
}

string Console::get_line()
{
	return last_line;
}