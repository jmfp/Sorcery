#pragma once
#include <vector>
#include <engine/Mesh.h>
#include <engine/Shader.h>
#include <engine/Camera.h>
#include <string>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>
using namespace std;

class Model
{
    public:
        Model(char* path){
            loadModel(path);
        };
        vector<MeshTexture> textures_loaded;
        void SetShader(Shader shader){
            for(unsigned int i = 0; i < meshes.size(); i++)
                meshes[i].SetShader(shader);
        }
        void SetMatrices(glm::mat4 model, glm::mat4 view, glm::mat4 projection);
        void Draw(Shader &shader);
        // TODO: get camera automatically from parent scene
        void SetCamera(Camera* camera){
            mainCamera = camera;
        }
    private:
        Camera* mainCamera = nullptr;
        vector<Mesh> meshes;
        string directory;
        glm::mat4 modelMatrix;
        glm::mat4 viewMatrix;
        glm::mat4 projectionMatrix;
        void loadModel(string path);
        void processNode(aiNode* node, const aiScene* scene);
        Mesh processMesh(aiMesh* mesh, const aiScene* scene);
        vector<MeshTexture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
        unsigned int TextureFromFile(const char *path, const string &directory, bool gamma);
};