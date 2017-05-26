#include "../include/Player.h"

void Player::init(Texture *pTex, Vector2f vPos)
{
    //position in Spritesheet
    m_vTexPos = Vector2f(270.0f, 177.0f);
    m_vPos = vPos;

    m_sprite.create(pTex, vPos, Vector2f(26, 26), Vector2f(13,13),
                    "shader/coloredSpriteShader.vert", "shader/coloredSpriteShader.frag");
    m_sprite.setTexRect(m_vTexPos.x, m_vTexPos.y);
    m_sprite.setColor(1,1,0);


    m_iCurrAnim = animRight;
    m_iPrevAnim = 0;
    m_fAnimTimer = 0.0f;
    m_bReverseAnim = false;

    m_ixAnim = 1;
    m_iyAmim = 0;

    m_iAnimStep = 0;

    m_bAlive = true;
    m_bEater = false;
    m_fEaterTime = 10.0f;
}
void Player::del()
{
    m_sprite.del();
}
void Player::handleEvents()
{
    if(m_vPos.x <= 0)
    {
        m_sprite.move(680, 0);
        m_vPos.x = m_vPos.x + 680;
    }
    if(m_vPos.x >= 682)
    {
        m_vPos.x = m_vPos.x -670;
        m_sprite.move(-670, 0);
    }
}
void Player::update(float fTime)
{
    m_fAnimTimer += fTime;
    animate();
    if(m_bEater)
    {
        m_fEaterTime -= fTime;
        if(m_fEaterTime <= 0.0f)
        {
            m_bEater = false;
            m_fEaterTime = 10.0f;
        }
        m_sprite.setColor(sin(m_fEaterTime) * 1, 1.0f, 1.0f);
    }
    else m_sprite.setColor(1, 1, 1);

}
void Player::render()
{
    m_sprite.render();
}
void Player::move(float x, float y)
{
    m_sprite.move(x, y);
    m_vPos.x += x;
    m_vPos.y -= y;

    //animation Status
    if(x < 0)
    {
        m_iPrevAnim = m_iCurrAnim;
        m_iCurrAnim = animLeft;
        m_ixAnim = -1;
        m_iyAmim =  0;
    }
    else if(x > 0)
    {
        m_iPrevAnim = m_iCurrAnim;
        m_iCurrAnim = animRight;
        m_ixAnim = 1;
        m_iyAmim = 0;
    }
    else if(y < 0)
    {
        m_iPrevAnim = m_iCurrAnim;
        m_iCurrAnim = animDown;
        m_ixAnim = 0;
        m_iyAmim = 1;
    }

    else
    {
        m_iPrevAnim = m_iCurrAnim;
        m_iCurrAnim = animUp;
        m_ixAnim =  0;
        m_iyAmim = -1;
    }
}
void Player::animate()
{
    if(m_fAnimTimer >= 0.1f)
    {
        m_fAnimTimer -= 0.1f;
        if(!m_bReverseAnim)
        {
            if(m_iAnimStep == 2)    m_bReverseAnim = true;
            else
            {
                m_sprite.setTexRect(m_vTexPos.x + 16.0f * m_ixAnim * m_iAnimStep, m_vTexPos.y + 16.0f * m_iyAmim * m_iAnimStep);
                m_iAnimStep++;
            }
        }
        else
        {
            if(m_iAnimStep== 0)    m_bReverseAnim = false;
            else
            {
                m_sprite.setTexRect(m_vTexPos.x + 16.0f * m_ixAnim * m_iAnimStep, m_vTexPos.y + 16.0f * m_iyAmim * m_iAnimStep);
                m_iAnimStep--;
            }
        }
    }
}
Vector2f Player::getPos()
{
    return m_vPos;
}