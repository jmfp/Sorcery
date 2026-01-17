#pragma once
#include <vector>
#include <engine/Math.h>

struct Edge{
    int start;
    int end;
};

struct TreeNode{
    float splitNumber;
    Vector3 position;
    std::vector<TreeNode*> children, parents;
    std::vector<Edge> edges; //specifically edges that connect to this node
};

class Procedural {
    public:
        Procedural(){};
        ~Procedural(){};
};


class CityGenerator {
    public:
        std::vector<TreeNode*> nodes;
        float seed;
        CityGenerator(float seedValue = 0.5f);
        ~CityGenerator(){};
    private:

};