#include "headers/audio.h"

Audio::Audio() {
	music = new sf::Music();
}

void Audio::pushSound(const char* path) {
	sf::SoundBuffer buffer;

	buffer.loadFromFile(path);
	this->buffers.push_back(buffer);
}

void Audio::playSound(unsigned index, float volume) {
	sound.setBuffer(buffers.at(index));
	sound.play();
}

void Audio::playMusic(const char* path, float volume) {
	music->setVolume(volume);
	music->setLoop(true);
	music->openFromFile(path);
	music->play();
}