#ifndef MATH_H
#define MATH_H

static int WIDTH = 696;
static int HEIGHT = 758+120;

inline void initMath(int windowWidht, int windowHeight)
{
    WIDTH = windowWidht;
    HEIGHT = windowHeight;
}

struct Vector2i
{
    Vector2i() {}
    Vector2i(int inX, int inY) {x = inX; y = inY;}
    int x, y;
};
struct Vector2f
{
    Vector2f() {}
    Vector2f(float inX, float inY) {x = inX; y = inY;}
    float x, y;
};

struct Vector3f
{
    Vector3f() {}
    Vector3f(float inX, float inY, float inZ) {x = inX; y = inY; z = inZ;}
    float x, y, z;
};
class Matrix3f
{
public:

    float m[3][3];
    Matrix3f()
    {
        for (unsigned int i = 0 ; i < 3 ; i++)
        {
            for (unsigned int j = 0; j < 3; j++)
            {
                m[i][j] = 0.0f;
            }
        }
    }
    inline Matrix3f operator*(const Matrix3f& Right) const
    {
        Matrix3f Ret;
        for (unsigned int i = 0 ; i < 3 ; i++)
        {
            for (unsigned int j = 0 ; j < 3 ; j++)
            {
                Ret.m[i][j] = m[i][0] * Right.m[0][j] +
                              m[i][1] * Right.m[1][j] +
                              m[i][2] * Right.m[2][j];
            }
        }

        return Ret;
    }
};
inline Vector2f toGLCoord(const Vector2f windowCoords)
{
    Vector2f glCoords;
    glCoords.x = windowCoords.x / (WIDTH/2) - 1;
    glCoords.y = 1 - windowCoords.y / (HEIGHT/2);
    return glCoords;
}
inline Vector2f toWindowCoord(const Vector2f glcoords)
{
    Vector2f winCoords;
    winCoords.x = glcoords.x * (WIDTH/2) + (WIDTH/2);
    winCoords.y = (HEIGHT/2) - glcoords.y * (HEIGHT/2);
    return winCoords;
}

#endif