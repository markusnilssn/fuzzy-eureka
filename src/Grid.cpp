#include "Grid.h"
#include "Common/Debug.h"
#include <iostream>

#include <set>

Grid::Grid(const int width, const int height, const sf::Vector2i& nodeSize)
    : nodes(nullptr)
    , width(width)
    , height(height)
    , nodeSize(nodeSize)
{
    nodes = new Node*[width];
    for (int x = 0; x < width; ++x) 
    {
        nodes[x] = new Node[height];
        for (int y = 0; y < height; y++)
        {
            auto& node = nodes[x][y];
            node.x = x;
            node.y = y;
            node.blocked = false;
            node.owner = InvalidEntity;
            node.worldPosition = sf::Vector2f(x * nodeSize.x, y * nodeSize.y);
            // node.centerPosition = sf::Vector2f(worldPosition.x + nodeSize.x, worldPosition.y + nodeSize.y);
        }
    }
}

Grid::~Grid()
{
    for(int i = 0; i < height; i++) {
        delete[] nodes[i];   
    }

    delete[] nodes;
}

std::set<Node*> Grid::NodesUnderRectangle(const sf::FloatRect& rectangle)
{
    Node* topLeft = NodeFromWorldPosition(rectangle.position);
    Node* bottomRight = NodeFromWorldPosition(rectangle.position + sf::Vector2f(rectangle.size.x - nodeSize.x, rectangle.size.y - nodeSize.y)); // offset 

    std::set<Node*> returnValue;

    int xLeft = std::min(topLeft->x, bottomRight->x);
    int xRight = std::max(topLeft->x, bottomRight->x);
    int yTop = std::min(topLeft->y, bottomRight->y);
    int yBottom = std::max(topLeft->y, bottomRight->y);

    for (int x = xLeft; x <= xRight; ++x)
    {
        for (int y = yTop; y <= yBottom; ++y)
        {
            if (x < 0 || x >= width || y < 0 || y >= height)
            {
                continue;
            }

            returnValue.insert(&nodes[x][y]);
        }
    }

    return returnValue;
}

std::set<Node*> Grid::NodesUnderCircle(const sf::CircleShape &circle)
{
    Node* center = NodeFromWorldPosition(circle.getPosition());
    std::set<Node*> returnValue;

    for (int x = -circle.getRadius(); x <= circle.getRadius(); ++x)
    {
        for (int y = -circle.getRadius(); y <= circle.getRadius(); ++y)
        {
            if (x * x + y * y <= circle.getRadius() * circle.getRadius())
            {
                int nodeX = center->x + x;
                int nodeY = center->y + y;

                if (nodeX < 0 || nodeX >= width || nodeY < 0 || nodeY >= height)
                {
                    continue;
                }

                returnValue.insert(&nodes[nodeX][nodeY]);
            }
        }
    }

    return returnValue;
}

Node* Grid::NearestNode(Node* node)
{
    Node* nearestNode = node;

    for (int x = -1; x <= 1; ++x)
    {
        for (int y = -1; y <= 1; ++y)
        {
            int nodeX = node->x + x;
            int nodeY = node->y + y;

            if (nodeX < 0 || nodeX >= width || nodeY < 0 || nodeY >= height)
            {
                continue;
            }

            Node* currentNode = &nodes[nodeX][nodeY];

            // if (!currentNode->isBlocked && currentNode->fCost() < nearestNode->fCost())
            // {
                nearestNode = currentNode;
            // }
        }
    }

    return nearestNode;
}

std::list<Node*> Grid::NodesInRange(Node* node, float range)
{
    std::list<Node*> returnValue;

    for (int x = -range; x <= range; ++x)
    {
        for (int y = -range; y <= range; ++y)
        {
            if (x * x + y * y <= range * range)
            {
                int nodeX = node->x + x;
                int nodeY = node->y + y;

                if (nodeX < 0 || nodeX >= width || nodeY < 0 || nodeY >= height)
                {
                    continue;
                }

                returnValue.push_back(&nodes[nodeX][nodeY]);
            }
        }
    }

    return returnValue;
}

