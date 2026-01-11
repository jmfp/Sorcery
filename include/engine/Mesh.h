#pragma once

class Mesh {
    public:
        Mesh(){};
        ~Mesh(){};
        void Draw(){};
        float* GetVertices(){};
        void SetVertices(float newVertices[]){vertices = newVertices;};
        unsigned int* GetIndices(){};
        int GetVertexCount(){};
        int GetIndexCount(){};
        float* vertices;
        unsigned int indices[];
};