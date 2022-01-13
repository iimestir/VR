#ifndef OBSERVER_VR_H
#define OBSERVER_VR_H

#include <vector>

#include "notificationType.h"

using namespace std;

class Observer {
public:
	void virtual update(notificationType, vector<void*>) = 0;
};

#endif
