#ifndef VPOS_H
#define VPOS_H

struct vPos {
public:
	float x;
	float y;
	float z;

	vPos(float x, float y, float z) : x(x), y(y), z(z) {}
};
#endif