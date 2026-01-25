#pragma once
#include <vector>
#include <engine/Math.h>
#include <random>

struct Edge{
    int start;
    int end;
    bool isAlleyWay = false;
    bool isOutskirts = false;

    // checks if the edge connects the two nodes
    bool connects(int nodeA, int nodeB ) {
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
    std::vector<Edge> edges; //specifically edges that connect to this node

    TreeNode(Vector3 pos, float split = 0.0f) : position(pos), splitNumber(split) {}
};

struct Face {
    std::vector<int> indices;
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
        ~CityGenerator(){};
        void Generate(int initialNodes = 10, int maxDepth = 3);
        void GenerateSeedNodes(int count, float minX, float maxX, float minZ, float maxZ); // TODO: add y axis to cover terrain
        void ConnectNodes();
        void FilterEdges(float minAngle = 30.0f, float maxLength = 100.0f);
        void SplitEdges(float maxEdgeLength = 75.0f);
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

        void MarkOutskirtsEdges();
        bool EdgesIntersect(const Edge& e1, const Edge& e2) const;
    private:
        std::mt19937 rng;
        // std::vector<Building> buildings; // Store generated buildings
        // bool isGeneratingSubCity = false; // Flag to prevent recursive city generation
        void UpdateNodeEdges();
        void RemoveUnconnectedNodes();
        void GenerateAlleyways(); // Generate smaller roads (alleyways) between main roads
        std::vector<Vector3> FindLineFaceIntersections(const Vector3& lineStart, const Vector3& lineEnd, const Face& face) const;

};