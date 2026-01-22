#pragma once
#include "Math.h"

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <engine/Shader.h>
#include <engine/Texture.h>

struct Vertex {
    Vertex() : position(0, 0, 0), normal(0, 0, 0), texCoords(0, 0) {}
    Vertex(glm::vec3 pos, glm::vec3 norm, glm::vec2 tex) : position(pos), normal(norm), texCoords(tex) {}
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

class Mesh {
    public:
        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture*> textures);
        ~Mesh(){};
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture*> textures;
        void Draw(Shader &shader, glm::mat4 model, glm::mat4 view, glm::mat4 projection);
        void SetShader(Shader shader){this->shader = shader;}
        void Bind(){glBindVertexArray(VAO);}
        void Unbind(){glBindVertexArray(0);}
        unsigned int GetIndexCount(){return indices.size();}
    private:
        unsigned int VAO, VBO, EBO;
        void SetupMesh();
        Shader shader;
};