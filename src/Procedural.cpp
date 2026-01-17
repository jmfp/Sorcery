#include <engine/Procedural.h>

CityGenerator::CityGenerator(float seedValue) {
    seed = seedValue;
    nodes = std::vector<TreeNode*>();
    for(int i = 0; i < nodes.size(); i++) {
        delete nodes[i];
    }
    nodes.clear();
}