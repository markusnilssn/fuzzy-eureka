#pragma once 
#include "Engine/System.h"
#include <list>
#include <queue>
#include <set>
#include <unordered_set>
#include <thread>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "Engine/Concurrency.h"

#include "Engine/MessageQueue.h"
#include "Grid.h"
#include <mutex> 
#include "Common/Input.h"

// cloud be a local set 
struct NavigationComponent 
{   
    Node* endNode;

    std::list<Node*> path;
};

struct ObjectComponent 
{
    std::set<Node*> currentNodes;
    std::set<Node*> lastNodes;

    // sf::Vector2f center;
    sf::FloatRect bounds;
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
    explicit AStarSystem(Engine& engine, MessageQueue& messageQueue, Grid& grid, Concurrency& concurrency, Input& input, sf::RenderWindow& window);

    void Start() override;
    void Destroy() override;

    void Update(float deltaTime) override;
    void Render(sf::RenderWindow& window) override;

private:
    friend class FuzzyEureka;
    
    Grid& grid;
    MessageQueue& messageQueue;
    Concurrency& concurrency;
    sf::RenderWindow& window;
    Input& input;

    std::unordered_set<Entity> selectedEntities;
    sf::FloatRect selectionBox;
    sf::Vector2f startPosition;
    bool isSelecting = false;
    
    const bool IsWalkable(Node* node, Entity entity, const sf::Vector2i& sizeInNodes);
    std::list<Node*> FindPath(Node* startNode, Node* endNode, Entity entity, const sf::Vector2i& sizeInNodes);
    Node* FindNodeWithLowestFCost(const std::unordered_set<Node *>& nodes, std::unordered_map<Node*, Weight>& costs);
    static int GetDistanceBetweenNodes(Node* first, Node* second);

};