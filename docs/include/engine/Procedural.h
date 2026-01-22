#pragma once
#include <vector>
#include <engine/Math.h>
#include <random>

struct Edge{
    int start;
    int end;
    
    bool connects(int nodeA, int nodeB) const {
        return (start == nodeA && end == nodeB) || (start == nodeB && end == nodeA);
    }
    
    int getOther(int node) const {
        if (start == node) return end;
        if (end == node) return start;
        return -1;
    }
};

struct TreeNode{
    float splitNumber;
    Vector3 position;
    std::vector<TreeNode*> children, parents;
    std::vector<Edge> edges;
    
    TreeNode(Vector3 pos, float split = 0.0f) 
        : position(pos), splitNumber(split) {}
};

struct Face {
    std::vector<int> nodeIndices;
    Vector3 center;
    float area;
    
    Face() : center(0.0f, 0.0f, 0.0f), area(0.0f) {}
};

class Procedural {
    public:
        Procedural(){};
        ~Procedural(){};
};

class CityGenerator {
    public:
        std::vector<TreeNode*> nodes;
        std::vector<Edge> allEdges;
        float seed;
        
        CityGenerator(float seedValue = 0.5f);
        ~CityGenerator();
        
        void Generate(int initialNodes = 10, int maxDepth = 3);
        void GenerateSeedNodes(int count, float minX, float maxX, float minZ, float maxZ);
        void ConnectNodes();
        void FilterEdges(float minAngle = 30.0f, float maxLength = 100.0f);
        void SplitEdges(float maxEdgeLength = 50.0f);
        std::vector<Face> FindFaces();
        void GenerateNodesInFaces(const std::vector<Face>& faces, int depth, int maxDepth);
        
        float Distance(const Vector3& a, const Vector3& b) const;
        float AngleBetweenEdges(const Edge& e1, const Edge& e2, int commonNode) const;
        Vector3 CalculateFaceCenter(const Face& face) const;
        float CalculateFaceArea(const Face& face) const;
        bool PointInFace(const Vector3& point, const Face& face) const;
        
        // Get renderable data for OpenGL
        std::vector<float> GetEdgeVertices() const; // Returns interleaved x,y,z positions for edges
        
        // Edge crossing detection
        bool EdgesIntersect(const Edge& e1, const Edge& e2) const;
        
    private:
        std::mt19937 rng;
        void UpdateNodeEdges();
        void RemoveUnconnectedNodes();
};
