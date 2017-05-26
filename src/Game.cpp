#include "../include/Game.h"

void Game::init(GLFWwindow *pWindow)
{
    m_pWindow = pWindow;
    m_sheet.loadTexture("data/pacman.png", m_sheetID);

    m_player.init(&m_sheet, Vector2f(300, 105));
    m_level.init(&m_sheet, &m_player);

    m_fClickLock = 0.0f;
    m_bClickLock = false;
    m_iScore = 0;

    m_sScore.create(&m_sheet, Vector2f(200, 758), Vector2f(100, 30), Vector2f(40, 9));
    m_sScore.setTexRect(295, 227);
    m_sDigit1.create(&m_sheet, Vector2f(270, 788), Vector2f(30, 30), Vector2f(8.1, 10));
    m_sDigit1.setTexRect(232, 240);
    m_sDigit2.create(&m_sheet, Vector2f(240, 788), Vector2f(30, 30), Vector2f(8.1, 10));
    m_sDigit2.setTexRect(232, 240);
    m_sDigit3.create(&m_sheet, Vector2f(210, 788), Vector2f(30, 30), Vector2f(8.1, 10));
    m_sDigit3.setTexRect(232, 240);
    m_sDigit4.create(&m_sheet, Vector2f(180, 788), Vector2f(30, 30), Vector2f(8.1, 10));
    m_sDigit4.setTexRect(232, 240);
    m_sGameOver.create(&m_sheet, Vector2f(150, 260), Vector2f(300, 60), Vector2f(80, 7));
    m_sGameOver.setTexRect(233, 253);

}
void Game::del()
{
    m_player.del();
    m_level.del();
    m_sScore.del();

    m_sDigit1.delWithoutTexture();
    m_sDigit2.delWithoutTexture();
    m_sDigit3.delWithoutTexture();
    m_sDigit4.delWithoutTexture();
    m_sGameOver.delWithoutTexture();

}
void Game::handleEvents()
{
    if(m_player.m_bAlive)
    {
        if (glfwGetKey(m_pWindow, GLFW_KEY_UP) == GLFW_PRESS) m_player.move(0.0f, 100.0f * m_fTime);
        if (glfwGetKey(m_pWindow, GLFW_KEY_DOWN) == GLFW_PRESS) m_player.move(0.0f, -100.0f * m_fTime);
        if (glfwGetKey(m_pWindow, GLFW_KEY_RIGHT) == GLFW_PRESS) m_player.move(100.0f * m_fTime, 0.0f);
        if (glfwGetKey(m_pWindow, GLFW_KEY_LEFT) == GLFW_PRESS) m_player.move(-100.0f * m_fTime, 0.0f);
    }
    if(glfwGetKey(m_pWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(m_pWindow, true);

    if(glfwGetMouseButton(m_pWindow, GLFW_MOUSE_BUTTON_LEFT) && !m_bClickLock)
    {
        m_bClickLock = true;
        double x = 0;
        double y = 0;
        glfwGetCursorPos(m_pWindow, &x, &y);
        //m_level.setWall(x, y);
        //m_level.setPoint(x, y);
        //m_level.setBigPoint(x, y);
        m_level.setDSquare(x, y);

        printf("Window koords: %f, %f\n", x, y);

    }
    if(glfwGetMouseButton(m_pWindow, GLFW_MOUSE_BUTTON_RIGHT) && !m_bClickLock)
    {
        m_bClickLock = true;
        double x = 0;
        double y = 0;
        glfwGetCursorPos(m_pWindow, &x, &y);
        //m_level.unsetWall(x, y);
        //m_level.unsetPoint(x, y);
        //m_level.unsetBigPoint(x, y);
        m_level.unsetDSquare(x, y);

        printf("Window koords(unsetting): %f, %f\n", x, y);
    }

    if(glfwGetKey(m_pWindow, GLFW_KEY_ENTER) == GLFW_PRESS && !m_bClickLock)
    {
        m_level.saveLevel();
        m_bClickLock = true;
    }
    if(glfwGetKey(m_pWindow, GLFW_KEY_BACKSPACE) == GLFW_PRESS && !m_bClickLock)
    {
        m_level.loadLevel();
        m_bClickLock = true;
    }
    if(glfwGetKey(m_pWindow, GLFW_KEY_TAB) == GLFW_PRESS && !m_bClickLock)
    {
        m_bClickLock = true;
        if(m_level.m_bDrawGrid) m_level.m_bDrawGrid = false;
        else m_level.m_bDrawGrid = true;
    }
    m_level.handleEvents();
    m_player.handleEvents();
}
void Game::update(float fTime)
{
    m_fTime = fTime;

    m_fClickLock += m_fTime;
    if(m_fClickLock >= 0.4f)
    {
        m_fClickLock -= 0.4f;
        m_bClickLock = false;
    }

    if(m_player.m_bAlive) m_player.update(fTime);
    m_level.update(m_player.getPos(), fTime, m_iScore);

    //score
    m_sDigit1.setTexRect(m_iDigitPos[(m_iScore % 10)], 240);
    m_sDigit2.setTexRect(m_iDigitPos[((m_iScore / 10) % 10)], 240);
    m_sDigit3.setTexRect(m_iDigitPos[((m_iScore / 100) % 10)], 240);
    m_sDigit4.setTexRect(m_iDigitPos[((m_iScore / 1000) % 10)], 240);

}
void Game::render()
{
    m_level.render();
    if(m_player.m_bAlive) m_player.render();
    m_sScore.render();
    m_sDigit1.render();
    m_sDigit2.render();
    m_sDigit3.render();
    m_sDigit4.render();
    if(!m_player.m_bAlive) m_sGameOver.render();
}