#include "AStarSystem.h"
#include "Engine/Engine.h"
#include "Grid.h"

#include "Engine/TransformComponent.h"

#include <iostream>
#include "GameMessages.h"

AStarSystem::AStarSystem(Engine& engine, MessageQueue& messageQueue, Grid& grid)
    : System(engine)
    , messageQueue(messageQueue)
    , grid(grid)
{
    RequireComponent<TransformComponent>();
    RequireComponent<ObjectComponent>();
    RequireComponent<NavigationComponent>();
}

void AStarSystem::Start()
{
    messageQueue.Subscribe<MoveEntity>([this](const MoveEntity& data)
    {
        // redo with improved interface 
        auto& navigation = engine.GetComponent<NavigationComponent>(data.entity);
        auto& transform = engine.GetComponent<TransformComponent>(data.entity);

        Node* mainNode = grid.NodeFromWorldPosition(transform.position);

        int xSize = transform.size.x / grid.GetNodeSize().x;
        int ySize = transform.size.y / grid.GetNodeSize().y;

        navigation.path = FindPath(mainNode, data.node, data.entity, sf::Vector2i{xSize, ySize});   

        std::cout << "x: " << data.node->X() << " y: " << data.node->Y() << " Entity " << data.entity << std::endl;
    });
}

void AStarSystem::Destroy()
{

}

void AStarSystem::Update(float deltaTime) 
{
    for (auto& [entity, registry] : EntitiesWith<TransformComponent, ObjectComponent>())
    {
        auto& transform  = std::get<0>(registry);
        auto& object = std::get<1>(registry);

        sf::FloatRect rect{ transform.position, transform.size };

        std::set<Node*> currentNodes = grid.NodesUnderRectangle(rect);

        if (!currentNodes.empty())
        {
            for (Node* n : currentNodes)
                grid.Lock(n, entity);

            for (Node* old : object.lastNodes)
            {
                if (old != nullptr && currentNodes.find(old) == currentNodes.end())
                {
                    grid.Unlock(old);
                }
            }
        }

        object.lastNodes = std::move(currentNodes);
    }


    for (auto& [entity, registry] : EntitiesWith<TransformComponent, NavigationComponent>())
    {
        auto& transform  = std::get<0>(registry);
        auto& navigation = std::get<1>(registry);


        if (!navigation.path.empty())
        {
            Node* nodeToGoTo = navigation.path.front();
            sf::Vector2f destination = grid.WorldPositionFromNode(nodeToGoTo);

            sf::Vector2f difference = (destination - transform.position);
            sf::Vector2f toMove = difference.normalized();
            transform.position += toMove * moveSpeed * deltaTime;
            if(difference.lengthSquared() < 0.1f)
            {
                transform.position = destination;
                navigation.path.pop_front();
            }

            // navigation.moveTick += deltaTime;
            // if (transform.position != destination && navigation.moveTick > 0.3f)
            // {
            //     transform.position = destination;
            //     navigation.path.pop_front();
            //     navigation.moveTick = 0.0f;
            // }
        }
    }
}

const bool AStarSystem::IsWalkable(Node *node, Entity entity, const sf::Vector2i &sizeInNodes)
{
    int baseX = node->X();
    int baseY = node->Y();

    for (int dx = 0; dx < sizeInNodes.x; ++dx)
    {
        for (int dy = 0; dy < sizeInNodes.y; ++dy)
        {
            int x = baseX + dx;
            int y = baseY + dy;

            Node* check = grid.GetNodeAt(x, y);
            if (check != nullptr && check->IsLocked() && check->Owner() != entity)
            {
                return false;
            }
        }
    }
    
    return true;
}

std::list<Node *> AStarSystem::FindPath(Node *startNode, Node *endNode, Entity entity, const sf::Vector2i &sizeInNodes)
{
    if (endNode == nullptr || startNode == nullptr)
    {
        std::cout << "Unable to walk from or to node" << std::endl;
        return std::list<Node*>();
    }

    std::list<Node*> openList;
    std::list<Node*> closedList;
    std::unordered_map<Node*, Weight> weights;

    if (!IsWalkable(startNode, entity, sizeInNodes) || !IsWalkable(endNode, entity, sizeInNodes))
    {
        std::cout << "Unable to walk from or to node" << std::endl;
        return std::list<Node*>();
    }

    openList.push_back(startNode);
    weights[startNode] = Weight {
        .parent = nullptr,
        .gCost  = 0,
        .hCost  = GetDistanceBetweenNodes(startNode, endNode)
    };

    std::list<Node*> path;

    while (!openList.empty())
    {
        Node* current = FindNodeWithLowestFCost(openList, weights);
        openList.remove(current);
        closedList.push_back(current);

        if (current == endNode)
        {
            Node* retrace = endNode;
            while (retrace != startNode)
            {
                path.push_back(retrace);
                retrace = weights[retrace].parent;
            }

            std::reverse(path.begin(), path.end());
            break;
        }

        std::list<Node*> neighbors = grid.FindNeighbors(current);
        for (Node* node : neighbors)
        {
            bool containsInClosed = (std::find(closedList.begin(), closedList.end(), node) != closedList.end());
            if (containsInClosed) 
            {
                continue;
            }

            if (!IsWalkable(node, entity, sizeInNodes)) 
            {
                continue;
            }

            int distanceBetweenNodes = GetDistanceBetweenNodes(current, node);
            int tentativeG = weights[current].gCost + distanceBetweenNodes;

            auto [it, inserted] = weights.try_emplace(node, Weight{
                .parent = nullptr,
                .gCost  = std::numeric_limits<int>::max(),
                .hCost  = 0
            });
            Weight& weight = it->second;
            weight.hCost = GetDistanceBetweenNodes(node, endNode);

            if (tentativeG < weight.gCost)
            {
                weight.gCost = tentativeG;
                weight.parent = current;

                bool containsInOpen = (std::find(openList.begin(), openList.end(), node) != openList.end());
                if (!containsInOpen)
                    openList.push_back(node);
            }
        }
    }

    if (path.empty())
    {
        std::cout << "No path from (" << startNode->X() << "," << startNode->Y() << ") to (" << endNode->X() << "," << endNode->Y() << ").\n";
    }

    return path;
}

Node *AStarSystem::FindNodeWithLowestFCost(const std::list<Node *>& nodes, std::unordered_map<Node*, Weight>& weights)
{
    if (nodes.size() == 1)
    {
        return nodes.front();
    }

    Node* lowest = nodes.front();

    for (auto node : nodes)
    {
        if (weights[node].fCost() < weights[lowest].fCost())
        {
            lowest = node;
        }
        
        if (weights[node].fCost() == weights[lowest].fCost())
        {
            lowest = weights[node].hCost < weights[lowest].hCost ? node : lowest;
        }
    }

    return lowest;
}

int AStarSystem::GetDistanceBetweenNodes(Node *first, Node *second)
{
    int distX = std::abs(first->X() - second->X());
    int distY = std::abs(first->Y() - second->Y());

    if (distX > distY)
        return 14 * distY + 10 * (distX - distY);

    return 14 * distX + 10 * (distY - distX);
}
