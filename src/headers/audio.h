#ifndef AUDIO_H
#define AUDIO_H

#include <SFML/Audio.hpp>
#include <deque>
#include <vector>
#include <iostream>

using namespace std;

// Singleton
class Audio {
public:
	static Audio& getInstance() {
		static Audio instance;
		return instance;
	}
private:
	Audio();

	sf::Music* music;
	sf::Sound channel1;
	sf::Sound channel2;
	sf::Sound channel3;
	sf::Sound channel4;
	vector<sf::SoundBuffer> buffers;

public:
	void pushSound(const char*);
	void playSound(unsigned, unsigned, float = 100.0f);
	void playMusic(const char*, float = 100.0f);
};


#endif