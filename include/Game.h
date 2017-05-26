#ifndef GAME_H
#define GAME_h

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Sprite.h"
#include "Player.h"
#include "Level.h"

class Game
{
public:

    void init(GLFWwindow *pWindow);
    void del();

    void handleEvents();
    void update(float fTime);
    void render();

private:

    GLFWwindow *m_pWindow;

    Texture m_sheet;
    Player m_player;
    Level m_level;

    Sprite m_sScore;
    Sprite m_sDigit1;
    Sprite m_sDigit2;
    Sprite m_sDigit3;
    Sprite m_sDigit4;
    Sprite m_sGameOver;

    GLuint m_sheetID;

    float m_fTime;

    float m_fClickLock;
    bool m_bClickLock;

    int  m_iScore;
    int m_iDigitPos[10] = {232, 240, 249, 258, 266, 274, 282, 290, 297, 305};

};

#endif