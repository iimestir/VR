#include "headers/game.h"

void Game::handleAddNote() {
	noteAmount++;

	if (noteAmount == 8) {
		// TODO : détruire la porte fermée qui bloque la sortie
	}
}

void Game::update(notificationType request) {
	switch (request) {
		case ADD_NOTE:
			handleAddNote();
			break;
		default:
			throw invalid_argument("Unsupported request type : " + request);
	}
}

Game::Game() {
	this->noteAmount = 0;
}
