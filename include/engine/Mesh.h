#pragma once
#include "Math.h"

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <engine/Shader.h>

struct Vertex {
    Vertex() : position(0, 0, 0), normal(0, 0, 0), texCoords(0, 0) {}
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
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
        void Draw(Shader &shader, glm::mat4 model, glm::mat4 view, glm::mat4 projection);
        void SetShader(Shader shader){this->shader = shader;}
    private:
        unsigned int VAO, VBO, EBO;
        void SetupMesh();
        Shader shader;
};