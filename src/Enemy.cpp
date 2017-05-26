#include "../include/Enemy.h"

void Enemy::create(Texture *pTex, float xPos, float yPos, float yTexPos)
{
    m_sprite.create(pTex, Vector2f(xPos, yPos), Vector2f(33, 33), Vector2f(15, 15));
    m_sprite.setTexRect(238, yTexPos);

    m_vPos = Vector2f(xPos, yPos);

    m_vDir = Vector2i(0, 1);
    m_vPath = Vector2f(0, 0);

    m_bCollision = false;
    m_bMovingPath = false;
    m_bOut = false;
    m_bAlive = true;
    m_bChaseLock = false;

    m_fyTexPos = yTexPos;

    m_fChaseLockTime = 0.5f;
}
void Enemy::del()
{
    m_sprite.delWithoutTexture();
}
void Enemy::handleEvents()
{
    if(m_bAlive)
    {
        if (m_bCollision)
        {
            m_vDir = getDir();
            m_bCollision = false;
        }
    }
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
void Enemy::update(float fTime)
{
    if(m_bAlive)
    {
        move(m_vDir.x * 100 * fTime, m_vDir.y * 100 * fTime);
        if (m_bMovingPath)
        {
            m_vPath.x -= 100 * fTime;
            m_vPath.y -= 100 * fTime;
        }
        if (m_vDir.x > 0) m_sprite.setTexRect(289, m_fyTexPos);
        else if (m_vDir.x < 0) m_sprite.setTexRect(238, m_fyTexPos);
        else if (m_vDir.y > 0) m_sprite.setTexRect(255, m_fyTexPos);
        else m_sprite.setTexRect(272, m_fyTexPos);

        if(m_bChaseLock)
        {
            m_fChaseLockTime -= fTime;
            if(m_fChaseLockTime <= 0)
            {
                m_fChaseLockTime = 1.0f;
                m_bChaseLock = false;
            }
        }
    }
}
void Enemy::render()
{
    if(m_bAlive)
    {
        m_sprite.render();
    }
}
void Enemy::move(float x, float y)
{
    m_vPos.x += x;
    m_vPos.y -= y;

    m_sprite.move(x, y);
}
bool Enemy::movePath(float x, float y, float fTime)
{
    if(!m_bMovingPath)
    {
        m_bMovingPath = true;
        m_vPath.x = x;
        m_vPath.y = y;
        if(x == 0 && y == 0)
        {
            m_bMovingPath = false;
            getDir();
            return true;
        }
        else if (x > 0)
        {
            m_vDir.x = 1;
            m_vDir.y = 0;
        }
        else if (x == 0)
        {
            m_vDir.x = 0;
            if(y > 0)
            {
                m_vDir.y = 1;
            }
            else if(y < 0)
            {
                m_vDir.y = -1;
                m_vPath.y *= -1;
            }
        }
        else
        {
            m_vDir.x = -1;
            m_vPath.x *= -1;
            m_vDir.y = 0;
        }
    }
    else
    {
        if(m_vPath.x > 0) return false;
        if(m_vPath.y > 0) return false;
        else
        {
            m_vPath = Vector2f(0, 0);
            getDir();
            m_bMovingPath = false;
            return true;
        }

    }
}
void Enemy::chasePlayer(float xPlayer, float yPlayer)
{

}
Vector2i Enemy::getDir()
{
    int xr = rand() % 2;
    int yr = 0;
    if(xr == 0)
    {
        yr = rand() % 2;
        if (yr == 0) yr = -1;

    }
    else
    {
        xr = rand() % 2;
        if(xr == 0) xr = -1;
    }
    return Vector2i(xr, yr);
}
void Enemy::oppositeDir()
{
    if(m_vDir.x < 0) m_vDir.x = 1;
    else if(m_vDir.x > 0) m_vDir.x = -1;
    else if(m_vDir.y < 0) m_vDir.y = 1;
    else if(m_vDir.y > 0) m_vDir.y = -1;
}
void Enemy::changeDir(int x, int y)
{
    if(x > 0) m_vDir.x = 1;
    else if(x < 0) m_vDir.x = -1;
    else m_vDir.x = 0;
    if(y > 0) m_vDir.y = 1;
    else if(y < 0) m_vDir.y = -1;
    else m_vDir.y = 0;
}