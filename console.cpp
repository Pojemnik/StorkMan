#include "console.h"

Console_stream::Console_stream(Stream_color c) : color(c) {}

Console_stream& operator<<(Console_stream& stream, string& s)
{
	stream.ingest(s);
	return stream;
}

Console_stream& operator<<(Console_stream& stream, char& c)
{
	stream.ingest(c);
	return stream;
}

Console_stream& operator<<(Console_stream& stream, string s)
{
	stream.ingest(s);
	return stream;
}

Console_stream& operator<<(Console_stream& stream, char c)
{
	stream.ingest(c);
	return stream;
}

Console_stream& operator<<(Console_stream& stream, Vectorf& v)
{
	stream.ingest(std::to_string(v.x) + " " + std::to_string(v.y));
	return stream;
}

Console_stream& operator<<(Console_stream& stream, Vectori& v)
{
	stream.ingest(std::to_string(v.x) + " " + std::to_string(v.y));
	return stream;
}

Console_stream& operator<<(Console_stream& stream, Vectori v)
{
	stream.ingest(std::to_string(v.x) + " " + std::to_string(v.y));
	return stream;
}

Console_stream& operator<<(Console_stream& stream, Vectorf v)
{
	stream.ingest(std::to_string(v.x) + " " + std::to_string(v.y));
	return stream;
}

Console_stream& operator<<(Console_stream& stream, float& f)
{
	stream.ingest(std::to_string(f));
	return stream;
}

Console_stream& operator<<(Console_stream& stream, float f)
{
	stream.ingest(std::to_string(f));
	return stream;
}

Console_stream& operator<<(Console_stream& stream, int& i)
{
	stream.ingest(std::to_string(i));
	return stream;
}

Console_stream& operator<<(Console_stream& stream, int i)
{
	stream.ingest(std::to_string(i));
	return stream;
}

void Console_stream::ingest(char c)
{
	buffer += c;
	if (c == '\n')
		flush();
}

void Console_stream::ingest(string s)
{
	for (auto c : s)
	{
		ingest(c);
	}
}

void Console_stream::flush()
{
	data.push(buffer.erase(buffer.size() - 1, 1));
	buffer = "";
}

bool Console_stream::data_available()
{
	if (data.size())
		return true;
	return false;
}

string Console_stream::get_line()
{
	string s = data.front();
	data.pop();
	return s;
}

void Console_stream::clear()
{
	buffer = "";
	while (data.size())
	{
		data.pop();
	}
}

Console::Console(const sf::Texture* tex, sf::Font* f, Vectori res) : font(f),
out(Stream_color::WHITE), log(Stream_color::GREY), err(Stream_color::RED)
{
	screen_resolution = res;
	Vectorf scale = { (float)res.x / 1920.f, 1 };
	background.setTexture(*tex);
	for (auto& text : content_text)
	{
		text.setFont(*f);
		text.setCharacterSize(15);
	}
	buffer.setFont(*f);
	buffer.setCharacterSize(15);
	input_buffer.push(">");
	buffer.setString(input_buffer.back() + cursor);
}

void Console::activate(Vectori res)
{
	screen_resolution = res;
	Vectorf scale = { (float)res.x / 1920.f, 1 };
	background.setScale(scale);
	background.setPosition(0, (float)res.y - 180);
	active = true;
	buffer.setPosition(0, (float)screen_resolution.y - 20);
	history_pos = 0;
	update_content();
}

bool Console::output_available()
{
	return out.data_available() || log.data_available() || err.data_available();
}

void Console::get_data_from_streams()
{
	while (out.data_available())
	{
		content.push_back(
			std::make_pair(out.get_line(), out.color));
	}
	while (log.data_available())
	{
		content.push_back(
			std::make_pair(log.get_line(), log.color));
	}
	while (err.data_available())
	{
		content.push_back(
			std::make_pair(err.get_line(), err.color));
	}
}

void Console::update_content()
{
	get_data_from_streams();
	int lines = 0;
	for (auto& i : content_text)
	{
		i.setString("");
	}
	for (size_t i = content.size() - 1 - scroll_pos;
		i >= 0 && lines < lines_n; i--, lines++)
	{
		content_text[lines].setPosition(0, (float)screen_resolution.y - (lines + 2) * 18);
		switch (content[i].second)
		{
		case Stream_color::RED:
			content_text[lines].setFillColor(sf::Color::Red);
			break;
		case Stream_color::GREY:
			content_text[lines].setFillColor(sf::Color(100, 100, 100));
			break;
		case Stream_color::WHITE:
			content_text[lines].setFillColor(sf::Color::White);
			break;
		}
		content_text[lines].setString(content[i].first);
	}
	history_pos = 0;
}

void Console::clear()
{
	content.clear();
	while (input_buffer.size())
	{
		input_buffer.pop();
	}
	input_buffer.push(">");
	log.clear();
	out.clear();
	err.clear();
	update_content();
	buffer.setString(input_buffer.back() + cursor);
	input_history.clear();
	history_pos = 0;
}

void Console::get_next_history_line()
{
	if (input_history.size() > history_pos)
	{
		history_pos++;
		input_buffer.back() =
			">" + input_history[input_history.size() - history_pos];
		buffer.setString(input_buffer.back() + cursor);
	}
}

void Console::get_previous_history_line()
{
	if (history_pos == 1)
	{
		history_pos = 0;
		input_buffer.back() = ">";
		buffer.setString(input_buffer.back() + cursor);
	}
	if (history_pos > 1)
	{
		history_pos--;
		input_buffer.back() =
			">" + input_history[input_history.size() - history_pos];
		buffer.setString(input_buffer.back() + cursor);
	}
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
	target.draw(background, states);
	for (auto& text : content_text)
	{
		target.draw(text, states);
	}
	target.draw(buffer, states);
}

void Console::input_append(char c)
{
	if (isprint(c) && c != '`')
	{
		input_buffer.back() += c;
		buffer.setString(input_buffer.back() + cursor);
	}
	else
	{
		if (c == '\r')
		{
			content.push_back(
				std::make_pair(input_buffer.back(), Stream_color::WHITE));
			input_history.push_back(input_buffer.back().substr(1));
			input_buffer.back() = input_buffer.back().substr(1);
			input_buffer.push(">");
			buffer.setString(input_buffer.back() + cursor);
			update_content();
		}
		if (c == '\b')
		{
			if (input_buffer.back().length() > 1)
			{
				input_buffer.back().erase(input_buffer.back().length() - 1, 1);
				buffer.setString(input_buffer.back() + cursor);
			}
		}
	}
}

bool Console::user_input_data_available()
{
	return input_buffer.size() > 1;
}

void Console::input_append(string s)
{
	for (auto c : s)
	{
		input_append(c);
	}
}

void Console::scroll(int delta)
{
	scroll_pos += delta;
	scroll_pos = std::max(0, scroll_pos);
	scroll_pos = std::min(scroll_pos,
		std::max(0, (int)content.size() - lines_n));
	update_content();
}

string Console::get_user_input_line()
{
	string s = input_buffer.front();
	input_buffer.pop();
	return s;
}