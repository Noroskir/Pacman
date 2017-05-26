#include "../include/Level.h"

void Level::init(Texture *pTexture, Player *pPlayer)
{
    m_pTexture = pTexture;
    m_sprite.create(pTexture, Vector2f(0,0), Vector2f(696, 758), Vector2f(232, 256));
    m_sprite.setTexRect(3, -0.33);
    m_pPlayer = pPlayer;

    m_iScale = 3;

    for(int i = 0; i< 64; i++)
    {
        for( int j = 0; j < 60; j++)
        {
            m_bWand[i][j] = 0;
        }
    }

    loadLevel();
    createGrid();
    setSquares();

    m_shader.init("shader/lineShader.vert", "shader/lineShader.frag");

    Vector2f vEnemyPos[4] = {Vector2f(325, 280), Vector2f(285, 350), Vector2f(325, 350), Vector2f(365, 350)};

    int yTexPos[4] = {24, 40, 56, 73};
    for(int i = 0; i < 4; i++)
    {
        Enemy e;
        e.create(pTexture, vEnemyPos[i].x, vEnemyPos[i].y, yTexPos[i]);
        m_lEnemys.push_back(e);
    }
    m_lEnemys.front().m_bOut = true;

    m_bDrawGrid = false;
    m_fGesamtZeit = 0.0f;
    m_iSchwierigkeit = 1;
}
void Level::del()
{
    glDeleteBuffers(1, &m_vbo);
    glDeleteVertexArrays(1, &m_vao);

    std::list<Line>::iterator it = m_lLines.begin();
    while(it != m_lLines.end())
    {
        glDeleteBuffers(1, &it->vbo);
        glDeleteVertexArrays(1, &it->vao);

        it++;
    }
    m_lLines.clear();

    std::list<Square>::iterator itS = m_lSquares.begin();
    while(itS != m_lSquares.end())
    {
        glDeleteBuffers(1, &itS->vbo);
        glDeleteBuffers(1, &itS->ebo);
        glDeleteVertexArrays(1, &itS->vao);

        itS++;
    }
    m_lSquares.clear();


    std::list<Point>::iterator itP = m_lPoints.begin();
    while(itP != m_lPoints.end())
    {
        itP->sprite.del();
        itP++;
    }
    m_lPoints.clear();

    m_sprite.del();
    std::list<Enemy>::iterator itE = m_lEnemys.begin();
    while(itE != m_lEnemys.end())
    {
        itE->del();
        itE++;
    }
    m_lEnemys.clear();
    std::list<BigPoint>::iterator itBE = m_lBigPoints.begin();
    while(itBE != m_lBigPoints.end())
    {
        itBE->sprite.del();
        itBE++;
    }
    m_lBigPoints.clear();
    std::list<DecisionSquare>::iterator itDS = m_lDSquare.begin();
    while(itDS != m_lDSquare.end())
    {
        glDeleteBuffers(1, &itDS->vbo);
        glDeleteBuffers(1, &itDS->ebo);
        glDeleteVertexArrays(1, &itDS->vao);

        itDS++;
    }
}
void Level::handleEvents()
{
    std::list<Enemy>::iterator itE = m_lEnemys.begin();
    while(itE != m_lEnemys.end())
    {
        itE->handleEvents();
        itE++;
    }
}
void Level::update(Vector2f playerPos, float fTime, int &score)
{
    m_fGesamtZeit += fTime;

    std::list<Enemy>::iterator itE = m_lEnemys.begin();
    int counter = 0;
    while(itE != m_lEnemys.end())
    {
        if(m_fGesamtZeit >= m_fEnemyGo[counter])
        {
            itE->update(fTime);
            if(!m_bEnemyWentPath[counter][0])
            {
                m_bEnemyWentPath[counter][0] = itE->movePath(m_fEnemyGoPath[counter][0], m_fEnemyGoPath[counter][1], fTime);
            }
            else if(!m_bEnemyWentPath[counter][1])
            {
                m_bEnemyWentPath[counter][1] = itE->movePath(m_fEnemyGoPath[counter][2], m_fEnemyGoPath[counter][3], fTime);
            }
            else itE->m_bOut = true;
        }
        if (counter <= m_iSchwierigkeit)
        {
            collisionEDS(itE, playerPos);
        }
        if(!itE->m_bAlive)
        {
            itE = m_lEnemys.erase(itE);
        }
        else itE++;
        counter++;
    }

    //Collisionen
    collisionPW(playerPos, fTime);
    int x = static_cast<int>(playerPos.x);
    int y = static_cast<int>(playerPos.y);
    collisionPP(x, y, score);
    collisionEP();
}
void Level::render()
{
    m_sprite.render();

    if(m_bDrawGrid)
    {
        m_shader.use();
        glUniform3f(m_shader.getUniform("gColor"), 1, 0, 0);
        std::list<Line>::iterator it = m_lLines.begin();
        while (it != m_lLines.end())
        {
            glBindVertexArray(it->vao);
            glDrawArrays(GL_LINES, 0, 3);
            glBindVertexArray(0);

            it++;

        }

        std::list<Square>::iterator itS = m_lSquares.begin();
        while (itS != m_lSquares.end())
        {
            if (itS->bCollision)
            {
                glUniform3f(m_shader.getUniform("gColor"), 0, 1, 0);
            }

            glBindVertexArray(itS->vao);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
            glUniform3f(m_shader.getUniform("gColor"), 1, 0, 0);
            itS++;

        }
        std::list<DecisionSquare>::iterator itDS = m_lDSquare.begin();
        while(itDS != m_lDSquare.end())
        {
            glBindVertexArray(itDS->vao);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
            glUniform3f(m_shader.getUniform("gColor"), 0, .4, 1);
            itDS++;
        }

        m_shader.unuse();
    }
    std::list<Point>::iterator itP = m_lPoints.begin();
    while(itP != m_lPoints.end())
    {
        itP->sprite.render();
        itP++;
    }
    std::list<BigPoint>::iterator itBP = m_lBigPoints.begin();
    while(itBP != m_lBigPoints.end())
    {
        itBP->sprite.render();
        itBP++;
    }
    std::list<Enemy>::iterator itE = m_lEnemys.begin();
    while(itE != m_lEnemys.end())
    {
        itE->render();
        itE++;
    }
}
void Level::createGrid()
{
    float starty = 4*m_iScale;
    float yoffset = 4*m_iScale;



    for(int i = 0; i < 64; i++)
    {
        Line l;

        float y = toGLCoord(Vector2f(0, starty + i * yoffset)).y;

        GLfloat vertices[] = {
                -1.0f, y, 0.0f,
                1.0f, y, 0.0f
        };


        glGenVertexArrays(1, &l.vao);
        glGenBuffers(1, &l.vbo);

        glBindVertexArray(l.vao);

        //daten in vbo schreiben
        glBindBuffer(GL_ARRAY_BUFFER, l.vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);


        //color attribut übergeben & aktivieren
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *) 0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindVertexArray(0);

        m_lLines.push_back(l);
    }

    float startx = 4*m_iScale;
    float xoffset = 4*m_iScale;

    for(int i = 0; i < 64; i++)
    {
        Line l;

        float x = toGLCoord(Vector2f(startx + i * xoffset, 0)).x;

        GLfloat vertices[] = {
                x, 1.0f, 0.0f,
                x, -1.0f, 0.0f
        };


        glGenVertexArrays(1, &l.vao);
        glGenBuffers(1, &l.vbo);

        glBindVertexArray(l.vao);

        //daten in vbo schreiben
        glBindBuffer(GL_ARRAY_BUFFER, l.vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);


        //color attribut übergeben & aktivieren
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *) 0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindVertexArray(0);

        m_lLines.push_back(l);
    }

    m_shader.unuse();
}
void Level::setSquares()
{
    float xpos = 0;
    float ypos = 0;
    float yoffset = 4*m_iScale;
    float xoffset = 4*m_iScale;


    for (int j = 0; j < 64; j ++)
    {

        for (int i = 0; i < 60; i++)
        {
            if (m_bWand[j][i] == wall)
            {
                Square s;
                s.bCollision = false;

                s.xPos = i * 4*m_iScale;
                s.yPos = j * 4*m_iScale;

                float y = toGLCoord(Vector2f(0, ypos + j * yoffset)).y;
                float x = toGLCoord(Vector2f(xpos + i * xoffset, 0)).x;

                GLfloat vertices[] = {
                        x, y, 0.0f,                                                                     //top left
                        x + static_cast<float>(4*m_iScale) / static_cast<float>(WIDTH/2), y, 0.0f,     //top right
                        x, y - static_cast<float>(4*m_iScale) / static_cast<float>(HEIGHT/2), 0.0f,     //bottom left
                        x + static_cast<float>(4*m_iScale) / static_cast<float>(WIDTH/2),
                        y - static_cast<float>(4*m_iScale) / static_cast<float>(HEIGHT/2), 0.0f,     //bottom right
                };

                GLuint indices[] = {
                        0, 1, 2,
                        1, 2, 3
                };


                glGenVertexArrays(1, &s.vao);
                glGenBuffers(1, &s.vbo);
                glGenBuffers(1, &s.ebo);

                //alles innerhalb des vao, damit später nur vao callen
                glBindVertexArray(s.vao);

                //daten in vbo schreiben
                glBindBuffer(GL_ARRAY_BUFFER, s.vbo);
                glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

                //daten in element bo schreiebn
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s.ebo);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

                //positions attribut
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *) 0);
                glEnableVertexAttribArray(0);

                glBindBuffer(GL_ARRAY_BUFFER, 0);

                glBindVertexArray(0);

                m_lSquares.push_back(s);
            }
            if(m_bWand[j][i] == point)
            {
                Point p;


                p.xPos = i * 4*m_iScale;
                p.yPos = j * 4*m_iScale;

                p.sprite.create(m_pTexture, Vector2f(p.xPos, p.yPos), Vector2f( 7, 7), Vector2f(3, 3));
                p.sprite.setTexRect(293.5, 83);

                m_lPoints.push_back(p);
            }
            if(m_bWand[j][i] == bigPoint)
            {
                BigPoint p;
                p.xPos = i * 4*m_iScale;
                p.yPos = j * 4*m_iScale;

                p.sprite.create(m_pTexture, Vector2f(p.xPos, p.yPos), Vector2f( 16, 16), Vector2f(8, 8));
                p.sprite.setTexRect(300, 87);

                m_lBigPoints.push_back(p);
            }
            if(m_bWand[j][i] == dSquare)
            {

                DecisionSquare s;
                s.xPos = i * 4*m_iScale;
                s.yPos = j * 4*m_iScale;

                float y = toGLCoord(Vector2f(0, j * 4*m_iScale)).y;
                float x = toGLCoord(Vector2f(i * 4*m_iScale, 0)).x;


                GLfloat vertices[] = {
                        x, y, 0.0f,     //top left
                        x + static_cast<float>(4*m_iScale) / static_cast<float>(WIDTH/2), y, 0.0f,     //top right
                        x, y - static_cast<float>(4*m_iScale) / static_cast<float>(HEIGHT/2), 0.0f,     //bottom left
                        x + static_cast<float>(4*m_iScale) / static_cast<float>(WIDTH/2),
                        y - static_cast<float>(4*m_iScale) / static_cast<float>(HEIGHT/2), 0.0f,     //bottom right
                };



                GLuint indices[] = {
                        0, 1, 2,
                        1, 2, 3
                };


                glGenVertexArrays(1, &s.vao);
                glGenBuffers(1, &s.vbo);
                glGenBuffers(1, &s.ebo);

                //alles innerhalb des vao, damit später nur vao callen
                glBindVertexArray(s.vao);

                //daten in vbo schreiben
                glBindBuffer(GL_ARRAY_BUFFER, s.vbo);
                glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

                //daten in element bo schreiebn
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s.ebo);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

                //positions attribut
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *) 0);
                glEnableVertexAttribArray(0);

                glBindBuffer(GL_ARRAY_BUFFER, 0);

                glBindVertexArray(0);

                m_shader.use();
                glUniform3f(m_shader.getUniform("gColor"), 0, .4, 1);
                m_shader.unuse();

                m_lDSquare.push_back(s);
            }
        }
    }
}
void Level::setWall(float fx, float fy)
{

    fx = fx / (4*m_iScale);
    fy = fy / (4*m_iScale);

    printf("Kästchenkoordinaten: %i, %i\n", static_cast<int>(fx) , static_cast<int>(fy));

    m_bWand[static_cast<int>(fy)][static_cast<int>(fx)] = wall;

    Square s;
    s.xPos = static_cast<int>(fx) * 4*m_iScale;
    s.yPos = static_cast<int>(fy) * 4*m_iScale;
    s.bCollision = false;

    float y = toGLCoord(Vector2f(0, static_cast<int>(fy) * 4*m_iScale)).y;
    float x = toGLCoord(Vector2f(static_cast<int>(fx) * 4*m_iScale, 0)).x;


    GLfloat vertices[] = {
            x, y, 0.0f,     //top left
            x + static_cast<float>(4*m_iScale) / static_cast<float>(WIDTH/2), y, 0.0f,     //top right
            x, y - static_cast<float>(4*m_iScale) / static_cast<float>(HEIGHT/2), 0.0f,     //bottom left
            x + static_cast<float>(4*m_iScale) / static_cast<float>(WIDTH/2),
            y - static_cast<float>(4*m_iScale) / static_cast<float>(HEIGHT/2), 0.0f,     //bottom right
    };



    GLuint indices[] = {
            0, 1, 2,
            1, 2, 3
    };


    glGenVertexArrays(1, &s.vao);
    glGenBuffers(1, &s.vbo);
    glGenBuffers(1, &s.ebo);

    //alles innerhalb des vao, damit später nur vao callen
    glBindVertexArray(s.vao);

    //daten in vbo schreiben
    glBindBuffer(GL_ARRAY_BUFFER, s.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

    //daten in element bo schreiebn
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //positions attribut
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *) 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    m_lSquares.push_back(s);
}
void Level::unsetWall(float fx, float fy)
{
    fx = (fx / (4*m_iScale));
    fy = fy / (4*m_iScale);


    static_cast<int>(fx);
    printf("Kästchenkoordinaten: %i, %i\n", static_cast<int>(fx) , static_cast<int>(fy));

    m_bWand[static_cast<int>(fy)][static_cast<int>(fx)] = false;

    std::list<Square>::iterator itS = m_lSquares.begin();

    while(itS != m_lSquares.end())
    {
        if (static_cast<int>(itS->xPos) == static_cast<int>(fx) * 4*m_iScale
            && static_cast<int>(itS->yPos) == static_cast<int>(fy) * 4*m_iScale
            && !m_bWand[static_cast<int>(fy)][static_cast<int>(fx)])
        {
            glDeleteVertexArrays(1, &itS->vao);
            glDeleteBuffers(1, &itS->vbo);
            glDeleteBuffers(1, &itS->ebo);

            m_lSquares.erase(itS);
            itS = m_lSquares.end();
        }
        else itS++;
    }
}
void Level::setPoint(float fx, float fy)
{
    Point p;
    p.xPos = static_cast<int>(fx/(4*m_iScale)) * 4*m_iScale;
    p.yPos = static_cast<int>(fy/(4*m_iScale)) * 4*m_iScale;

    p.sprite.create(m_pTexture, Vector2f(p.xPos, p.yPos), Vector2f( 7, 7), Vector2f(3, 3));
    p.sprite.setTexRect(293.5, 83);

    m_lPoints.push_back(p);


    m_bWand[p.yPos/(4*m_iScale)][p.xPos/(4*m_iScale)] = point;
}
void Level::unsetPoint(float fx, float fy)
{
    int x = static_cast<int>(fx / (4*m_iScale)) *4*m_iScale;
    int y = static_cast<int>(fy / (4*m_iScale)) * 4*m_iScale;

    m_bWand[y/(4*m_iScale)][x/(4*m_iScale)] = false;
    std::list<Point>::iterator itP = m_lPoints.begin();

    while(itP != m_lPoints.end())
    {
        if ((itP->xPos) == x && (itP->yPos) == y)
        {
            itP->sprite.delWithoutTexture();

            m_lPoints.erase(itP);
            itP = m_lPoints.end();
        }
        else itP++;
    }
}
void Level::setBigPoint(float fx, float fy)
{
    BigPoint p;
    p.xPos = static_cast<int>(fx/(4*m_iScale)) * 4*m_iScale;
    p.yPos = static_cast<int>(fy/(4*m_iScale)) * 4*m_iScale;

    p.sprite.create(m_pTexture, Vector2f(p.xPos, p.yPos), Vector2f( 16, 16), Vector2f(8, 8));
    p.sprite.setTexRect(300, 87);

    m_lBigPoints.push_back(p);

    m_bWand[p.yPos/(4*m_iScale)][p.xPos/(4*m_iScale)] = bigPoint;
}
void Level::unsetBigPoint(float fx, float fy)
{
    int x = static_cast<int>(fx / (4*m_iScale)) *4*m_iScale;
    int y = static_cast<int>(fy / (4*m_iScale)) * 4*m_iScale;

    m_bWand[y/(4*m_iScale)][x/(4*m_iScale)] = false;
    std::list<BigPoint>::iterator itP = m_lBigPoints.begin();

    while(itP != m_lBigPoints.end())
    {
        if ((itP->xPos) == x && (itP->yPos) == y)
        {
            itP->sprite.delWithoutTexture();

            m_lBigPoints.erase(itP);
            itP = m_lBigPoints.end();
        }
        else itP++;
    }
}
void Level::setDSquare(float fx, float fy)
{
    fx = fx / (4*m_iScale);
    fy = fy / (4*m_iScale);

    printf("Kästchenkoordinaten: %i, %i\n", static_cast<int>(fx) , static_cast<int>(fy));

    m_bWand[static_cast<int>(fy)][static_cast<int>(fx)] = dSquare;

    DecisionSquare s;
    s.xPos = static_cast<int>(fx) * 4*m_iScale;
    s.yPos = static_cast<int>(fy) * 4*m_iScale;

    float y = toGLCoord(Vector2f(0, static_cast<int>(fy) * 4*m_iScale)).y;
    float x = toGLCoord(Vector2f(static_cast<int>(fx) * 4*m_iScale, 0)).x;


    GLfloat vertices[] = {
            x, y, 0.0f,     //top left
            x + static_cast<float>(4*m_iScale) / static_cast<float>(WIDTH/2), y, 0.0f,     //top right
            x, y - static_cast<float>(4*m_iScale) / static_cast<float>(HEIGHT/2), 0.0f,     //bottom left
            x + static_cast<float>(4*m_iScale) / static_cast<float>(WIDTH/2),
            y - static_cast<float>(4*m_iScale) / static_cast<float>(HEIGHT/2), 0.0f,     //bottom right
    };



    GLuint indices[] = {
            0, 1, 2,
            1, 2, 3
    };


    glGenVertexArrays(1, &s.vao);
    glGenBuffers(1, &s.vbo);
    glGenBuffers(1, &s.ebo);

    //alles innerhalb des vao, damit später nur vao callen
    glBindVertexArray(s.vao);

    //daten in vbo schreiben
    glBindBuffer(GL_ARRAY_BUFFER, s.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

    //daten in element bo schreiebn
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //positions attribut
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *) 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    m_shader.use();
    glUniform3f(m_shader.getUniform("gColor"), 0, .4, 1);
    m_shader.unuse();

    m_lDSquare.push_back(s);
}
void Level::unsetDSquare(float fx, float fy)
{
    fx = (fx / (4*m_iScale));
    fy = fy / (4*m_iScale);


    static_cast<int>(fx);
    printf("Kästchenkoordinaten: %i, %i\n", static_cast<int>(fx) , static_cast<int>(fy));

    m_bWand[static_cast<int>(fy)][static_cast<int>(fx)] = false;

    std::list<DecisionSquare>::iterator itS = m_lDSquare.begin();

    while(itS != m_lDSquare.end())
    {
        if (static_cast<int>(itS->xPos) == static_cast<int>(fx) * 4*m_iScale
            && static_cast<int>(itS->yPos) == static_cast<int>(fy) * 4*m_iScale
            && !m_bWand[static_cast<int>(fy)][static_cast<int>(fx)])
        {
            glDeleteVertexArrays(1, &itS->vao);
            glDeleteBuffers(1, &itS->vbo);
            glDeleteBuffers(1, &itS->ebo);

            m_lDSquare.erase(itS);
            itS = m_lDSquare.end();
        }
        else itS++;
    }
}
void Level::saveLevel()
{
    std::ofstream output("data/Level/level.dat", std::ios::binary);
    output.write( (char*) &m_bWand, sizeof(m_bWand));

    printf("Saving Level!\n");
}
void Level::loadLevel()
{
    char wand[64][60];
    std::ifstream input("data/Level/level.dat", std::ios::binary);
    input.read((char*) &wand, sizeof(wand));

    for(int j = 0; j < 64; j++)
    {
        for (int i = 0; i < 60; i++)
        {
            m_bWand[j][i] = wand[j][i];
        }
    }


}
void Level::collisionEP()
{
    std::list<Enemy>::iterator itE = m_lEnemys.begin();
    int x = static_cast<int>(m_pPlayer->getPos().x);
    int y = static_cast<int>(m_pPlayer->getPos().y);
    while(itE != m_lEnemys.end())
    {
        if(x >= itE->m_vPos.x && x <= itE->m_vPos.x + 40 && y >= itE->m_vPos.y && y <= itE->m_vPos.y + 40)
        {
            if(!m_pPlayer->m_bEater) m_pPlayer->m_bAlive = false;
            else itE->m_bAlive = false;
        }
        if(x+26 >= itE->m_vPos.x && x+26 <= itE->m_vPos.x + 39 && y >= itE->m_vPos.y && y <= itE->m_vPos.y + 39)
        {
            if(!m_pPlayer->m_bEater) m_pPlayer->m_bAlive = false;
            else itE->m_bAlive = false;
        }
        if(x >= itE->m_vPos.x && x <= itE->m_vPos.x + 39 && y+26 >= itE->m_vPos.y && y+26 <= itE->m_vPos.y + 39)
        {
            if(!m_pPlayer->m_bEater) m_pPlayer->m_bAlive = false;
            else itE->m_bAlive = false;
        }
        if(x+26 >= itE->m_vPos.x && x+26 <= itE->m_vPos.x + 39 && y+26 >= itE->m_vPos.y && y+26 <= itE->m_vPos.y + 39)
        {
            if(!m_pPlayer->m_bEater) m_pPlayer->m_bAlive = false;
            else itE->m_bAlive = false;
        }
        //Collision Enemy & Enemy
        std::list<Enemy>::iterator itE2 = m_lEnemys.begin();
        while(itE2 != m_lEnemys.end())
        {
            if(itE2->m_bOut)
            {
                if (itE == itE2)
                {
                    itE2++;
                }
                else if (itE->m_vPos.x >= itE2->m_vPos.x && itE->m_vPos.x <= itE2->m_vPos.x + 33
                         && itE->m_vPos.y >= itE2->m_vPos.y && itE->m_vPos.y <= itE2->m_vPos.y + 33)
                {
                    itE->oppositeDir();
                    itE2->oppositeDir();
                    itE2++;
                }
                else if (itE->m_vPos.x + 33 >= itE2->m_vPos.x + 33 && itE->m_vPos.x + 33 <= itE2->m_vPos.x + 33
                         && itE->m_vPos.y >= itE2->m_vPos.y && itE->m_vPos.y <= itE2->m_vPos.y + 33)
                {
                    itE->oppositeDir();
                    itE2->oppositeDir();
                    itE2++;
                }
                else if (itE->m_vPos.x >= itE2->m_vPos.x && itE->m_vPos.x <= itE2->m_vPos.x + 33
                         && itE->m_vPos.y + 33 <= itE2->m_vPos.y && itE->m_vPos.y + 33 >= itE2->m_vPos.y + 33)
                {
                    itE->oppositeDir();
                    itE2->oppositeDir();
                    itE2++;
                }
                else if (itE->m_vPos.x + 33 >= itE2->m_vPos.x && itE->m_vPos.x + 33 <= itE2->m_vPos.x + 33
                         && itE->m_vPos.y + 33 <= itE2->m_vPos.y && itE->m_vPos.y + 33 >= itE2->m_vPos.y + 33)
                {
                    itE->oppositeDir();
                    itE2->oppositeDir();
                    itE2++;
                }
                else itE2++;
            }
            else itE2++;
        }
        itE++;
    }
}
void Level::collisionPW(Vector2f playerPos, float fTime)
{
    std::list<Square>::iterator it = m_lSquares.begin();
    std::list<Enemy>::iterator itE;
    while(it != m_lSquares.end())
    {
        //player collision
        if(static_cast<int>(playerPos.x)<= it->xPos + 4*m_iScale && static_cast<int>(playerPos.x) >= it->xPos
           && static_cast<int>(playerPos.y + 1) >= it->yPos && static_cast<int>(playerPos.y + 1) <= it->yPos + 4*m_iScale)
        {
            it->bCollision = true;
            m_pPlayer->move(100 * fTime, 0);
        }
        if(static_cast<int>(playerPos.x)<= it->xPos + 4*m_iScale && static_cast<int>(playerPos.x) >= it->xPos
           && static_cast<int>(playerPos.y + 24) >= it->yPos && static_cast<int>(playerPos.y +24) <= it->yPos + 4*m_iScale)
        {
            it->bCollision = true;
            m_pPlayer->move(100 * fTime, 0);
        }
        if(static_cast<int>(playerPos.x + 1)<= it->xPos + 4*m_iScale && static_cast<int>(playerPos.x + 1) >= it->xPos
           && static_cast<int>(playerPos.y) >= it->yPos && static_cast<int>(playerPos.y) <= it->yPos + 4*m_iScale)
        {
            it->bCollision = true;
            m_pPlayer->move(0, -100 * fTime);
        }
        if(static_cast<int>(playerPos.x + 24)<= it->xPos + 4*m_iScale && static_cast<int>(playerPos.x + 24) >= it->xPos
           && static_cast<int>(playerPos.y) >= it->yPos && static_cast<int>(playerPos.y) <= it->yPos + 4*m_iScale)
        {
            it->bCollision = true;
            m_pPlayer->move(0, -100 * fTime);
        }
        if(static_cast<int>(playerPos.x + 1)<= it->xPos + 4*m_iScale && static_cast<int>(playerPos.x + 1) >= it->xPos
           && static_cast<int>(playerPos.y + 26) >= it->yPos && static_cast<int>(playerPos.y + 26) <= it->yPos + 4*m_iScale)
        {
            it->bCollision = true;
            m_pPlayer->move(0, 100 * fTime);
        }
        if(static_cast<int>(playerPos.x + 24)<= it->xPos + 4*m_iScale && static_cast<int>(playerPos.x + 24) >= it->xPos
           && static_cast<int>(playerPos.y + 26) >= it->yPos && static_cast<int>(playerPos.y + 26) <= it->yPos +4*m_iScale)
        {
            it->bCollision = true;
            m_pPlayer->move(0, 100 * fTime);
        }
        if(static_cast<int>(playerPos.x + 26)<= it->xPos + 4*m_iScale && static_cast<int>(playerPos.x + 26) >= it->xPos
           && static_cast<int>(playerPos.y + 1) >= it->yPos && static_cast<int>(playerPos.y + 1) <= it->yPos + 4*m_iScale)
        {
            it->bCollision = true;
            m_pPlayer->move(-100 * fTime, 0);
        }
        if(static_cast<int>(playerPos.x + 26)<= it->xPos + 4*m_iScale && static_cast<int>(playerPos.x + 26) >= it->xPos
           && static_cast<int>(playerPos.y + 24) >= it->yPos && static_cast<int>(playerPos.y +24) <= it->yPos +4*m_iScale)
        {
            it->bCollision = true;
            m_pPlayer->move(-100 * fTime, 0);

        }
        //enemy collisi
        itE = m_lEnemys.begin();
        while(itE != m_lEnemys.end())
        {
            int iEx = static_cast<int>(itE->m_vPos.x);
            int iEy = static_cast<int>(itE->m_vPos.y);

            if (iEx >= it->xPos && iEx <= it->xPos + 4*m_iScale && iEy >= it->yPos && iEy <= it->yPos + 4*m_iScale)
            {
                if (iEx >= it->xPos && iEx <= it->xPos + 4*m_iScale
                    && iEy + 2 >= it->yPos && iEy + 2 <= it->yPos + 4*m_iScale)
                {
                    itE->move(100 * fTime, 0);
                    it->bCollision = true;
                    itE->m_bCollision = true;

                }
                else if (iEx + 2 >= it->xPos && iEx + 2 < it->xPos + 4*m_iScale
                         && iEy >= it->yPos && iEy <= it->yPos + 4*m_iScale)
                {
                    itE->move(0, -100 * fTime);
                    it->bCollision = true;
                    itE->m_bCollision = true;
                }
            }
            else if (iEx + 33 >= it->xPos && iEx + 33 <= it->xPos +4*m_iScale && iEy >= it->yPos && iEy <= it->yPos +4*m_iScale)
            {
                if (iEx + 31 >= it->xPos && iEx + 31 <= it->xPos + 4*m_iScale
                    && iEy >= it->yPos && iEy <= it->yPos + 4*m_iScale)
                {
                    itE->move(0, -100 * fTime);
                    it->bCollision = true;
                    itE->m_bCollision = true;

                }
                else if (iEx + 33 >= it->xPos && iEx + 33 < it->xPos + 4*m_iScale
                         && iEy + 2 >= it->yPos && iEy + 2 <= it->yPos + 4*m_iScale)
                {
                    itE->move(-100 * fTime, 0);
                    it->bCollision = true;
                    itE->m_bCollision = true;

                }
            }
            else if (iEx >= it->xPos && iEx <= it->xPos + 4*m_iScale && iEy + 33 >= it->yPos && iEy + 33 <= it->yPos + 4*m_iScale)
            {
                if (iEx >= it->xPos && iEx <= it->xPos + 4*m_iScale
                    && iEy + 31 >= it->yPos && iEy + 31 <= it->yPos + 4*m_iScale)
                {
                    itE->move(100 * fTime, 0.0f);
                    it->bCollision = true;
                    itE->m_bCollision = true;

                }
                else if (iEx + 2 >= it->xPos && iEx + 2 < it->xPos + 4*m_iScale
                         && iEy + 33 >= it->yPos && iEy + 33 <= it->yPos + 4*m_iScale)
                {
                    it->bCollision = true;
                    itE->move(0, 100 * fTime);
                    itE->m_bCollision = true;
                }
            }
            else if (iEx + 33 >= it->xPos && iEx + 33 <= it->xPos + 4*m_iScale && iEy + 33 >= it->yPos &&
                     iEy + 33 <= it->yPos + 4*m_iScale)
            {
                if (iEx + 33 >= it->xPos && iEx + 33 <= it->xPos + 4*m_iScale
                    && iEy + 31 >= it->yPos && iEy + 31 <= it->yPos + 4*m_iScale)
                {
                    it->bCollision = true;
                    itE->move(-100 * fTime, 0.0f);
                    itE->m_bCollision = true;
                }
                else if (iEx + 31 >= it->xPos && iEx + 31 < it->xPos + 4*m_iScale
                         && iEy + 33 >= it->yPos && iEy + 33 <= it->yPos + 4*m_iScale)
                {
                    it->bCollision = true;
                    itE->move(0, 100 * fTime);
                    itE->m_bCollision = true;
                }
            }
            itE++;
        }

        it++;
    }
}
void Level::collisionPP(int x, int y, int &score)
{
    //punkte collision
    std::list<Point>::iterator itP = m_lPoints.begin();
    while(itP != m_lPoints.end())
    {
        if(itP->xPos >= x && itP->xPos <= x + 26 && itP->yPos >= y && itP->yPos <= y + 26)
        {
            itP->sprite.delWithoutTexture();
            itP = m_lPoints.erase(itP);
            score += 10;
        }
        if(itP->xPos + 7 >= x && itP->xPos + 7 <= x + 26 && itP->yPos >= y && itP->yPos <= y + 26)
        {
            itP->sprite.delWithoutTexture();
            itP = m_lPoints.erase(itP);
            score += 10;
        }
        if(itP->xPos >= x && itP->xPos <= x + 26 && itP->yPos + 7 >= y && itP->yPos + 7 <= y + 26)
        {
            itP->sprite.delWithoutTexture();
            itP = m_lPoints.erase(itP);
            score += 10;
        }
        if(itP->xPos + 7 >= x && itP->xPos + 7 <= x + 26 && itP->yPos + 7 >= y && itP->yPos + 7 <= y + 26)
        {
            itP->sprite.delWithoutTexture();
            itP = m_lPoints.erase(itP);
            score += 10;
        }
        itP++;
    }
    std::list<BigPoint>::iterator itBP = m_lBigPoints.begin();
    while(itBP != m_lBigPoints.end())
    {
        if(itBP->xPos >= x && itBP->xPos <= x + 26 && itBP->yPos >= y && itBP->yPos <= y + 26)
        {
            itBP->sprite.delWithoutTexture();
            itBP = m_lBigPoints.erase(itBP);
            score += 20;
            m_pPlayer->m_bEater = true;
        }
        if(itBP->xPos + 7 >= x && itBP->xPos + 7 <= x + 26 && itBP->yPos >= y && itBP->yPos <= y + 26)
        {
            itBP->sprite.delWithoutTexture();
            itBP = m_lBigPoints.erase(itBP);
            score += 20;
            m_pPlayer->m_bEater = true;
        }
        if(itBP->xPos >= x && itBP->xPos <= x + 26 && itBP->yPos + 7 >= y && itBP->yPos + 7 <= y + 26)
        {
            itBP->sprite.delWithoutTexture();
            itBP = m_lBigPoints.erase(itBP);
            score += 20;
            m_pPlayer->m_bEater = true;
        }
        if(itBP->xPos + 7 >= x && itBP->xPos + 7 <= x + 26 && itBP->yPos + 7 >= y && itBP->yPos + 7 <= y + 26)
        {
            itBP->sprite.delWithoutTexture();
            itBP = m_lBigPoints.erase(itBP);
            score += 20;
            m_pPlayer->m_bEater = true;
        }
        itBP++;
    }
}
void Level::collisionEDS(std::list<Enemy>::iterator it, Vector2f playerPos)
{
    std::list<DecisionSquare>::iterator itS = m_lDSquare.begin();
    bool bCollision = false;
    while(itS != m_lDSquare.end())
    {
        if(!bCollision)
        {
            if (it->m_vPos.x >= itS->xPos && it->m_vPos.x <= itS->xPos + 4 * m_iScale
                && it->m_vPos.y >= itS->yPos && it->m_vPos.y <= itS->yPos + 4 * m_iScale)
            {
                //printf("Decision Collision!\n");
                bCollision = true;
            }
            /*
            if (it->m_vPos.x + 33 >= itS->xPos && it->m_vPos.x + 33 <= itS->xPos + 4 * m_iScale
                && it->m_vPos.y >= itS->yPos && it->m_vPos.y <= itS->yPos + 4 * m_iScale)
            {
                printf("Decision Collision!\n");
                bCollision = true;
            }
            if (it->m_vPos.x >= itS->xPos && it->m_vPos.x <= itS->xPos + 4 * m_iScale
                && it->m_vPos.y + 33 >= itS->yPos && it->m_vPos.y + 33 <= itS->yPos + 4 * m_iScale)
            {
                printf("Decision Collision!\n");
                bCollision = true;
            }
            if (it->m_vPos.x + 33 >= itS->xPos && it->m_vPos.x + 33 <= itS->xPos + 4 * m_iScale
                && it->m_vPos.y + 33 >= itS->yPos && it->m_vPos.y + 33 <= itS->yPos + 4 * m_iScale)
            {
                printf("Decision Collision!\n");
                bCollision = true;
            }
             */
            itS++;
        }
        else
        {
            if(!it->m_bChaseLock)
            {
                if(std::abs(playerPos.x - it->m_vPos.x) > std::abs(playerPos.y - it->m_vPos.y))
                {

                    if (it->m_vPos.x + 10 <= playerPos.x)
                    {
                        //checken ob wand
                        if (m_bWand[static_cast<int>(it->m_vPos.y / (m_iScale * 4))][static_cast<int>(
                                (it->m_vPos.x + 33 + 8) / (4 * m_iScale))] != wall
                            && m_bWand[static_cast<int>((it->m_vPos.y + 30) / (m_iScale * 4))][static_cast<int>(
                                (it->m_vPos.x + 33 + 8) / (4 * m_iScale))] != wall)
                        {
                            it->changeDir(1, 0);
                            it->m_bChaseLock = true;
                        }
                    }
                    if (it->m_vPos.x - 10 >= playerPos.x)
                    {
                        if (m_bWand[static_cast<int>(it->m_vPos.y / (m_iScale * 4))][static_cast<int>(
                                (it->m_vPos.x - 8) /
                                (4 * m_iScale))] !=
                            wall
                            && m_bWand[static_cast<int>((it->m_vPos.y + 30) / (m_iScale * 4))][static_cast<int>(
                                (it->m_vPos.x - 8) / (4 * m_iScale))] != wall)
                        {
                            it->changeDir(-1, 0);
                            it->m_bChaseLock = true;
                        }
                    }
                }
                else
                {
                    if (it->m_vPos.y - 10 >= playerPos.y)
                    {
                        if (m_bWand[static_cast<int>((it->m_vPos.y - 8) / (m_iScale * 4))][static_cast<int>(
                                (it->m_vPos.x) / (4 * m_iScale))] != wall
                            && m_bWand[static_cast<int>((it->m_vPos.y - 8) / (m_iScale * 4))][static_cast<int>(
                                (it->m_vPos.x + 33) / (4 * m_iScale))] != wall)
                        {
                            it->changeDir(0, 1);
                            it->m_bChaseLock = true;
                        }
                    }
                    if (it->m_vPos.y + 10 <= playerPos.y)
                    {
                        if (m_bWand[static_cast<int>((it->m_vPos.y + 33 + 8) / (m_iScale * 4))][static_cast<int>(
                                (it->m_vPos.x) / (4 * m_iScale))] != wall
                            && m_bWand[static_cast<int>((it->m_vPos.y + 33 + 8) / (m_iScale * 4))][static_cast<int>(
                                (it->m_vPos.x + 33) / (4 * m_iScale))] != wall)
                        {
                            it->changeDir(0, -1);
                            it->m_bChaseLock = true;
                        }
                    }
                }
            }

            itS = m_lDSquare.end();
        }
    }
}