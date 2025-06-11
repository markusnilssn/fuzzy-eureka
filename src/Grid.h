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

    [[nodiscard]] std::set<Node*> NodesUnderRectangle(const sf::FloatRect& rectangle); // Nodes within a rectangle
    [[nodiscard]] std::set<Node*> NodesUnderCircle(const sf::CircleShape& circle); // Nodes within a circle

    [[nodiscard]] Node* NearestNode(Node* node); // Nearest to target that's not blocked
    [[nodiscard]] std::list<Node*> NodesInRange(Node* node, float range); // Nodes within a range
    [[nodiscard]] std::list<Node*> FindNeighbors(Node* node); // Neighbors of a node

    [[nodiscard]] Node* NodeFromWorldPosition(const sf::Vector2f& worldPosition); 
    [[nodiscard]] Node* NodeFromAbsolutePosition(const sf::Vector2f& absolutePosition);
    [[nodiscard]] const sf::Vector2f WorldPositionFromNode(Node* node);

    void Lock(Node* nodes, Entity entity = InvalidEntity);
    void Unlock(Node* nodes);

    [[nodiscard]]const bool IsInsideGrid(int x, int y);
    
    [[nodiscard]] const long GetWidth() const;
    [[nodiscard]] const long GetHeight() const;

    [[nodiscard]] const sf::Vector2i& GetNodeSize() const;

    [[nodiscard]] Node* GetNodeAt(const int x, const int y);

    // debug! 
    void Render(sf::RenderWindow& window);

private:
    Node** nodes;

    int width;
    int height;

    sf::Vector2i nodeSize;

};