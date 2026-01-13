#pragma once
#include <vector>
#include <engine/Mesh.h>
#include <engine/Shader.h>
#include <string>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
using namespace std;

class Model
{
    public:
        Model(char* path){
            loadModel(path);
        };
        vector<MeshTexture> textures_loaded;
        void Draw(Shader &shader);
    private:
        vector<Mesh> meshes;
        string directory;
        void loadModel(string path);
        void processNode(aiNode* node, const aiScene* scene);
        Mesh processMesh(aiMesh* mesh, const aiScene* scene);
        vector<MeshTexture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
        unsigned int TextureFromFile(const char *path, const string &directory, bool gamma);
};