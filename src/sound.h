#pragma once
#include <SFML/Audio.hpp>
#include "messaging.h"

typedef std::string string;

class Sound_system : public Message_receiver, public Message_sender
{
	std::vector<std::unique_ptr<sf::Sound>> sounds;
	const std::vector<sf::SoundBuffer>* buffers;
	const std::vector<string> music_paths;
	sf::Music music;
	int music_id = -1;
	int next_music_id = -1;
	const float CHANGE_DELTA = 180.f;
	float timer;
	enum class Music_state {LOUDER, QUIETER, DEFAULT} state = Music_state::DEFAULT;

public:
	Sound_system(const std::vector<sf::SoundBuffer>* buffers_, std::vector<string> music_paths_);
	void update(float dt);
};