#include <engine/Texture.h>
#include <glad/glad.h>

Texture::Texture() : textureFilePath(nullptr), type(TextureType::DIFFUSE), width(512), height(512), nrChannels(4) {
    // No texture file path was provided so use the default texture
    data = stbi_load("../src/textures/default_texture.png", &width, &height, &nrChannels, 0);
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cerr << "Failed to load default texture!" << std::endl;
    }
}

Texture::Texture(const char * textureFilePath, int width, int height, TextureType type) : type(type), textureFilePath(textureFilePath) {
    data = stbi_load(textureFilePath, &width, &height, &nrChannels, 0);
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // TODO: Add safer conditional checks for different image formats with nr channels
    GLenum formatFlag = textureFilePath[strlen(textureFilePath) - 3] == 'j' ? GL_RGB : GL_RGBA;
    if (data)
    {
        try {
            glTexImage2D(GL_TEXTURE_2D, 0, formatFlag, width, height, 0, formatFlag, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        } catch (const std::exception& e) {
            stbi_image_free(data);
            data = stbi_load("../src/textures/default_texture.png", &width, &height, &nrChannels, 0);
            if (data) {
                GLenum defaultFormat = nrChannels == 3 ? GL_RGB : GL_RGBA;
                glTexImage2D(GL_TEXTURE_2D, 0, defaultFormat, width, height, 0, defaultFormat, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);
                std::cout << "Failed to load texture: " << textureFilePath << " default texture used" << std::endl;
            } else {
                std::cerr << "Error loading texture: " << e.what() << std::endl;
                std::cerr << "Failed to load default texture as well!" << std::endl;
            }
        }
    }
    else
    {
        // No texture was found so use the default texture
        data = stbi_load("../src/textures/default_texture.png", &width, &height, &nrChannels, 0);
        if (data) {
            GLenum defaultFormat = nrChannels == 3 ? GL_RGB : GL_RGBA;
            glTexImage2D(GL_TEXTURE_2D, 0, defaultFormat, width, height, 0, defaultFormat, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
            std::cout << "Failed to load texture: " << textureFilePath << ", using default texture" << std::endl;
        } else {
            std::cerr << "Failed to load texture: " << textureFilePath << " and default texture failed to load!" << std::endl;
        }
    }
    stbi_image_free(data);
}