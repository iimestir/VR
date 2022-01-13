#ifndef GAME_VR_H
#define GAME_VR_H

#include <iostream>
#include <stdio.h>

#include "observer.h"
#include "notificationType.h"


using namespace std;

class Game : public Observer {
private:
	unsigned noteAmount;

	void handleAddNote();
	void update(notificationType) override;
public:
	Game();
};

#endif