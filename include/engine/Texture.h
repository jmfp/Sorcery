#pragma once
#include <stb_image.h>
#include <iostream>

class Texture{
    public:
        Texture(const char * textureFilePath, int width, int height);
        unsigned int GetTexture(){return texture;}
    private:
        int width, height, nrChannels;
        unsigned char *data;
        unsigned int texture;
};