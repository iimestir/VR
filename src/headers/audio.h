#ifndef AUDIO_H
#define AUDIO_H

#include <SFML/Audio.hpp>
#include <vector>

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
	sf::Sound sound;
	vector<sf::SoundBuffer> buffers;

public:
	void pushSound(const char*);
	void playSound(unsigned, float = 100.0f);
	void playMusic(const char*, float = 100.0f);
};


#endif