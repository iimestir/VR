#include "headers/audio.h"

Audio::Audio() {
	music = new sf::Music();
}

void Audio::pushSound(const char* path) {
	sf::SoundBuffer buffer;

	buffer.loadFromFile(path);
	this->buffers.push_back(buffer);
}

void Audio::playSound(unsigned index, unsigned channel, float volume) {
	if (channel == 1) {
		channel1.setBuffer(buffers.at(index));
		channel1.setVolume(volume);
		channel1.play();
	} else if (channel == 2 && channel2.getStatus() == sf::SoundSource::Stopped) {
		channel2.setBuffer(buffers.at(index));
		channel2.setVolume(volume);
		channel2.play();
	} else if (channel == 3 && channel3.getStatus() == sf::SoundSource::Stopped) {
		channel3.setBuffer(buffers.at(index));
		channel3.setVolume(volume);
		channel3.play();
	} else if (channel == 4 && channel4.getStatus() == sf::SoundSource::Stopped) {
		channel4.setBuffer(buffers.at(index));
		channel4.setVolume(volume);
		channel4.play();
	}		   
}

void Audio::playMusic(const char* path, float volume) {
	music->setVolume(volume);
	music->setLoop(true);
	music->openFromFile(path);
	music->play();
}