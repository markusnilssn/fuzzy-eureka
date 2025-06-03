#pragma once 
#include "Engine/System.h"
#include <list>
#include <queue>

class Grid;
class Node;

// cloud be a local set 
struct AStarComponent 
{   
    std::set<Node*> currentNodes;
    std::set<Node*> lastNodes;

    Node* endNode;

    std::list<Node*> path;

    float moveTick;
};

class AStarSystem final : public System
{
public:
    struct Weight
    {
        Node* parent;
        
        int gCost;
        int hCost;

        const float fCost() const { return gCost + hCost; }
    };

public:
    explicit AStarSystem(Engine& engine, std::shared_ptr<Grid> grid);

    void Start() override;
    void Destroy() override;

    void Update(float deltaTime) override;

    void GoTo(Entity entity, Node* node);

private:
    friend class FuzzyEureka;
    
    std::shared_ptr<Grid> grid;
    
    static constexpr float moveSpeed = 20.0f;

    const bool IsWalkable(Node* node, Entity entity, const sf::Vector2i& sizeInNodes);
    std::list<Node*> FindPath(Node* startNode, Node* endNode, Entity entity, const sf::Vector2i& sizeInNodes);
    Node* FindNodeWithLowestFCost(const std::list<Node *>& nodes, std::unordered_map<Node*, Weight>& costs);
    static int GetDistanceBetweenNodes(Node* first, Node* second);

};