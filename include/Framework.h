#ifndef FRAMEWORK_H
#define FRAMEWORK_H

#include <iostream>
#include <fstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Game.h"


class Framework
{
public:

    Framework();

    void init(int width, int height, const char *title);
    void run();
    void del();

private:

    void createWindow(int width, int height, const char *title);

    void handleEvents();
    void update();
    void render();

    void calcTime();

    int     m_iWidth;
    int     m_iHeight;

    float   m_fTime;

    GLFWwindow *m_pWindow;

    Game m_game;

};

#endif