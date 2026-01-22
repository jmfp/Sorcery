#include <engine/Procedural.h>
#include <algorithm>
#include <cmath>
#include <limits>
#include <functional>
#include <iterator>
#include <tuple>

CityGenerator::CityGenerator(float seedValue) : seed(seedValue), rng(static_cast<unsigned int>(seedValue * 10000)) {
    nodes.clear();
    allEdges.clear();
}

CityGenerator::~CityGenerator() {
    for(auto* node : nodes) {
        delete node;
    }
    nodes.clear();
}

void CityGenerator::Generate(int initialNodes, int maxDepth) {
    // 1000x1000 bounds for very wide, less dense graph
    GenerateSeedNodes(initialNodes, -500.0f, 500.0f, -500.0f, 500.0f);
    ConnectNodes();
    // Filter edges: minimum angle 61 degrees, max length for larger space
    FilterEdges(61.0f, 1000.0f);
    
    // Only do recursive generation if maxDepth > 0
    if(maxDepth > 0) {
        SplitEdges(100.0f); // Increased split threshold for larger space
        std::vector<Face> faces = FindFaces();
        GenerateNodesInFaces(faces, 0, maxDepth);
    }
}

void CityGenerator::GenerateSeedNodes(int count, float minX, float maxX, float minZ, float maxZ) {
    std::uniform_real_distribution<float> distX(minX, maxX);
    std::uniform_real_distribution<float> distZ(minZ, maxZ);
    
    for(int i = 0; i < count; i++) {
        float x = distX(rng);
        float z = distZ(rng);
        nodes.push_back(new TreeNode(Vector3(x, 0.0f, z), 0.0f));
    }
}

void CityGenerator::ConnectNodes() {
    allEdges.clear();
    
    if(nodes.size() < 2) return;
    
    // Simplified Delaunay-like: Connect each node to its nearest neighbors
    // but ONLY if the edge doesn't cross any existing edge
    // This ensures a planar graph
    
    for(size_t i = 0; i < nodes.size(); i++) {
        std::vector<std::pair<float, int>> distances;
        
        for(size_t j = 0; j < nodes.size(); j++) {
            if(i == j) continue;
            float dist = Distance(nodes[i]->position, nodes[j]->position);
            distances.push_back({dist, static_cast<int>(j)});
        }
        
        if(distances.empty()) continue;
        std::sort(distances.begin(), distances.end());
        
        // Try to connect to nearest neighbors (reduced to 3 for less density)
        int connections = 0;
        const int maxConnections = 3;
        
        for(const auto& [dist, j] : distances) {
            if(connections >= maxConnections) break;
            
            // Check if edge already exists
            bool exists = false;
            for(const Edge& e : allEdges) {
                if(e.connects(static_cast<int>(i), j)) {
                    exists = true;
                    break;
                }
            }
            if(exists) continue;
            
            // Create candidate edge
            Edge candidate;
            candidate.start = static_cast<int>(i);
            candidate.end = j;
            
            // Check if this edge would cross ANY existing edge
            bool wouldCross = false;
            for(const Edge& existing : allEdges) {
                // Skip if they share a vertex (can't cross)
                if(existing.start == candidate.start || existing.start == candidate.end ||
                   existing.end == candidate.start || existing.end == candidate.end) {
                    continue;
                }
                
                // Check for intersection
                if(EdgesIntersect(candidate, existing)) {
                    wouldCross = true;
                    break;
                }
            }
            
            // Only add if it doesn't cross
            if(!wouldCross) {
                allEdges.push_back(candidate);
                connections++;
            }
        }
    }
    
    UpdateNodeEdges();
    RemoveUnconnectedNodes();
}

