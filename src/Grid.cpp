#include "Grid.h"

Grid::Grid(const int width, const int height, const sf::Vector2i& nodeSize)
    : nodes(nullptr)
    , width(width)
    , height(height)
    , nodeSize(nodeSize)
{
    nodes = new Node*[width];
    for (long x = 0; x < width; ++x)
        nodes[x] = new Node[height];
}

std::list<Node> Grid::NodesUnderRectangle(const sf::IntRect &rectangle)
{
    Node topLeft = NodeFromWorldPosition(sf::Vector2f(rectangle.position.x, rectangle.position.y));
    Node bottomRight = NodeFromWorldPosition(sf::Vector2f(rectangle.position.x + rectangle.size.x, rectangle.position.y + rectangle.size.y));

    std::list<Node> returnValue;

    Node::Index xLeft = std::min(topLeft.x, bottomRight.x);
    Node::Index xRight = std::max(topLeft.x, bottomRight.x);
    Node::Index yTop = std::min(topLeft.y, bottomRight.y);
    Node::Index yBottom = std::max(topLeft.y, bottomRight.y);

    for (Node::Index x = xLeft; x <= xRight; ++x)
    {
        for (Node::Index y = yTop; y <= yBottom; ++y)
        {
            if (x < 0 || x >= width || y < 0 || y >= height)
            {
                continue;
            }

            returnValue.push_back(nodes[x][y]);
        }
    }

    return returnValue;
}

std::list<Node> Grid::NodesUnderCircle(const sf::CircleShape &circle)
{
    Node center = NodeFromWorldPosition(circle.getPosition());
    std::list<Node> returnValue;

    for (int x = -circle.getRadius(); x <= circle.getRadius(); ++x)
    {
        for (int y = -circle.getRadius(); y <= circle.getRadius(); ++y)
        {
            if (x * x + y * y <= circle.getRadius() * circle.getRadius())
            {
                Node::Index nodeX = center.x + x;
                Node::Index nodeY = center.y + y;

                if (nodeX < 0 || nodeX >= width || nodeY < 0 || nodeY >= height)
                {
                    continue;
                }

                returnValue.push_back(nodes[nodeX][nodeY]);
            }
        }
    }

    return returnValue;
}

Node Grid::NearestNode(Node& node)
{
    Node nearestNode = node;

    for (int x = -1; x <= 1; ++x)
    {
        for (int y = -1; y <= 1; ++y)
        {
            Node::Index nodeX = node.x + x;
            Node::Index nodeY = node.y + y;

            if (nodeX < 0 || nodeX >= width || nodeY < 0 || nodeY >= height)
            {
                continue;
            }

            Node& currentNode = nodes[nodeX][nodeY];

            if (!currentNode.isBlocked && currentNode.fCost() < nearestNode.fCost())
            {
                nearestNode = currentNode;
            }
        }
    }

    return nearestNode;
}

std::list<Node> Grid::NodesInRange(Node& node, float range)
{
    std::list<Node> returnValue;

    for (int x = -range; x <= range; ++x)
    {
        for (int y = -range; y <= range; ++y)
        {
            if (x * x + y * y <= range * range)
            {
                Node::Index nodeX = node.x + x;
                Node::Index nodeY = node.y + y;

                if (nodeX < 0 || nodeX >= width || nodeY < 0 || nodeY >= height)
                {
                    continue;
                }

                returnValue.push_back(nodes[nodeX][nodeY]);
            }
        }
    }

    return returnValue;
}

std::list<Node> Grid::FindNeighbors(Node& node)
{
    std::list<Node> returnValue;

    for (int x = -1; x <= 1; ++x)
    {
        for (int y = -1; y <= 1; ++y)
        {
            if ((x == 0 && y == -1)
            || (x == -1 && y == 0)
            || (x == 1 && y == 0)
            || (x == 0 && y == 1))
            {
                continue;
            }

            Node::Index nodeX = node.x + x;
            Node::Index nodeY = node.y + y;

            if (nodeX < 0 || nodeX >= width || nodeY < 0 || nodeY >= height)
            {
                continue;
            }

            returnValue.push_back(nodes[nodeX][nodeY]);
        }
    }

    return returnValue;
}

Node Grid::NodeFromWorldPosition(const sf::Vector2f &worldPosition)
{
    int x = static_cast<int>(worldPosition.x / nodeSize.x);
    int y = static_cast<int>(worldPosition.y / nodeSize.y);

    if (x < 0 || x >= width || y < 0 || y >= height)
    {
        return Node{ -1, -1, Node::Type::Blocked, nullptr, false, 0.0f, 0.0f };
    }

    return nodes[x][y];
}

sf::Vector2f Grid::WorldPositionFromNode(Node& node)
{
    return sf::Vector2f(node.x * nodeSize.x, node.y * nodeSize.y);
}

Node **Grid::GetNodes() const
{
    return nodes;
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

Node Grid::GetNode(const Node::Index x, const Node::Index y)
{
    return nodes[x][y];
}

void Grid::Render(sf::RenderWindow &window)
{
    // for (Node::Index x = 0; x < width; ++x)
    // {
    //     for (Node::Index y = 0; y < height; ++y)
    //     {
    //         sf::RectangleShape rectangle(nodeSize);
    //         rectangle.setPosition(sf::Vector2f(x * nodeSize.x, y * nodeSize.y));

    //         if (nodes[x][y].isBlocked)
    //         {
    //             rectangle.setFillColor(sf::Color::Red);
    //         }
    //         else
    //         {
    //             rectangle.setFillColor(sf::Color::Green);
    //         }

    //         window.draw(rectangle);
    //     }
    // }
}