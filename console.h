#pragma once
#include "util.h"
#include <cctype>

enum class Command_code {
	NOTHING, CHANGE_RESOLUTION, CHANGE_SCALE,
	MOVE_PLAYER, RELOAD_LIGHT
};
enum class Stream_color { WHITE, GREY, RED };

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
	Console(sf::Texture* tex, sf::Font* f, Vectori res);

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
Console& operator<<(Console& con, const string& obj);
Console& operator<<(Console& con, const char& obj);
Console& operator<<(Console& con, const int& obj);
Console& operator<<(Console& con, const float& obj);

class Command_interpreter
{
	struct Command
	{
		std::string name;
		std::vector<std::string> args;
	};

private:
	Console* console;
	void print_argument_number_error(int correct_number);
	void print_incorrect_argument_error(std::string command,
		std::string what);
	Command get_command(string s);
	std::pair<Command_code, Vectorf> execute_command(Command cmd);
	std::pair<Command_code, Vectorf> execute_command_raw(Command cmd);
	Vectorf get_vectorf(const Command& cmd, std::string var_name);
	Vectori get_vectori(const Command& cmd, std::string var_name);
	float get_float(const Command& cmd, std::string var_name);
	bool get_bool(const Command& cmd, std::string var_name,
		std::array<std::string, 2> true_false_string);
	int get_int(const Command& cmd, std::string var_name);

public:
	Command_interpreter(Console* c);
	std::pair<Command_code, Vectorf> get_and_execute_command(string s);
};