//
// Created by 张程易 on 16/11/2017.
//

#include "FontFace.h"

GLuint FontFace::VAO, FontFace::VBO;
bool FontFace::isInit = false;

void FontFace::renderText(const std::string &text) {
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    float x = 0;
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++) {
        Character ch = Characters[*c];

        GLfloat xpos = x + ch.bearing.x;
        GLfloat ypos = -(ch.size.y - ch.bearing.y);

        GLfloat w = ch.size.x;
        GLfloat h = ch.size.y;
        // 对每个字符更新VBO
        GLfloat vertices[6][4] = {
                {xpos,     ypos + h, 0.0, 0.0},
                {xpos,     ypos,     0.0, 1.0},
                {xpos + w, ypos,     1.0, 1.0},

                {xpos,     ypos + h, 0.0, 0.0},
                {xpos + w, ypos,     1.0, 1.0},
                {xpos + w, ypos + h, 1.0, 0.0}
        };

        glBindTexture(GL_TEXTURE_2D, ch.textureID);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        x += (ch.advance >> 6);
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

FontFace::FontFace(FT_Library ft, const std::string &path, int fontSize) {
    this->fontSize = fontSize;
    if (FT_New_Face(ft, path.c_str(), 0, &face))
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
    FT_Set_Pixel_Sizes(face, 0, 48);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    for (GLubyte c = 0; c < 128; c++) {

        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }

        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
        );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Character character = {
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                (GLuint) face->glyph->advance.x
        };
        Characters.emplace(c, character);
    }
    if (!isInit) {
        isInit = true;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
}

int FontFace::getFontSize() const {
    return fontSize;
}
