#ifndef SUBJECT_VR_H
#define SUBJECT_VR_H

#include <vector>
#include <stdio.h>

#include "observer.h"
#include "notificationType.h"

using namespace std;

class Subject {
private:
	vector<Observer*> observers;
public:
	void registerObserver(Observer* obs) {
		observers.push_back(obs);
	}

	void notify(notificationType request) {
		for (Observer* o : observers)
			o->update(request);
	}
};

#endif

