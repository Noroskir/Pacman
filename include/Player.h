#ifndef PLAYER_H
#define PLAYER_H

#include "Sprite.h"


class Player
{
public:
    void init(Texture *pTex, Vector2f vPos);
    void del();

    void handleEvents();
    void update(float fTime);
    void render();

    void move(float x, float y);
    Vector2f getPos();

    bool m_bAlive;
    bool m_bEater;

private:

    void animate();

    Sprite m_sprite;
    Vector2f m_vTexPos;
    Vector2f m_vPos;

    enum m_AnimState {animUp, animDown, animLeft, animRight};
    int m_iPrevAnim;
    int m_iCurrAnim;
    int m_iAnimStep;
    bool m_bReverseAnim;

    int m_ixAnim;
    int m_iyAmim;

    int m_iAnimSteps;
    float m_fAnimTimer;
    float m_fEaterTime;

};


#endif