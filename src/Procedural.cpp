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
    buildings.clear();
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
    
    // Add alleyways: generate smaller roads (blue) that cut across city blocks
    GenerateAlleyways();
    
    // Mark outskirts edges: identify roads that lead to dead ends (these will be rendered in green)
    MarkOutskirtsEdges();
    
    // Extend one of the outskirts roads by 1000 units and generate a new smaller city at its end
    // This creates a connection between the main city and a satellite city
    ExtendLastOutskirtsAndGenerateCity(1000.0f, 30, 2);
    
    // Generate simple cube buildings on each city block (face) in the road network
    GenerateBuildings();
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
        
        // Try to connect to nearest neighbors (4 connections for main roads)
        int connections = 0;
        const int maxConnections = 4;
        
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
        if(face.area < 150.0f) continue; // Lower threshold to generate nodes in more faces
        
        int nodesToGenerate = 2 + static_cast<int>(dist(rng) * 3); // Generate 2-4 nodes per face
        
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
    
    // Convert main road edges into vertex data for OpenGL rendering
    // We exclude alleyways (rendered separately in blue) and outskirts roads (rendered separately in green)
    for(const Edge& edge : allEdges) {
        if(edge.isAlleyway) continue; // Skip alleyways - they have their own rendering method
        if(edge.isOutskirts) continue; // Skip outskirts roads - they're rendered in green separately
        
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

std::vector<float> CityGenerator::GetAlleywayVertices() const {
    std::vector<float> vertices;
    
    // Second pass: alleyways only
    for(const Edge& edge : allEdges) {
        if(!edge.isAlleyway) continue; // Only alleyways
        
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

std::vector<float> CityGenerator::GetOutskirtsVertices() const {
    std::vector<float> vertices;
    
    // Convert outskirts edges (dead-end roads) into vertex data for OpenGL rendering
    // These are the roads that lead to nowhere - we render them in green to distinguish them
    for(const Edge& edge : allEdges) {
        if(!edge.isOutskirts) continue; // Only process edges marked as outskirts
        if(edge.isAlleyway) continue; // Don't include alleyways - they're not considered outskirts roads
        
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

void CityGenerator::MarkOutskirtsEdges() {
    // This function identifies which roads are "outskirts" - roads that lead to dead ends
    // We mark these roads so we can render them in green and potentially extend them to connect to other cities
    
    // First, clear all existing outskirts flags
    for(Edge& edge : allEdges) {
        edge.isOutskirts = false;
    }
    
    // An edge is an outskirts edge if at least one of its endpoint nodes has only 1 connection
    // This means the road leads to a dead end (a node with no other roads connected)
    for(Edge& edge : allEdges) {
        if(edge.isAlleyway) continue; // Don't mark alleyways as outskirts - they're internal roads
        
        // Count how many roads connect to the start node of this edge
        int startConnections = 0;
        for(const Edge& e : allEdges) {
            if(e.start == edge.start || e.end == edge.start) {
                startConnections++;
            }
        }
        
        // Count how many roads connect to the end node of this edge
        int endConnections = 0;
        for(const Edge& e : allEdges) {
            if(e.start == edge.end || e.end == edge.end) {
                endConnections++;
            }
        }
        
        // If either endpoint has only 1 connection, this road leads to a dead end
        // Mark it as an outskirts road so we can render it in green
        if(startConnections == 1 || endConnections == 1) {
            edge.isOutskirts = true;
        }
    }
}

void CityGenerator::ExtendLastOutskirtsAndGenerateCity(float extensionLength, int newCityNodes, int newCityDepth) {
    // This function extends one of the outskirts roads (roads leading to dead ends) by a specified distance
    // and then generates a new smaller city at the end of that extended road
    // This creates a connection between cities via extended roads
    
    // Safety check: prevent infinite recursion if we're already generating a sub-city
    if(isGeneratingSubCity) return;
    
    // Find the second-to-last outskirts edge by iterating backwards through all edges
    // We look for the second one we find (not the last one) to avoid extending the most recent road
    Edge* secondToLastOutskirtsEdge = nullptr;
    size_t foundCount = 0;
    
    for(size_t i = allEdges.size(); i > 0; i--) {
        size_t idx = i - 1;
        if(allEdges[idx].isOutskirts && !allEdges[idx].isAlleyway) {
            foundCount++;
            if(foundCount == 2) {
                // Found the second-to-last outskirts road
                secondToLastOutskirtsEdge = &allEdges[idx];
                break;
            }
        }
    }
    
    if(!secondToLastOutskirtsEdge) return; // Need at least 2 outskirts edges to extend the second-to-last one
    
    // Figure out which end of the road is the dead end (the one with only 1 connection)
    // We need to know this so we can extend the road in the right direction (away from the city)
    int startConnections = 0;
    int endConnections = 0;
    
    for(const Edge& e : allEdges) {
        if(e.start == secondToLastOutskirtsEdge->start || e.end == secondToLastOutskirtsEdge->start) {
            startConnections++;
        }
        if(e.start == secondToLastOutskirtsEdge->end || e.end == secondToLastOutskirtsEdge->end) {
            endConnections++;
        }
    }
    
    Vector3 deadEndPos(0.0f, 0.0f, 0.0f);
    Vector3 otherEndPos(0.0f, 0.0f, 0.0f);
    int deadEndNode = -1;
    
    if(startConnections == 1) {
        // Start node is the dead end - extend from here
        deadEndPos = nodes[secondToLastOutskirtsEdge->start]->position;
        otherEndPos = nodes[secondToLastOutskirtsEdge->end]->position;
        deadEndNode = secondToLastOutskirtsEdge->start;
    } else if(endConnections == 1) {
        // End node is the dead end - extend from here
        deadEndPos = nodes[secondToLastOutskirtsEdge->end]->position;
        otherEndPos = nodes[secondToLastOutskirtsEdge->start]->position;
        deadEndNode = secondToLastOutskirtsEdge->end;
    } else {
        return; // This shouldn't happen for outskirts edges - they should have a dead end
    }
    
    // Calculate the direction vector pointing away from the city
    // We do this by going from the connected end towards the dead end, then continuing that direction
    Vector3 direction(
        deadEndPos.x - otherEndPos.x,
        0.0f,
        deadEndPos.z - otherEndPos.z
    );
    
    // Normalize the direction vector so it has length 1.0
    float dirLength = std::sqrt(direction.x * direction.x + direction.z * direction.z);
    if(dirLength < 0.0001f) return; // Avoid division by zero
    
    direction.x /= dirLength;
    direction.z /= dirLength;
    
    // Calculate where the extended road should end (1000 units away from the dead end)
    Vector3 extensionEnd(
        deadEndPos.x + direction.x * extensionLength,
        0.0f,
        deadEndPos.z + direction.z * extensionLength
    );
    
    // Create a new road node at the extension endpoint
    TreeNode* extensionNode = new TreeNode(extensionEnd, 999.0f);
    int extensionNodeIdx = static_cast<int>(nodes.size());
    nodes.push_back(extensionNode);
    
    // Create the actual road edge connecting the dead end to the new extension node
    Edge extensionEdge;
    extensionEdge.start = deadEndNode;
    extensionEdge.end = extensionNodeIdx;
    extensionEdge.isOutskirts = true; // Mark it as outskirts so it renders in green
    extensionEdge.isAlleyway = false;
    allEdges.push_back(extensionEdge);
    
    UpdateNodeEdges(); // Update the node connections
    
    // Generate a new smaller city at the end of the extended road
    // Set the flag to prevent infinite recursion, then generate the city
    isGeneratingSubCity = true;
    GenerateCityAtPosition(extensionEnd, newCityNodes, newCityDepth, 200.0f);
    isGeneratingSubCity = false;
}

void CityGenerator::GenerateCityAtPosition(const Vector3& centerPosition, int initialNodes, int maxDepth, float cityRadius) {
    // This function generates a new smaller city at a specific position
    // It's used to create satellite cities connected to the main city via extended roads
    
    // Safety check: if we're generating a sub-city, limit the depth to prevent infinite recursion
    if(isGeneratingSubCity && maxDepth > 0) {
        // Don't do recursive face generation for sub-cities - keep them simple
        maxDepth = 0;
    }
    
    // Generate the initial road nodes in a circular area around the center position
    float halfRadius = cityRadius * 0.5f;
    GenerateSeedNodes(initialNodes, 
                      centerPosition.x - halfRadius, 
                      centerPosition.x + halfRadius,
                      centerPosition.z - halfRadius, 
                      centerPosition.z + halfRadius);
    
    // Connect the nodes together to form the road network
    ConnectNodes();
    
    // Filter out edges that are too long or create bad angles (same rules as main city)
    FilterEdges(61.0f, 1000.0f);
    
    // If we have depth remaining, do recursive generation to add more detail
    if(maxDepth > 0) {
        SplitEdges(100.0f); // Split long edges into shorter segments
        std::vector<Face> faces = FindFaces(); // Find the city blocks (faces)
        GenerateNodesInFaces(faces, 0, maxDepth); // Add more nodes inside the faces
    }
    
    // Generate alleyways (smaller roads) between the main roads
    GenerateAlleyways();
    
    // Mark which roads are outskirts (dead ends) so we can render them in green
    MarkOutskirtsEdges();
    
    // Note: We don't generate buildings for sub-cities to keep them simple and fast
    // Buildings are only generated for the main city to improve performance
}

void CityGenerator::GenerateAlleyways() {
    // Find all faces in the current graph
    std::vector<Face> faces = FindFaces();
    
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    
    // For each face, add alleyways that cut across from one side to the opposite
    for(const Face& face : faces) {
        if(face.nodeIndices.size() < 3) continue;
        if(face.area < 150.0f) continue; // Small faces don't get alleys
        
        // Calculate face bounding box to determine orientation
        float minX = std::numeric_limits<float>::max();
        float maxX = std::numeric_limits<float>::lowest();
        float minZ = std::numeric_limits<float>::max();
        float maxZ = std::numeric_limits<float>::lowest();
        
        for(int idx : face.nodeIndices) {
            minX = std::min(minX, nodes[idx]->position.x);
            maxX = std::max(maxX, nodes[idx]->position.x);
            minZ = std::min(minZ, nodes[idx]->position.z);
            maxZ = std::max(maxZ, nodes[idx]->position.z);
        }
        
        float width = maxX - minX;
        float height = maxZ - minZ;
        
        // Scale extend distance with face size to ensure we hit edges on large faces
        float extend = std::max(100.0f, std::max(width, height) * 0.5f);
        
        // Add randomness to offset (not perfectly centered), but keep it within reasonable bounds
        float maxOffsetX = width * 0.3f;
        float maxOffsetZ = height * 0.3f;
        float offsetX = (dist(rng) - 0.5f) * maxOffsetX;
        float offsetZ = (dist(rng) - 0.5f) * maxOffsetZ;
        
        // Create two alleyways: one horizontal (left-to-right) and one vertical (top-to-bottom)
        // Horizontal alleyway (cuts across Z, goes left-to-right)
        if(width > 30.0f) {
            // Extend line beyond face bounds to ensure we intersect edges
            Vector3 startH = Vector3(minX - extend, 0.0f, face.center.z + offsetZ);
            Vector3 endH = Vector3(maxX + extend, 0.0f, face.center.z + offsetZ);
            
            // Find intersection points with face edges
            std::vector<Vector3> intersectionsH = FindLineFaceIntersections(startH, endH, face);
            
            if(intersectionsH.size() >= 2) {
                // Create alleyway nodes at intersection points
                std::vector<int> alleyNodesH;
                for(const Vector3& intersection : intersectionsH) {
                    TreeNode* alleyNode = new TreeNode(intersection, 999.0f);
                    int nodeIdx = static_cast<int>(nodes.size());
                    nodes.push_back(alleyNode);
                    alleyNodesH.push_back(nodeIdx);
                }
                
                // Connect alleyway nodes in sequence
                for(size_t i = 0; i < alleyNodesH.size() - 1; i++) {
                    Edge alleyEdge;
                    alleyEdge.start = alleyNodesH[i];
                    alleyEdge.end = alleyNodesH[i + 1];
                    alleyEdge.isAlleyway = true;
                    
                    // Check for intersections with existing non-alleyway edges only
                    // Allow alleyways to connect at nodes, but prevent crossing main roads
                    bool wouldCross = false;
                    for(const Edge& existing : allEdges) {
                        if(existing.isAlleyway) continue; // Allow alleyways to cross other alleyways
                        if(existing.connects(alleyEdge.start, alleyEdge.end)) continue;
                        
                        // Skip if they share a vertex (can connect at nodes)
                        if(existing.start == alleyEdge.start || existing.start == alleyEdge.end ||
                           existing.end == alleyEdge.start || existing.end == alleyEdge.end) {
                            continue;
                        }
                        
                        if(EdgesIntersect(alleyEdge, existing)) {
                            wouldCross = true;
                            break;
                        }
                    }
                    
                    if(!wouldCross) {
                        allEdges.push_back(alleyEdge);
                    }
                }
            }
        }
        
        // Vertical alleyway (cuts across X, goes top-to-bottom)
        if(height > 30.0f) {
            // Extend line beyond face bounds to ensure we intersect edges (reuse extend from above)
            Vector3 startV = Vector3(face.center.x + offsetX, 0.0f, minZ - extend);
            Vector3 endV = Vector3(face.center.x + offsetX, 0.0f, maxZ + extend);
            
            // Find intersection points with face edges
            std::vector<Vector3> intersectionsV = FindLineFaceIntersections(startV, endV, face);
            
            if(intersectionsV.size() >= 2) {
                // Create alleyway nodes at intersection points
                std::vector<int> alleyNodesV;
                for(const Vector3& intersection : intersectionsV) {
                    TreeNode* alleyNode = new TreeNode(intersection, 999.0f);
                    int nodeIdx = static_cast<int>(nodes.size());
                    nodes.push_back(alleyNode);
                    alleyNodesV.push_back(nodeIdx);
                }
                
                // Connect alleyway nodes in sequence
                for(size_t i = 0; i < alleyNodesV.size() - 1; i++) {
                    Edge alleyEdge;
                    alleyEdge.start = alleyNodesV[i];
                    alleyEdge.end = alleyNodesV[i + 1];
                    alleyEdge.isAlleyway = true;
                    
                    // Check for intersections with existing non-alleyway edges only
                    // Allow alleyways to connect at nodes, but prevent crossing main roads
                    bool wouldCross = false;
                    for(const Edge& existing : allEdges) {
                        if(existing.isAlleyway) continue; // Allow alleyways to cross other alleyways
                        if(existing.connects(alleyEdge.start, alleyEdge.end)) continue;
                        
                        // Skip if they share a vertex (can connect at nodes)
                        if(existing.start == alleyEdge.start || existing.start == alleyEdge.end ||
                           existing.end == alleyEdge.start || existing.end == alleyEdge.end) {
                            continue;
                        }
                        
                        if(EdgesIntersect(alleyEdge, existing)) {
                            wouldCross = true;
                            break;
                        }
                    }
                    
                    if(!wouldCross) {
                        allEdges.push_back(alleyEdge);
                    }
                }
            }
        }
    }
    
    UpdateNodeEdges();
}

std::vector<Vector3> CityGenerator::FindLineFaceIntersections(const Vector3& lineStart, const Vector3& lineEnd, const Face& face) const {
    std::vector<Vector3> intersections;
    
    // Line segment: lineStart to lineEnd (2D, XZ plane)
    float lineDx = lineEnd.x - lineStart.x;
    float lineDz = lineEnd.z - lineStart.z;
    
    // Check intersection with each edge of the face
    for(size_t i = 0; i < face.nodeIndices.size(); i++) {
        int nextIdx = (i + 1) % face.nodeIndices.size();
        Vector3 edgeStart = nodes[face.nodeIndices[i]]->position;
        Vector3 edgeEnd = nodes[face.nodeIndices[nextIdx]]->position;
        
        float edgeDx = edgeEnd.x - edgeStart.x;
        float edgeDz = edgeEnd.z - edgeStart.z;
        
        // 2D line segment intersection using cross product method
        float denom = lineDx * edgeDz - lineDz * edgeDx;
        if(std::abs(denom) < 0.0001f) continue; // Lines are parallel
        
        float diffX = edgeStart.x - lineStart.x;
        float diffZ = edgeStart.z - lineStart.z;
        
        float t = (diffX * edgeDz - diffZ * edgeDx) / denom;
        float u = (diffX * lineDz - diffZ * lineDx) / denom;
        
        // Check if intersection is within both segments
        // t: 0-1 for line segment, u: 0-1 for edge segment
        if(t >= 0.0f && t <= 1.0f && u >= 0.0f && u <= 1.0f) {
            Vector3 intersection = Vector3(
                lineStart.x + t * lineDx,
                0.0f,
                lineStart.z + t * lineDz
            );
            
            // Check for duplicates
            bool isDuplicate = false;
            for(const Vector3& existing : intersections) {
                float dist = Distance(intersection, existing);
                if(dist < 0.5f) {
                    isDuplicate = true;
                    break;
                }
            }
            if(!isDuplicate) {
                intersections.push_back(intersection);
            }
        }
    }
    
    // Sort intersections along the line
    if(intersections.size() > 1) {
        std::sort(intersections.begin(), intersections.end(),
            [&](const Vector3& a, const Vector3& b) {
                float distA = (a.x - lineStart.x) * (a.x - lineStart.x) + (a.z - lineStart.z) * (a.z - lineStart.z);
                float distB = (b.x - lineStart.x) * (b.x - lineStart.x) + (b.z - lineStart.z) * (b.z - lineStart.z);
                return distA < distB;
            });
    }
    
    return intersections;
}

void CityGenerator::GenerateBuildings() {
    // This function creates simple cube buildings on each city block (face) in the road network
    // Each building is placed at the center of a face and sized based on the face's area
    
    buildings.clear();
    std::vector<Face> faces = FindFaces(); // Get all the city blocks (faces bounded by roads)
    std::uniform_real_distribution<float> heightDist(5.0f, 20.0f); // Random height between 5 and 20 units
    
    for(const Face& face : faces) {
        if(face.nodeIndices.size() < 3) continue; // Need at least 3 nodes to form a valid face
        if(face.area < 50.0f) continue; // Skip very small faces - they're too small for buildings
        
        Building building;
        building.position = face.center; // Place building at the center of the city block
        building.position.y = 0.0f; // Ground level (Y = 0)
        
        // Calculate building size based on face area
        // Square root gives us an approximate side length, then we scale it down to 60%
        // This leaves space around the building for roads and walkways
        building.size = std::sqrt(face.area) * 0.6f; // 60% of face size
        building.size = std::max(3.0f, std::min(building.size, 15.0f)); // Clamp between 3 and 15 units
        
        // Give each building a random height to add variety
        building.height = heightDist(rng);
        
        buildings.push_back(building);
    }
}

std::vector<float> CityGenerator::GetBuildingVertices() const {
    // This function converts all the building data into vertex positions for OpenGL rendering
    // Each building is represented as a cube with 36 vertices (6 faces, each with 6 vertices for 2 triangles)
    
    std::vector<float> vertices;
    
    for(const Building& building : buildings) {
        // Calculate the half-dimensions for the building cube
        float halfSize = building.size * 0.5f; // Half the width/depth
        float halfHeight = building.height * 0.5f; // Half the height (not used, but kept for clarity)
        
        Vector3 pos = building.position;
        
        // Generate 36 vertices to form a complete cube (6 faces, each with 6 vertices)
        // Each vertex is 3 floats (x, y, z), so we push 3 values per vertex
        
        // Front face (facing negative Z direction)
        vertices.push_back(pos.x - halfSize); vertices.push_back(pos.y); vertices.push_back(pos.z - halfSize);
        vertices.push_back(pos.x + halfSize); vertices.push_back(pos.y); vertices.push_back(pos.z - halfSize);
        vertices.push_back(pos.x + halfSize); vertices.push_back(pos.y + building.height); vertices.push_back(pos.z - halfSize);
        vertices.push_back(pos.x + halfSize); vertices.push_back(pos.y + building.height); vertices.push_back(pos.z - halfSize);
        vertices.push_back(pos.x - halfSize); vertices.push_back(pos.y + building.height); vertices.push_back(pos.z - halfSize);
        vertices.push_back(pos.x - halfSize); vertices.push_back(pos.y); vertices.push_back(pos.z - halfSize);
        
        // Back face
        vertices.push_back(pos.x - halfSize); vertices.push_back(pos.y); vertices.push_back(pos.z + halfSize);
        vertices.push_back(pos.x + halfSize); vertices.push_back(pos.y); vertices.push_back(pos.z + halfSize);
        vertices.push_back(pos.x + halfSize); vertices.push_back(pos.y + building.height); vertices.push_back(pos.z + halfSize);
        vertices.push_back(pos.x + halfSize); vertices.push_back(pos.y + building.height); vertices.push_back(pos.z + halfSize);
        vertices.push_back(pos.x - halfSize); vertices.push_back(pos.y + building.height); vertices.push_back(pos.z + halfSize);
        vertices.push_back(pos.x - halfSize); vertices.push_back(pos.y); vertices.push_back(pos.z + halfSize);
        
        // Left face
        vertices.push_back(pos.x - halfSize); vertices.push_back(pos.y + building.height); vertices.push_back(pos.z + halfSize);
        vertices.push_back(pos.x - halfSize); vertices.push_back(pos.y + building.height); vertices.push_back(pos.z - halfSize);
        vertices.push_back(pos.x - halfSize); vertices.push_back(pos.y); vertices.push_back(pos.z - halfSize);
        vertices.push_back(pos.x - halfSize); vertices.push_back(pos.y); vertices.push_back(pos.z - halfSize);
        vertices.push_back(pos.x - halfSize); vertices.push_back(pos.y); vertices.push_back(pos.z + halfSize);
        vertices.push_back(pos.x - halfSize); vertices.push_back(pos.y + building.height); vertices.push_back(pos.z + halfSize);
        
        // Right face
        vertices.push_back(pos.x + halfSize); vertices.push_back(pos.y + building.height); vertices.push_back(pos.z + halfSize);
        vertices.push_back(pos.x + halfSize); vertices.push_back(pos.y + building.height); vertices.push_back(pos.z - halfSize);
        vertices.push_back(pos.x + halfSize); vertices.push_back(pos.y); vertices.push_back(pos.z - halfSize);
        vertices.push_back(pos.x + halfSize); vertices.push_back(pos.y); vertices.push_back(pos.z - halfSize);
        vertices.push_back(pos.x + halfSize); vertices.push_back(pos.y); vertices.push_back(pos.z + halfSize);
        vertices.push_back(pos.x + halfSize); vertices.push_back(pos.y + building.height); vertices.push_back(pos.z + halfSize);
        
        // Bottom face
        vertices.push_back(pos.x - halfSize); vertices.push_back(pos.y); vertices.push_back(pos.z - halfSize);
        vertices.push_back(pos.x + halfSize); vertices.push_back(pos.y); vertices.push_back(pos.z - halfSize);
        vertices.push_back(pos.x + halfSize); vertices.push_back(pos.y); vertices.push_back(pos.z + halfSize);
        vertices.push_back(pos.x + halfSize); vertices.push_back(pos.y); vertices.push_back(pos.z + halfSize);
        vertices.push_back(pos.x - halfSize); vertices.push_back(pos.y); vertices.push_back(pos.z + halfSize);
        vertices.push_back(pos.x - halfSize); vertices.push_back(pos.y); vertices.push_back(pos.z - halfSize);
        
        // Top face
        vertices.push_back(pos.x - halfSize); vertices.push_back(pos.y + building.height); vertices.push_back(pos.z - halfSize);
        vertices.push_back(pos.x + halfSize); vertices.push_back(pos.y + building.height); vertices.push_back(pos.z - halfSize);
        vertices.push_back(pos.x + halfSize); vertices.push_back(pos.y + building.height); vertices.push_back(pos.z + halfSize);
        vertices.push_back(pos.x + halfSize); vertices.push_back(pos.y + building.height); vertices.push_back(pos.z + halfSize);
        vertices.push_back(pos.x - halfSize); vertices.push_back(pos.y + building.height); vertices.push_back(pos.z + halfSize);
        vertices.push_back(pos.x - halfSize); vertices.push_back(pos.y + building.height); vertices.push_back(pos.z - halfSize);
    }
    
    return vertices;
}
