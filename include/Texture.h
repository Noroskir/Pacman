#ifndef TEXTURE_H
#define TEXTURE_H


#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "SOIL.h"

class Texture
{
public:

    void loadTexture(const char *texturePath, GLuint &ID);

    GLuint getTexture();
    void  getMesures(int &x, int &y);


private:

    int m_fWidth;
    int m_fHeight;

    GLuint m_texID;
};


#endif