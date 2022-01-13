#ifndef GAME_VR_H
#define GAME_VR_H

#include <iostream>
#include <stdio.h>
#include <vector>

#include "observer.h"
#include "notificationType.h"
#include "headers/scene.h"


using namespace std;

class Game : public Observer {
private:
	unsigned noteAmount;
	unsigned doorID;
	unsigned flashID;

	void endGame(vector<void*>);
	void handleAddNote(vector<void*>);
	void update(notificationType, vector<void*>) override;
public:
	Game();

	void setDoorID(unsigned);
	void setFlashID(unsigned);
};

#endif