void CityGenerator::FilterEdges(float minAngle, float maxLength) {
    // First pass: filter by length only
    std::vector<Edge> lengthFiltered;
    for(const Edge& edge : allEdges) {
        float length = Distance(nodes[edge.start]->position, nodes[edge.end]->position);
        if(length <= maxLength) {
            lengthFiltered.push_back(edge);
        }
    }
    
    // Second pass: filter by angle, but only remove edges that create angles < minAngle
    // with edges already in the filtered set
    std::vector<Edge> filteredEdges;
    
    for(const Edge& edge : lengthFiltered) {
        bool passesAngleCheck = true;
        
        // Check angles at start node against already-accepted edges
        for(const Edge& acceptedEdge : filteredEdges) {
            if(acceptedEdge.connects(edge.start, edge.end)) continue;
            
            // Check if acceptedEdge connects to start node
            if(acceptedEdge.start == edge.start || acceptedEdge.end == edge.start) {
                float angle = AngleBetweenEdges(edge, acceptedEdge, edge.start);
                // Only reject if angle is significantly less than minAngle
                if(angle < minAngle && angle > 1.0f) {
                    passesAngleCheck = false;
                    break;
                }
            }
        }
        
        // Check angles at end node
        if(passesAngleCheck) {
            for(const Edge& acceptedEdge : filteredEdges) {
                if(acceptedEdge.connects(edge.start, edge.end)) continue;
                
                if(acceptedEdge.start == edge.end || acceptedEdge.end == edge.end) {
                    float angle = AngleBetweenEdges(edge, acceptedEdge, edge.end);
                    if(angle < minAngle && angle > 1.0f) {
                        passesAngleCheck = false;
                        break;
                    }
                }
            }
        }
        
        if(passesAngleCheck) {
            filteredEdges.push_back(edge);
        }
    }
    
    allEdges = filteredEdges;
    UpdateNodeEdges();
}

void CityGenerator::SplitEdges(float maxEdgeLength) {
    std::vector<Edge> newEdges;
    std::vector<TreeNode*> newNodes;
    
    for(const Edge& edge : allEdges) {
        float length = Distance(nodes[edge.start]->position, nodes[edge.end]->position);
        
        if(length > maxEdgeLength) {
            Vector3 startPos = nodes[edge.start]->position;
            Vector3 endPos = nodes[edge.end]->position;
            Vector3 midPos = Vector3(
                (startPos.x + endPos.x) / 2.0f,
                0.0f,
                (startPos.z + endPos.z) / 2.0f
            );
            
            TreeNode* newNode = new TreeNode(midPos, nodes[edge.start]->splitNumber + 1.0f);
            int newNodeIndex = static_cast<int>(nodes.size() + newNodes.size());
            newNodes.push_back(newNode);
            
            Edge edge1;
            edge1.start = edge.start;
            edge1.end = newNodeIndex;
            
            Edge edge2;
            edge2.start = newNodeIndex;
            edge2.end = edge.end;
            
            newEdges.push_back(edge1);
            newEdges.push_back(edge2);
        } else {
            newEdges.push_back(edge);
        }
    }
    
    for(TreeNode* node : newNodes) {
        nodes.push_back(node);
    }
    
    allEdges = newEdges;
    UpdateNodeEdges();
}

