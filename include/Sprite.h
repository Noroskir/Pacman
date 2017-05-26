#ifndef SPRITE_H
#define SPRITE_H

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <math.h>

#include "Shaderprogram.h"
#include "Texture.h"
#include "Math.h"

class Sprite
{
public:

    ~Sprite();

    void create(const char *texturePath, float xStart, float yStart,
                float xLen, float yLen, float xTexLen, float yTexLen);
    void create(Texture *pTex, Vector2f vPos, Vector2f vSize, Vector2f vTexSize,
                const char *vertexPath = "shader/spriteShader.vert", const char *fragmentPath = "shader/spriteShader.frag");
    void del();
    void delWithoutTexture();

    void render();
    void move(float x, float y);

    void shiftTexRect(float x, float y);
    void setTexRect(float x, float y);
    Vector2f getPos();

    GLuint getUniform(const char *name);
    void setColor(float r, float g, float b);

private:


    GLuint m_vao;
    GLuint m_ebo;
    GLuint m_vbo;

    GLuint m_texID1;

    GLuint m_gPos;
    GLuint m_gTexShift;

    Shaderprogram m_shader;
    Texture m_tex;

    Vector2f m_vShiftPos;
    Vector2f m_vTexShift;
    Vector2i m_vTexDim;

    Vector2f m_vPos;
};


#endif