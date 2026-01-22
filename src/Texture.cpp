#include <engine/Texture.h>
#include <glad/glad.h>

Texture::Texture() : textureID(0), data(nullptr), textureFilePath(nullptr), type(TextureType::DIFFUSE), width(0), height(0), nrChannels(0) {
}

Texture::Texture(const char * textureFilePath, int width, int height, TextureType type) : type(type), textureFilePath(textureFilePath) {
    data = stbi_load(textureFilePath, &width, &height, &nrChannels, 0);
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // TODO: Add safer conditional checks for different image formats
    if (data && textureFilePath[strlen(textureFilePath) - 3] == 'p')
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else if (data && textureFilePath[strlen(textureFilePath) - 3] == 'j')
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        // No texture was found so use the default texture
        data = stbi_load("assets/textures/default_texture.png", &width, &height, &nrChannels, 0);
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
}