#pragma once 
#include "Engine/System.h"
#include <list>
#include <queue>

class Grid;
class Node;

struct AStarComponent 
{
    Node* currentNode;
    Node* lastNode;

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
    void Render(sf::RenderWindow& window) override;

    void GoTo(Entity entity, Node* node);

private:
    friend class FuzzyEureka;
    
    std::shared_ptr<Grid> grid;
    static constexpr float moveSpeed = 20.0f;

    std::list<Node*> FindPath(Node* startNode, Node* endNode);
    Node* FindNodeWithLowestFCost(const std::list<Node *>& nodes, std::unordered_map<Node*, Weight>& costs);
    static int GetDistanceBetweenNodes(Node* first, Node* second);

};