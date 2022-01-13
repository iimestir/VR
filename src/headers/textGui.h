#ifndef FONT_H
#define FONT_H

#include <iostream>
#include <map>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ft2build.h>

#include FT_FREETYPE_H

#include "shader.h"
#include "VAO.h"
#include "VBO.h"

using namespace std;

struct Character {
    unsigned int textureID;  // ID handle of the glyph texture
    glm::ivec2   Size;       // Size of glyph
    glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
    unsigned int Advance;    // Offset to advance to next glyph
};

class TextGUI {
	private : 
        map<char, Character> Characters;
        Shader textShader;

        unsigned int vao;
        unsigned int vbo;
        unsigned int texture;

        const char* text;
        float x;
        float y; 
        float scale;
        glm::vec3 color;

	public :
        TextGUI(const char*, const char*, const char*, unsigned, unsigned, float, float, float = 1.0f, float = 1.0f, float = 1.0f, float = 1.0f);

        void loadFont();

        void fillCharacters();
        void setText(const char*);
};

#endif