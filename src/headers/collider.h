#ifndef COLLIDER_H
#define COLLIDER_H

#include <functional>
#include <iostream>

using namespace std;

struct Colliders {
	float minX;
	float minY;
	float minZ;
	float maxX;
	float maxY;
	float maxZ;

	function<void(unsigned)> handle;
	unsigned vaoID;

	bool collidesWith(float x, float y, float z) {
		bool testX = minX <= x && x <= maxX;
		bool testY = minY <= y && y <= maxY;
		bool testZ = minZ <= z && z <= maxZ;

		if (testX && testY && testZ && handle != NULL)
			handle(vaoID);

		return testX && testY && testZ;
	}
};

#endif