std::vector<Face> CityGenerator::FindFaces() {
    std::vector<Face> faces;
    
    // "Follow the wall" algorithm per documentation:
    // Each edge can be part of at most two faces (one on each side)
    // We need to track (edge, direction) pairs, not just edges
    
    // Create set of (edgeIndex, direction) pairs
    // direction: true = start->end, false = end->start
    std::vector<std::pair<size_t, bool>> edgeDirections;
    for(size_t i = 0; i < allEdges.size(); i++) {
        edgeDirections.push_back({i, true});  // A->B
        edgeDirections.push_back({i, false}); // B->A
    }
    
    // While we have unused (edge, direction) pairs
    while(!edgeDirections.empty()) {
        // Pick first unused (edge, direction)
        auto [edgeIdx, forward] = edgeDirections[0];
        const Edge& startEdge = allEdges[edgeIdx];
        
        int startNode = forward ? startEdge.start : startEdge.end;
        int currentNode = forward ? startEdge.end : startEdge.start;
        std::vector<int> faceNodes;
        faceNodes.push_back(startNode);
        faceNodes.push_back(currentNode);
        
        std::vector<std::pair<size_t, bool>> usedInFace;
        usedInFace.push_back({edgeIdx, forward});
        
        // Follow the wall (rightmost edge at each intersection)
        while(currentNode != startNode && faceNodes.size() < 50) {
            int prevNode = faceNodes[faceNodes.size() - 2];
            Vector3 incomingDir = Vector3(
                nodes[currentNode]->position.x - nodes[prevNode]->position.x,
                0.0f,
                nodes[currentNode]->position.z - nodes[prevNode]->position.z
            );
            
            // Normalize incoming direction
            float inLen = std::sqrt(incomingDir.x * incomingDir.x + incomingDir.z * incomingDir.z);
            if(inLen > 0.0001f) {
                incomingDir.x /= inLen;
                incomingDir.z /= inLen;
            }
            
            // Find rightmost edge (largest clockwise angle)
            float bestAngle = -999.0f;
            size_t bestEdgeIdx = SIZE_MAX;
            bool bestForward = true;
            int bestNextNode = -1;
            
            // Check all edges connected to current node
            for(size_t i = 0; i < allEdges.size(); i++) {
                const Edge& e = allEdges[i];
                int otherNode = -1;
                bool edgeForward = false;
                
                if(e.start == currentNode) {
                    otherNode = e.end;
                    edgeForward = true;
                } else if(e.end == currentNode) {
                    otherNode = e.start;
                    edgeForward = false;
                }
                
                if(otherNode == -1 || otherNode == prevNode) continue;
                
                Vector3 outgoingDir = Vector3(
                    nodes[otherNode]->position.x - nodes[currentNode]->position.x,
                    0.0f,
                    nodes[otherNode]->position.z - nodes[currentNode]->position.z
                );
                
                // Normalize
                float outLen = std::sqrt(outgoingDir.x * outgoingDir.x + outgoingDir.z * outgoingDir.z);
                if(outLen < 0.0001f) continue;
                outgoingDir.x /= outLen;
                outgoingDir.z /= outLen;
                
                // Calculate signed angle (cross product gives clockwise angle)
                float cross = incomingDir.x * outgoingDir.z - incomingDir.z * outgoingDir.x;
                float dot = incomingDir.x * outgoingDir.x + incomingDir.z * outgoingDir.z;
                float angle = std::atan2(cross, dot);
                
                if(angle > bestAngle) {
                    bestAngle = angle;
                    bestEdgeIdx = i;
                    bestForward = edgeForward;
                    bestNextNode = otherNode;
                }
            }
            
            if(bestNextNode == -1 || bestEdgeIdx == SIZE_MAX) break;
            
            faceNodes.push_back(bestNextNode);
            usedInFace.push_back({bestEdgeIdx, bestForward});
            currentNode = bestNextNode;
        }
        
        // If we closed the loop, we found a face
        if(currentNode == startNode && faceNodes.size() >= 3) {
            Face face;
            face.nodeIndices = faceNodes;
            face.center = CalculateFaceCenter(face);
            face.area = CalculateFaceArea(face);
            if(face.area > 0.1f) {
                faces.push_back(face);
            }
        }
        
        // Remove used (edge, direction) pairs from our set
        edgeDirections.erase(
            std::remove_if(edgeDirections.begin(), edgeDirections.end(),
                [&](const std::pair<size_t, bool>& ed) {
                    for(const auto& used : usedInFace) {
                        if(ed.first == used.first && ed.second == used.second) {
                            return true;
                        }
                    }
                    return false;
                }),
            edgeDirections.end()
        );
    }
    
    return faces;
}

void CityGenerator::GenerateNodesInFaces(const std::vector<Face>& faces, int depth, int maxDepth) {
    if(depth >= maxDepth) return;
    
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    
    for(const Face& face : faces) {
        if(face.area < 100.0f) continue;
        
        int nodesToGenerate = 1 + static_cast<int>(dist(rng) * 2);
        
        for(int i = 0; i < nodesToGenerate; i++) {
            Vector3 newPos = face.center;
            newPos.x += (dist(rng) - 0.5f) * 20.0f;
            newPos.z += (dist(rng) - 0.5f) * 20.0f;
            
            if(PointInFace(newPos, face)) {
                TreeNode* newNode = new TreeNode(newPos, static_cast<float>(depth + 1));
                nodes.push_back(newNode);
            }
        }
    }
    
    ConnectNodes();
    FilterEdges(61.0f, 1000.0f); // Use same angle and length as main generation
    SplitEdges(100.0f);
    
    std::vector<Face> newFaces = FindFaces();
    GenerateNodesInFaces(newFaces, depth + 1, maxDepth);
}

