#ifndef DYNFLOAT_H
#define DYNFLOAT_H

class DFloat {
private:
	float* pointer;
	float d;

public:
	DFloat(float* pointer, float d = 0) : pointer(pointer), d(d) {}

	void setD(float d) {
		this->d = d;
	}

	inline float ndValue() {
		return *pointer;
	}

	inline float value() {
		return *pointer + d;
	}
};

#endif