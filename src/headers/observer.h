#ifndef OBSERVER_VR_H
#define OBSERVER_VR_H

#include "notificationType.h"

class Observer {
public:
	void virtual update(notificationType) = 0;
};

#endif
