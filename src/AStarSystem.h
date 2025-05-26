#pragma once 
#include "Engine/System.h"
#include "Node.h"
#include <list>

class AStarComponent 
{
    Node* lastNode;
    Node* currentNode;

    std::list<Node*> path;

    bool isMoving;
    bool onTarget;
};

class AStarSystem final : public System
{
public:
    AStarSystem(Engine& engine);

    void Start() override;
    void Destroy() override;

    void Update(float deltaTime) override;
    void Render(sf::RenderWindow& window) override;

    void MoveEntity(Entity entity, const sf::Vector2f& move);

    void OccupyNode(Entity entity, Node& node);

private:


};