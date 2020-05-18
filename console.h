#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <algorithm>
#include <vector>
#include <cctype>
#include <array>

enum class Stream_color { WHITE, GREY, RED };

typedef sf::Vector2f Vectorf;
typedef sf::Vector2i Vectori;
typedef std::string string;

class Console_stream
{
private:
	string buffer;
	Stream_color color;

	friend Console_stream& operator<<(Console_stream& stream, string& s);

public:
	Console_stream(Stream_color color);
};

class Console : public sf::Drawable
{
public:
	Console(const sf::Texture* tex, sf::Font* f, Vectori res);

	Console_stream message;
	Console_stream log;
	Console_stream error;

	void activate(Vectori res);
	void update_content();
	void deactivate();
	bool is_active();
	void input_append(char c);
	string get_line();
	void flush();
	void print(string s);
	void print(char c);
	void scroll(int delta);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	bool active = false;
	string last_line;
	Vectori screen_resolution;
	std::vector<string> content_history;
	string input_buffer;
	string output_buffer;
	const char cursor = '_';
	sf::Sprite background;
	sf::Text content;
	sf::Font* font;
	sf::Text buffer;
	int scroll_pos = 0;
	const int lines_n = 9;
};

//Tu powinien byæ template, ale nie umiem
inline Console& operator<<(Console& con, const string& obj)
{
	con.print(obj);
	return con;
}

inline Console& operator<<(Console& con, const int& obj)
{
	con.print(std::to_string(obj));
	return con;
}

inline Console& operator<<(Console& con, const float& obj)
{
	con.print(std::to_string(obj));
	return con;
}

inline Console& operator<<(Console& con, const char& obj)
{
	con.print(obj);
	return con;
}