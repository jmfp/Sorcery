#pragma once
#include <vector>
#include <engine/Math.h>
#include <random>

struct Edge{
    int start;
    int end;
    bool isAlleyway = false; // Flag to mark alleyway roads
    bool isOutskirts = false; // Flag to mark outskirts roads (dead ends)
    
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

struct Building {
    Vector3 position; // Center position of the building
    float size;       // Size of the building (width/depth, based on face area)
    float height;     // Height of the building
    
    Building() : position(0.0f, 0.0f, 0.0f), size(0.0f), height(0.0f) {}
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
        std::vector<float> GetEdgeVertices() const; // Returns interleaved x,y,z positions for main roads
        std::vector<float> GetAlleywayVertices() const; // Returns interleaved x,y,z positions for alleyways
        std::vector<float> GetOutskirtsVertices() const; // Returns interleaved x,y,z positions for outskirts roads
        std::vector<float> GetBuildingVertices() const; // Returns interleaved x,y,z positions for building cubes
        
        // Mark outskirts edges (roads that lead to dead ends)
        void MarkOutskirtsEdges();
        
        // Extend the last outskirts road and generate a city at its end
        void ExtendLastOutskirtsAndGenerateCity(float extensionLength = 1000.0f, int newCityNodes = 30, int newCityDepth = 2);
        
        // Generate a new city at a specific position
        void GenerateCityAtPosition(const Vector3& position, int initialNodes = 30, int maxDepth = 2, float cityRadius = 200.0f);
        
        // Generate buildings for each face
        void GenerateBuildings();
        
        // Edge crossing detection
        bool EdgesIntersect(const Edge& e1, const Edge& e2) const;
        
    private:
        std::mt19937 rng;
        std::vector<Building> buildings; // Store generated buildings
        bool isGeneratingSubCity = false; // Flag to prevent recursive city generation
        void UpdateNodeEdges();
        void RemoveUnconnectedNodes();
        void GenerateAlleyways(); // Generate smaller roads (alleyways) between main roads
        std::vector<Vector3> FindLineFaceIntersections(const Vector3& lineStart, const Vector3& lineEnd, const Face& face) const;
};
