#pragma once
#include "Math.h"

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <engine/Shader.h>

struct Vertex {
    Vertex() : position(0, 0, 0), normal(0, 0, 0), texCoords(0, 0) {}
    Vector3 position;
    Vector3 normal;
    Vector2 texCoords;
};

struct MeshTexture {
    unsigned int id;
    std::string type;
    std::string path;
};

class Mesh {
    public:
        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<MeshTexture> textures);
        ~Mesh(){};
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<MeshTexture> textures;
        void Draw(Shader &shader);
    private:
        unsigned int VAO, VBO, EBO;
        void SetupMesh();
};