#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <algorithm>
#include <vector>
#include <cctype>
#include <array>
#include <queue>
#include "messaging.h"

enum class Stream_color { WHITE, GREY, RED };

typedef sf::Vector2f Vectorf;
typedef sf::Vector2i Vectori;
typedef std::string string;

const int lines_n = 9;

class Console_stream
{
public:
	bool data_available();
	string get_line();
	void clear();
	Console_stream(Stream_color color);
	Stream_color color;

private:
	string buffer = "";
	std::queue<string> data;

	void ingest(string s);
	void ingest(char c);
	void flush();

	friend Console_stream& operator<<(Console_stream& stream, string& s);
	friend Console_stream& operator<<(Console_stream& stream, string s);
	friend Console_stream& operator<<(Console_stream& stream, Vectorf& v);
	friend Console_stream& operator<<(Console_stream& stream, Vectorf v);
	friend Console_stream& operator<<(Console_stream& stream, Vectori& v);
	friend Console_stream& operator<<(Console_stream& stream, Vectori v);
	friend Console_stream& operator<<(Console_stream& stream, char& c);
	friend Console_stream& operator<<(Console_stream& stream, char c);
	friend Console_stream& operator<<(Console_stream& stream, float& f);
	friend Console_stream& operator<<(Console_stream& stream, float f);
	friend Console_stream& operator<<(Console_stream& stream, int& i);
	friend Console_stream& operator<<(Console_stream& stream, int i);
};

class Console : public sf::Drawable, public Message_receiver, public Message_sender
{
public:
	Console_stream out;
	Console_stream log;
	Console_stream err;

	Console(const sf::Texture* tex, sf::Font* f, Vectori res);

	void activate();
	void deactivate();
	bool is_active();
	void input_append(char c);
	void input_append(string s);
	bool user_input_data_available();
	string get_user_input_line();
	void scroll(int delta);
	bool output_available();
	void update_content();
	void clear();
	void get_next_history_line();
	void get_previous_history_line();
	void push_message(Message& msg);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	const char cursor = '_';
	const string line_begin = ">";
	Vectori resolution;
	const sf::Font* font;
	bool active = false;
	int scroll_pos = 0;
	int history_pos = 0;
	std::vector<std::pair<string, Stream_color>> content;
	std::queue<string> input_buffer;
	std::vector<string> input_history;
	string output_buffer;
	sf::Sprite background;
	std::array<sf::Text, lines_n> content_text;
	sf::Text buffer;

	void get_data_from_streams();
};