#ifndef RBO_H
#define RBO_H

#include <glad/glad.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

class RBO {
private:
	GLuint ID;
public:
	RBO(unsigned, unsigned);

	inline GLuint getID() {
		return ID;
	}
};

#endif // FBO_H
