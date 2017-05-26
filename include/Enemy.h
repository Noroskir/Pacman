#ifndef ENEMY_H
#define ENEMY_h

#include "Sprite.h"

class Enemy
{
public:
    void create(Texture *pTex, float xPos, float yPos, float yTexPos);
    void del();

    void handleEvents();
    void update(float fTime);
    void render();

    void move(float x, float y);
    bool movePath(float x, float y, float fTime);
    void chasePlayer(float xPlayer, float yPlayer);
    void oppositeDir();
    void changeDir(int x, int y);
    void findPath(float xPlayer, float yPlayer);


    Vector2f m_vPos;
    bool m_bCollision;

    bool m_bOut;
    bool m_bAlive;
    bool m_bChaseLock;

private:

    float m_fyTexPos;
    float m_fChaseLockTime;
    Vector2i getDir();
    Vector2i m_vDir;

    Sprite m_sprite;

    bool m_bMovingPath;
    Vector2f m_vPath;
};


#endif