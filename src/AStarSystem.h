#pragma once 
#include "Engine/System.h"
#include <list>
#include <queue>

#include "Engine/MessageQueue.h"
#include "Grid.h"

// cloud be a local set 
struct NavigationComponent 
{   

    Node* endNode;

    std::list<Node*> path;

    float moveTick;
};

struct ObjectComponent 
{
    std::set<Node*> currentNodes;
    std::set<Node*> lastNodes;
};

class AStarSystem final : public System
{
    struct Weight
    {
        Node* parent;
        
        int gCost;
        int hCost;

        const float fCost() const { return gCost + hCost; }
    };

public:
    explicit AStarSystem(Engine& engine, MessageQueue& messageQueue, Grid& grid);

    void Start() override;
    void Destroy() override;

    void Update(float deltaTime) override;

private:
    friend class FuzzyEureka;
    
    Grid& grid;
    MessageQueue& messageQueue;
    
    static constexpr float moveSpeed = 5.0f;

    const bool IsWalkable(Node* node, Entity entity, const sf::Vector2i& sizeInNodes);
    std::list<Node*> FindPath(Node* startNode, Node* endNode, Entity entity, const sf::Vector2i& sizeInNodes);
    Node* FindNodeWithLowestFCost(const std::list<Node *>& nodes, std::unordered_map<Node*, Weight>& costs);
    static int GetDistanceBetweenNodes(Node* first, Node* second);

};