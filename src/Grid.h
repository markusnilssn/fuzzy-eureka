#pragma once
#include "Node.h"
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <list>
#include <set>

class Grid 
{
public:
    explicit Grid(const int width, const int height, const sf::Vector2i& nodeSize);
    ~Grid();

    std::set<Node*> NodesUnderRectangle(const sf::FloatRect& rectangle); // Nodes within a rectangle
    std::set<Node*> NodesUnderCircle(const sf::CircleShape& circle); // Nodes within a circle

    Node* NearestNode(Node* node); // Nearest to target that's not blocked
    std::list<Node*> NodesInRange(Node* node, float range); // Nodes within a range
    // std::list<Node*> FindAllNeighbors(Node* node); // Neighbors of a node
    std::list<Node*> FindNeighbors(Node* node); // Neighbors of a node

    Node* NodeFromWorldPosition(const sf::Vector2f& worldPosition); 
    Node* NodeFromAbsolutePosition(const sf::Vector2f& absolutePosition);
    const sf::Vector2f WorldPositionFromNode(Node* node);

    void Lock(Node* nodes, Entity entity = InvalidEntity);
    void Unlock(Node* nodes);

    const bool IsInsideGrid(int x, int y);
    
    [[nodiscard]] const long GetWidth() const;
    [[nodiscard]] const long GetHeight() const;

    [[nodiscard]] const sf::Vector2i& GetNodeSize() const;

    Node* GetNodeAt(const int x, const int y);

    // debug! 
    void Render(sf::RenderWindow& window);

private:
    Node** nodes;

    int width;
    int height;

    sf::Vector2i nodeSize;

};