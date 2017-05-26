#include "../include/Framework.h"

Framework::Framework()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, false);

    m_fTime = 0.0f;

}

void Framework::init(int width, int height, const char *title)
{
    initMath(width, height);
    createWindow(width, height, title);

    m_game.init(m_pWindow);
}
void Framework::run()
{
    while(!glfwWindowShouldClose(m_pWindow))
    {
        //framework zeug
        handleEvents();
        update();
        render();
    }
}
void Framework::del()
{
    m_game.del();
    glfwTerminate();
}
void Framework::handleEvents()
{
    glfwPollEvents();
    m_game.handleEvents();
}
void Framework::update()
{
    calcTime();
    m_game.update(m_fTime);
}
void Framework::render()
{
    m_game.render();

    glfwSwapBuffers(m_pWindow);
    glClear(GL_COLOR_BUFFER_BIT);
}
void Framework::calcTime()
{
    m_fTime = (float)glfwGetTime();
    glfwSetTime(0.0f);
}
void Framework::createWindow(int width, int height, const char *title)
{
    m_iWidth = width;
    m_iHeight = height;
    m_pWindow = glfwCreateWindow(m_iWidth, m_iHeight, title, nullptr, nullptr);


    if(m_pWindow == nullptr)
    {
        std::cout << "ERROR::Failed to create Window" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(m_pWindow);

    glewExperimental = true;
    if(glewInit() != GLEW_OK)
    {
        std::cout << "ERROR::Failed to init GLEW!" << std::endl;
        glfwTerminate();
    }

    int wid, hei;
    glfwGetFramebufferSize(m_pWindow, &wid, &hei);

    //GL Zeug
    glViewport(0, 0, width, height);
    glClearColor(0, 0.0f, 0.2f, 1);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //zeit zurücksetzten
    glfwSetTime(0.0f);
}