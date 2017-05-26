#include "../include/Texture.h"

void Texture::loadTexture(const char *texturePath, GLuint &ID)
{

    unsigned char *image = SOIL_load_image(texturePath, &m_fWidth, &m_fHeight, 0, SOIL_LOAD_RGBA);

    if( image == nullptr) std::cout << "Error loading Texture: " << texturePath << std::endl;

    glGenTextures(1, &m_texID);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_fWidth, m_fHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);

    SOIL_free_image_data(image);

    glBindTexture(GL_TEXTURE_2D, 0);

    ID = m_texID;
}
GLuint Texture::getTexture()
{
    return m_texID;
}
void Texture::getMesures(int &x, int &y)
{
    x = m_fWidth;
    y = m_fHeight;
}