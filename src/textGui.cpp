#include "headers/textGui.h"

TextGUI::TextGUI(const char* text, const char* vFile, const char* fFile, unsigned width, unsigned height, float x, float y, float r, float g, float b, float scale)
    : textShader(Shader(vFile, fFile)),
      text(text), x(x), y(y), scale(scale), color(vec3(r, g, b)) {

    textShader.activateShader();
    glm::mat4 matrix = glm::ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f);

    glUniformMatrix4fv(textShader.getUniformLocation("projection"), 1, false, glm::value_ptr(matrix));
    glUniform1i(textShader.getUniformLocation("text"), 0);

    // configure VAO/VBO for texture quads
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void TextGUI::loadFont() {
    this->Characters.clear();

    FT_Library ft;
    if (FT_Init_FreeType(&ft)) // all functions return a value different than 0 whenever an error occurred
        cout << "ERROR::FREETYPE: Could not init FreeType Library" << endl;

    FT_Face face;
    string font = "C:/Windows/Fonts/Arial.ttf";
    if (FT_New_Face(ft, font.c_str(), 0, &face))
        cout << "ERROR::FREETYPE: Failed to load font" << endl;

    // set size to load glyphs as
    FT_Set_Pixel_Sizes(face, 0, 48);

    // disable byte-alignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // then for the first 128 ASCII characters, pre-load/compile their characters and store them
    for (GLubyte c = 0; c < 128; c++) {
        // load character glyph 
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            cout << "ERROR::FREETYTPE: Failed to load Glyph" << endl;
            continue;
        }

        // generate texture
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, 
            GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer
        );

        // set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // now store character for later use
        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x
        };

        Characters.insert(pair<char, Character>(c, character));
    }

    glBindTexture(GL_TEXTURE_2D, 0);

    // destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}
    
void TextGUI::fillCharacters() {
    // activate corresponding render state	
    textShader.activateShader();

    glUniform3f(textShader.getUniformLocation("textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(vao);

    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    string t = text;
    unsigned x1 = x;
    unsigned y1 = y;

    // iterate through all characters
    string::const_iterator c;
    for (c = t.begin(); c != t.end(); c++) {
        Character ch = Characters[*c];

        float xpos = x1 + ch.Bearing.x * scale;
        float ypos = y1 + (this->Characters['H'].Bearing.y - ch.Bearing.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;
        // update VBO for each character
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 0.0f },
            { xpos,     ypos,       0.0f, 0.0f },

            { xpos,     ypos + h,   0.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 0.0f }
        };

        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.textureID);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph
        x1 += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (1/64th times 2^6 = 64)
    }
    
    glDisable(GL_BLEND);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glEnable(GL_DEPTH_TEST);

    glCullFace(GL_FRONT);
    glFrontFace(GL_CCW);
    glEnable(GL_CULL_FACE);
}

void TextGUI::setText(string text) {
    this->text = text.c_str();
}

void TextGUI::setColor(float r, float g, float b) {
    color.r = r;
    color.g = g;
    color.b = b;
}
