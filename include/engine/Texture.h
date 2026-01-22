#pragma once
#include <stb_image.h>
#include <iostream>

enum class TextureType {
    DIFFUSE,
    SPECULAR,
    EMISSION,
    NORMAL,
    HEIGHT
};


class Texture{
    public:
        Texture();
        Texture(const char * textureFilePath, int width, int height, TextureType type = TextureType::DIFFUSE);
        unsigned int GetTexture(){return textureID;}
        TextureType GetType(){return type;}
        const char* GetFilePath(){return textureFilePath;}
        void SetType(TextureType newType){type = newType;}
        void SetFilePath(const char* newPath){textureFilePath = newPath;}
        void SetID(unsigned int newID){textureID = newID;}
        unsigned char * GetData(){return data;}
    private:
        int width, height, nrChannels;
        unsigned char *data;
        unsigned int textureID; // OpenGL texture ID
        const char * textureFilePath;
        TextureType type;
};