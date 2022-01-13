#include "headers/game.h"

void Game::endGame(vector<void*> args) {
	Scene* scene = (Scene*)args.at(0);

	scene->editTextColor(0, 0.8f, 1.0f, 0.2f);
	Audio::getInstance().playSound(14, 4, 50.0f);

	scene->destroyVertexbyID(doorID);
}

void Game::handleAddNote(vector<void*> args) {
	noteAmount++;

	Scene* scene = (Scene*)args.at(0);

	scene->editText(0, "Obtained notes " + to_string(noteAmount) + "/8");

	Audio::getInstance().playSound(13, 3);

	if (noteAmount == 8)
		endGame(args);
}

void Game::update(notificationType request, vector<void*> args) {
	switch (request) {
		case ADD_NOTE:
			handleAddNote(args);
			break;
		default:
			throw invalid_argument("Unsupported request type : " + request);
	}
}

Game::Game() {
	this->noteAmount = 0;
}

void Game::setDoorID(unsigned id) {
	doorID = id;
}