float CityGenerator::Distance(const Vector3& a, const Vector3& b) const {
    float dx = a.x - b.x;
    float dz = a.z - b.z;
    return std::sqrt(dx * dx + dz * dz);
}

float CityGenerator::AngleBetweenEdges(const Edge& e1, const Edge& e2, int commonNode) const {
    int node1 = e1.getOther(commonNode);
    int node2 = e2.getOther(commonNode);
    
    if(node1 == -1 || node2 == -1) return 180.0f;
    
    Vector3 v1 = Vector3(
        nodes[node1]->position.x - nodes[commonNode]->position.x,
        0.0f,
        nodes[node1]->position.z - nodes[commonNode]->position.z
    );
    Vector3 v2 = Vector3(
        nodes[node2]->position.x - nodes[commonNode]->position.x,
        0.0f,
        nodes[node2]->position.z - nodes[commonNode]->position.z
    );
    
    float dot = v1.x * v2.x + v1.z * v2.z;
    float len1 = std::sqrt(v1.x * v1.x + v1.z * v1.z);
    float len2 = std::sqrt(v2.x * v2.x + v2.z * v2.z);
    
    if(len1 == 0.0f || len2 == 0.0f) return 0.0f;
    
    float cosAngle = dot / (len1 * len2);
    cosAngle = std::max(-1.0f, std::min(1.0f, cosAngle));
    
    return std::acos(cosAngle) * (180.0f / 3.14159265359f);
}

Vector3 CityGenerator::CalculateFaceCenter(const Face& face) const {
    if(face.nodeIndices.empty()) return Vector3(0, 0, 0);
    
    float sumX = 0.0f, sumZ = 0.0f;
    for(int idx : face.nodeIndices) {
        sumX += nodes[idx]->position.x;
        sumZ += nodes[idx]->position.z;
    }
    
    return Vector3(
        sumX / static_cast<float>(face.nodeIndices.size()),
        0.0f,
        sumZ / static_cast<float>(face.nodeIndices.size())
    );
}

float CityGenerator::CalculateFaceArea(const Face& face) const {
    if(face.nodeIndices.size() < 3) return 0.0f;
    
    float area = 0.0f;
    for(size_t i = 0; i < face.nodeIndices.size(); i++) {
        int j = (i + 1) % face.nodeIndices.size();
        area += nodes[face.nodeIndices[i]]->position.x * nodes[face.nodeIndices[j]]->position.z;
        area -= nodes[face.nodeIndices[j]]->position.x * nodes[face.nodeIndices[i]]->position.z;
    }
    return std::abs(area) / 2.0f;
}

bool CityGenerator::PointInFace(const Vector3& point, const Face& face) const {
    int intersections = 0;
    for(size_t i = 0; i < face.nodeIndices.size(); i++) {
        int j = (i + 1) % face.nodeIndices.size();
        Vector3 v1 = nodes[face.nodeIndices[i]]->position;
        Vector3 v2 = nodes[face.nodeIndices[j]]->position;
        
        if(((v1.z > point.z) != (v2.z > point.z)) &&
           (point.x < (v2.x - v1.x) * (point.z - v1.z) / (v2.z - v1.z) + v1.x)) {
            intersections++;
        }
    }
    return (intersections % 2) == 1;
}

void CityGenerator::UpdateNodeEdges() {
    for(TreeNode* node : nodes) {
        node->edges.clear();
    }
    
    for(const Edge& edge : allEdges) {
        if(edge.start >= 0 && edge.start < static_cast<int>(nodes.size()) &&
           edge.end >= 0 && edge.end < static_cast<int>(nodes.size())) {
            nodes[edge.start]->edges.push_back(edge);
            nodes[edge.end]->edges.push_back(edge);
        }
    }
}

