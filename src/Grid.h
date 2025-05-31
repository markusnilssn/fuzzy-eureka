#pragma once
#include "Node.h"
#include <list>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "PearlyNoise.hpp"

class Grid 
{
public:
    explicit Grid(const int width, const int height, const sf::Vector2i& nodeSize);

    std::list<Node*> NodesUnderRectangle(const sf::IntRect& rectangle); // Nodes within a rectangle
    std::list<Node*> NodesUnderCircle(const sf::CircleShape& circle); // Nodes within a circle

    Node* NearestNode(Node* node); // Nearest to target that's not blocked
    std::list<Node*> NodesInRange(Node* node, float range); // Nodes within a range
    std::list<Node*> FindNeighbors(Node* node); // Neighbors of a node

    Node* NodeFromWorldPosition(const sf::Vector2f& worldPosition); 
    const sf::Vector2f WorldPositionFromNode(Node* node);

    void Lock(Node* node, Entity entity = InvalidEntity);
    void Unlock(Node* node);
    
    [[nodiscard]] const long GetWidth() const;
    [[nodiscard]] const long GetHeight() const;

    [[nodiscard]] const sf::Vector2i& GetNodeSize() const;

    Node* GetNode(const int x, const int y);

    // debug! 
    void Render(sf::RenderWindow& window);

private:
    Node** nodes;

    int width;
    int height;

    sf::Vector2i nodeSize;

};