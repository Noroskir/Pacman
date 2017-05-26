#ifndef SHADERPROGRAMM_H
#define SHADERPROGRAMM_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>

class Shaderprogram
{
public:
    Shaderprogram();
    ~Shaderprogram();

    void init(const char *pathVertexShader, const char *pathFragmentShader);
    void use();
    void unuse();
    void del();

    void addAttrib(int location, int size, GLenum type, bool normalized, int stride, int offset);
    GLuint getUniform(const char *name);

private:
    void loadShader(const char* path, GLuint ID);
    void compileShader(GLuint ID, GLenum shaderType);
    void linkProgram(GLuint ID);

    GLuint m_shaderProg;
    GLuint m_vertShader;
    GLuint m_fragShader;

};

#endif