bool CityGenerator::EdgesIntersect(const Edge& e1, const Edge& e2) const {
    // Check if two edges intersect in 2D (XZ plane, Y is ignored)
    Vector3 p1 = nodes[e1.start]->position;
    Vector3 p2 = nodes[e1.end]->position;
    Vector3 p3 = nodes[e2.start]->position;
    Vector3 p4 = nodes[e2.end]->position;
    
    // Standard 2D line segment intersection test
    // Two segments (p1,p2) and (p3,p4) intersect if:
    // 1. Points p3 and p4 are on opposite sides of line (p1,p2), AND
    // 2. Points p1 and p2 are on opposite sides of line (p3,p4)
    
    // Helper function to compute 2D cross product (determines orientation)
    auto orient2D = [](const Vector3& a, const Vector3& b, const Vector3& c) -> float {
        return (b.x - a.x) * (c.z - a.z) - (b.z - a.z) * (c.x - a.x);
    };
    
    // Check if p3 and p4 are on opposite sides of line (p1,p2)
    float o1 = orient2D(p1, p2, p3);
    float o2 = orient2D(p1, p2, p4);
    
    // If both have same sign (or both zero), they're on same side - no intersection
    if((o1 > 0 && o2 > 0) || (o1 < 0 && o2 < 0) || (std::abs(o1) < 0.0001f && std::abs(o2) < 0.0001f)) {
        return false;
    }
    
    // Check if p1 and p2 are on opposite sides of line (p3,p4)
    float o3 = orient2D(p3, p4, p1);
    float o4 = orient2D(p3, p4, p2);
    
    // If both have same sign (or both zero), they're on same side - no intersection
    if((o3 > 0 && o4 > 0) || (o3 < 0 && o4 < 0) || (std::abs(o3) < 0.0001f && std::abs(o4) < 0.0001f)) {
        return false;
    }
    
    // Segments intersect (excluding endpoints)
    // Check if they share an endpoint (touching, not crossing)
    float eps = 0.001f;
    bool shareEndpoint = 
        (std::abs(p1.x - p3.x) < eps && std::abs(p1.z - p3.z) < eps) ||
        (std::abs(p1.x - p4.x) < eps && std::abs(p1.z - p4.z) < eps) ||
        (std::abs(p2.x - p3.x) < eps && std::abs(p2.z - p3.z) < eps) ||
        (std::abs(p2.x - p4.x) < eps && std::abs(p2.z - p4.z) < eps);
    
    return !shareEndpoint;
}

void CityGenerator::RemoveUnconnectedNodes() {
    // Remove nodes that have no edges
    std::vector<TreeNode*> connectedNodes;
    std::vector<int> nodeMapping(nodes.size(), -1);
    
    // Find connected nodes
    for(size_t i = 0; i < nodes.size(); i++) {
        if(!nodes[i]->edges.empty()) {
            nodeMapping[i] = static_cast<int>(connectedNodes.size());
            connectedNodes.push_back(nodes[i]);
        } else {
            delete nodes[i];
        }
    }
    
    // Update node references in edges
    for(Edge& edge : allEdges) {
        if(nodeMapping[edge.start] != -1 && nodeMapping[edge.end] != -1) {
            edge.start = nodeMapping[edge.start];
            edge.end = nodeMapping[edge.end];
        }
    }
    
    // Remove edges that reference deleted nodes
    allEdges.erase(
        std::remove_if(allEdges.begin(), allEdges.end(),
            [&](const Edge& e) {
                return nodeMapping[e.start] == -1 || nodeMapping[e.end] == -1;
            }),
        allEdges.end()
    );
    
    nodes = connectedNodes;
    UpdateNodeEdges();
}

std::vector<float> CityGenerator::GetEdgeVertices() const {
    std::vector<float> vertices;
    
    for(const Edge& edge : allEdges) {
        if(edge.start >= 0 && edge.start < static_cast<int>(nodes.size()) &&
           edge.end >= 0 && edge.end < static_cast<int>(nodes.size())) {
            // Add start vertex
            vertices.push_back(nodes[edge.start]->position.x);
            vertices.push_back(nodes[edge.start]->position.y);
            vertices.push_back(nodes[edge.start]->position.z);
            
            // Add end vertex
            vertices.push_back(nodes[edge.end]->position.x);
            vertices.push_back(nodes[edge.end]->position.y);
            vertices.push_back(nodes[edge.end]->position.z);
        }
    }
    
    return vertices;
}