std::list<Node*> Grid::FindNeighbors(Node* node)
{
    std::list<Node*> returnValue;

    for (int x = -1; x <= 1; ++x)
    {
        for (int y = -1; y <= 1; ++y)
        {
            if ((x == 0 && y == -1)
                || (x == -1 && y == 0)
                || (x == 1 && y == 0)
                || (x == 0 && y == 1))
            {   
                int nodeX = node->x + x;
                int nodeY = node->y + y;
                
                if (nodeX < 0 || nodeX >= width || nodeY < 0 || nodeY >= height)
                {
                    continue;
                }
                
                returnValue.push_back(&nodes[nodeX][nodeY]);
            }
        }
    }

    return returnValue;
}

Node* Grid::NodeFromWorldPosition(const sf::Vector2f &worldPosition)
{
    float xOffset = nodeSize.x / 2.0f;
    float yOffset = nodeSize.y / 2.0f;

    int x = static_cast<int>((worldPosition.x + xOffset) / nodeSize.x);
    int y = static_cast<int>((worldPosition.y + yOffset) / nodeSize.y);

    if (x < 0 || x >= width || y < 0 || y >= height)
    {
        return nullptr;
    }

    return &nodes[x][y];
}

Node *Grid::NodeFromAbsolutePosition(const sf::Vector2f &absolutePosition)
{
    int x = static_cast<int>(absolutePosition.x / nodeSize.x);
    int y = static_cast<int>(absolutePosition.y / nodeSize.y);

    if (x < 0 || x >= width || y < 0 || y >= height)
    {
        return nullptr;
    }

    return &nodes[x][y];
}

const sf::Vector2f Grid::WorldPositionFromNode(Node* node)
{
    return sf::Vector2f((node->x * nodeSize.x), (node->y * nodeSize.y));
}

void Grid::Lock(Node* node, Entity entity)
{
    if(node == nullptr)
    {  
        Debug::LogWarning("Trying to lock a nullptr object");
        return;
    }

    node->blocked = true;
    node->owner = entity;

}

void Grid::Unlock(Node* node)
{
    if(node == nullptr)
    {
        Debug::LogWarning("Trying to lock a nullptr object");
        return;
    }

    node->blocked = false;
    node->owner = InvalidEntity;
}

const bool Grid::IsInsideGrid(int x, int y)
{
    return x >= 0 && x < width && y >= 0 && y < height;
}


const long Grid::GetWidth() const
{
    return width;
}

const long Grid::GetHeight() const
{
    return height;
}

const sf::Vector2i &Grid::GetNodeSize() const
{
    return nodeSize;
}

Node* Grid::GetNodeAt(const int x, const int y)
{
    if(!IsInsideGrid(x, y))
        return nullptr;

    return &nodes[x][y];
}

void Grid::Render(sf::RenderWindow &window)
{
    const sf::View& view = window.getView();
    sf::Vector2f offset(nodeSize.x, nodeSize.y);
    sf::Vector2f size = view.getSize() + offset;
    sf::Vector2f center = (view.getCenter() - size / 2.0f) - offset;
    sf::FloatRect viewport(center, size);

    for (int x = 0; x < width; ++x)
    {
        for (int y = 0; y < height; ++y)
        {
            sf::Vector2f position(x * nodeSize.x, y * nodeSize.y);
            if(!viewport.contains(position))
                continue;


            sf::RectangleShape rectangle({(float)nodeSize.x, (float)nodeSize.y});
            rectangle.setPosition(position);

            uint8_t alpha = 30;
            if (nodes[x][y].blocked)
            {
                rectangle.setFillColor(sf::Color(255, 0, 0, alpha));
            }
            else
            {
                rectangle.setFillColor(sf::Color(0, 255, 0, 0));
            }

            rectangle.setOutlineColor(sf::Color::Black);
            rectangle.setOutlineThickness(0.5f);

            window.draw(rectangle);
        }
    }
}