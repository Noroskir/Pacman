#ifndef LEVEL_H
#define LEVEL_H

#include "Sprite.h"
#include "Shaderprogram.h"
#include <list>
#include <fstream>
#include "Player.h"
#include "Enemy.h"
#include <cmath>

class Level
{
public:
    void init(Texture *pTexture, Player *pPlayer);
    void del();

    void handleEvents();
    void update(Vector2f playerPos, float fTime, int &score);
    void render();

    void setWall(float fx, float fy);
    void unsetWall(float fx, float fy);
    void setPoint(float fx, float fy);
    void unsetPoint(float fx, float fy);
    void setBigPoint(float fx, float fy);
    void unsetBigPoint(float fx, float fy);
    void setDSquare(float fx, float fy);
    void unsetDSquare(float fx, float fy);

    void saveLevel();
    void loadLevel();

    bool m_bDrawGrid;


private:

    int m_iSchwierigkeit;
    int m_iScale;
    float m_fGesamtZeit;
    float m_fEnemyGo[4] = {0, 5, 10, 15};
    float m_fEnemyGoPath[4][4] = {{0, 0, 0, 0},
                                  {40, 0, 0, 60},
                                  {0, 0, 0, 60},
                                  {-25, 0, 0, 60}};
    bool m_bEnemyWentPath[4][2] = {{true, true},
                                   {false, false},
                                   {false, false},
                                   {false, false}};

    void createGrid();
    void setSquares();
    void collisionEP();
    void collisionPW(Vector2f playerPos, float fTime);
    void collisionPP(int x, int y, int &score);
    void collisionEDS(std::list<Enemy>::iterator it, Vector2f playerPos); //

    Sprite m_sprite;
    Sprite m_right;
    Texture *m_pTexture;
    Shaderprogram m_shader;
    Player *m_pPlayer;


    GLuint m_vao;
    GLuint m_vbo;

    struct Line
    {
        GLuint vao;
        GLuint vbo;
    };

    struct Square
    {
        GLuint vao;
        GLuint vbo;
        GLuint ebo;

        int xPos;
        int yPos;

        bool bCollision;
    };
    struct Point
    {
        int xPos;
        int yPos;

        Sprite sprite;
    };
    struct BigPoint
    {
        int xPos;
        int yPos;
        Sprite sprite;
    };
    struct DecisionSquare
    {
        GLuint vao;
        GLuint vbo;
        GLuint ebo;

        int xPos;
        int yPos;

    };


    std::list<Line> m_lLines;
    std::list<Square> m_lSquares;
    std::list<Point> m_lPoints;
    std::list<Enemy> m_lEnemys;
    std::list<BigPoint> m_lBigPoints;
    std::list<DecisionSquare> m_lDSquare;

    char m_bWand[64][60];

    enum m_eLevel {wall = 1, point, bigPoint, dSquare};
};

#endif