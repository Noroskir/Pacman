#include "../include/Shaderprogram.h"

Shaderprogram::Shaderprogram()
{

}
Shaderprogram::~Shaderprogram()
{

}
void Shaderprogram::init(const char *pathVertexShader, const char *pathFragmentShader)
{
    m_vertShader = glCreateShader(GL_VERTEX_SHADER);
    m_fragShader = glCreateShader(GL_FRAGMENT_SHADER);

    loadShader(pathVertexShader, m_vertShader);
    loadShader(pathFragmentShader, m_fragShader);

    compileShader(m_vertShader, GL_VERTEX_SHADER);
    compileShader(m_fragShader, GL_FRAGMENT_SHADER);

    m_shaderProg = glCreateProgram();

    glAttachShader(m_shaderProg, m_vertShader);
    glAttachShader(m_shaderProg, m_fragShader);
    linkProgram(m_shaderProg);

    glDeleteShader(m_vertShader);
    glDeleteShader(m_fragShader);
}
void Shaderprogram::del()
{
    glDeleteProgram(m_shaderProg);
}
void Shaderprogram::use()
{
    glUseProgram(m_shaderProg);
}
void Shaderprogram::unuse()
{
    glUseProgram(0);
}
void Shaderprogram::loadShader(const char *path, GLuint ID)
{
    try
    {
        std::ifstream file;
        file.open(path);

        std::string buffer, line;
        while (std::getline(file, line))
        {
            buffer += "\n" + line;
        }

        file.close();

        const char *p = buffer.c_str();
        glShaderSource(ID, 1, &p, 0);
    }
    catch(int fail)
    {
        std::cout << "ERROR::Could not read Shader from file!" << std::endl;
    }
}
void Shaderprogram::addAttrib(int location, int size, GLenum type, bool normalized, int stride, int offset)
{
    glVertexAttribPointer(location, size, type, normalized, stride, (GLvoid*) offset);
    glEnableVertexAttribArray(location);
}
void Shaderprogram::compileShader(GLuint ID, GLenum shaderType)
{
    glCompileShader(ID);

    GLint success;
    glGetShaderiv(ID, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        GLchar InfoLog[1024];
        glGetShaderInfoLog(ID, sizeof(InfoLog), NULL, InfoLog);
        fprintf(stderr, "Error compiling shader type %d: '%s'\n", shaderType, InfoLog);
    }
}
void Shaderprogram::linkProgram(GLuint ID)
{
    glLinkProgram(ID);

    GLint success;
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (success == 0)
    {
        GLchar InfoLog[1024];
        glGetProgramInfoLog(ID, sizeof(InfoLog), NULL, InfoLog);
        fprintf(stderr, "Error linking shader program: '%s'\n", InfoLog);
    }
    glValidateProgram(ID);
}
GLuint Shaderprogram::getUniform(const char *name)
{
    return glGetUniformLocation(m_shaderProg, name);
}