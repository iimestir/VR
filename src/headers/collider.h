#ifndef COLLIDER_H
#define COLLIDER_H

struct Colliders {
	float minX;
	float minY;
	float minZ;
	float maxX;
	float maxY;
	float maxZ;

	bool collidesWith(float x, float y, float z) {
		bool testX = minX <= x && x <= maxX;
		bool testY = minY <= y && y <= maxY;
		bool testZ = minZ <= z && z <= maxZ;

		if(testX || testY || testZ)
			cout << "X:" << testX << " Y:" << testY << " Z:" << testZ << endl;

		return testX && testY && testZ;
	}
};

#endif