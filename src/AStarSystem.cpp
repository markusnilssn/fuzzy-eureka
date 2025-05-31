#include "AStarSystem.h"
#include "Engine/Engine.h"
#include "Grid.h"

#include "Engine/TransformComponent.h"

#include <iostream>

AStarSystem::AStarSystem(Engine& engine, std::shared_ptr<Grid> grid)
    : System(engine)
    , grid(grid)
{
    RequireComponent<TransformComponent>();
    RequireComponent<AStarComponent>();
}

void AStarSystem::Start()
{
    // commandManager.RegisterListener()

}

void AStarSystem::Destroy()
{

}

void AStarSystem::Update(float deltaTime) 
{
    for(auto& entity : entities)
    {
        auto& transform = engine.GetComponent<TransformComponent>(entity);
        auto& navigation = engine.GetComponent<AStarComponent>(entity);
        
        Node* mainNode = grid->NodeFromWorldPosition(transform.position);
        if(mainNode != nullptr) 
        {
            grid->Lock(mainNode, entity);

            if(navigation.lastNode != nullptr && navigation.lastNode != mainNode)
            {
                grid->Unlock(navigation.lastNode);
            }
        }

        navigation.lastNode = mainNode;
        
        if(navigation.path.size() > 0)
        {
            // navigation.isMoving = true;
            Node* nodeToGoTo = navigation.path.front();

            // if(nodeToGoTo->IsLocked())
            // {
            //     if(nodeToGoTo == navigation.endNode)
            //     {
            //         navigation.path.clear();
            //     }
            //     else 
            //     {
            //         navigation.path = FindPath(mainNode, navigation.endNode); // recalc
            //     }
            // }
            
            const auto& worldPosition = grid->WorldPositionFromNode(nodeToGoTo);
            if(transform.position != worldPosition && navigation.moveTick > 0.3f)
            {
                transform.position = worldPosition;
                navigation.moveTick = 0.0f;

                navigation.path.pop_front();
            }
        }

        navigation.moveTick += deltaTime;
    }
}
void AStarSystem::Render(sf::RenderWindow& window) { }

void AStarSystem::GoTo(Entity entity, Node *node)
{
    auto& navigation = engine.GetComponent<AStarComponent>(entity);
    auto& transform = engine.GetComponent<TransformComponent>(entity);
    Node* mainNode = grid->NodeFromWorldPosition(transform.position);
    auto path = FindPath(mainNode, node);
    std::cout << "Path Size " << path.size() << std::endl;

    navigation.path = path;    
}

std::list<Node *> AStarSystem::FindPath(Node *startNode, Node *endNode)
{
    if(endNode == nullptr)
    {
        return std::list<Node*>();
    }

    std::list<Node*> openNodes;
    std::list<Node*> closedNodes;
    
    std::unordered_map<Node*, Weight> weights{};
    
    // Redirect to closest node
    // if(endNode->IsLocked()) 
    //     return std::list<Node*>();
    
    openNodes.push_back(startNode);
    weights[startNode] = Weight{ /* parent=*/ nullptr,
                             /* gCost=*/ 0,
                             /* hCost=*/ GetDistanceBetweenNodes(startNode, endNode) };
    
    std::list<Node*> path;
    Node* currentNode = nullptr;
    while(openNodes.size() > 0)
    {
        currentNode = FindNodeWithLowestFCost(openNodes, weights);
        // std::cout << currentNode->X() << " " << currentNode->Y() << std::endl;
        openNodes.remove(currentNode);
        closedNodes.push_back(currentNode);

        if(currentNode == endNode)
        {
            Node* currentRetracedNode = endNode;
            while(currentRetracedNode != startNode)
            {
                path.push_back(currentRetracedNode);
                auto [it, inserted] = weights.try_emplace(currentRetracedNode, Weight{

                });
                auto& weight = (*it).second;
                currentRetracedNode = weight.parent;
            }

            path.reverse();
            // openNodes.clear();
            // closedNodes.clear();

            // std::cout << "reversing list" << std::endl;
            break;
        }

        std::list<Node*> neighbors = grid->FindNeighbors(currentNode);
        // std::cout << "neighbour" << neighbors.size() << std::endl;
        for(auto node : neighbors)
        {
            auto containsInClosed = std::find(closedNodes.begin(), closedNodes.end(), node) != closedNodes.end();
            if(node->IsLocked() || containsInClosed) 
            {
                std::cout << "Locked " << node->X() << " " <<node->Y() << std::endl;
                continue;
            }

            int stepCost = GetDistanceBetweenNodes(currentNode, node);
            int currentG = (weights.count(currentNode) ? weights[currentNode].gCost : 0);
            int tentativeG = currentG + stepCost;

            auto [it, wasInserted] = weights.try_emplace(node, Weight{nullptr,
                std::numeric_limits<int>::max(),  // initial gCost = “infinite”
                0});
            auto& neighborWeight = it->second;
            neighborWeight.hCost = GetDistanceBetweenNodes(node, endNode);

            if (tentativeG < neighborWeight.gCost)
            {
                neighborWeight.gCost = tentativeG;
                neighborWeight.parent = currentNode;

                bool containsInOpen = (std::find(openNodes.begin(), openNodes.end(), node) != openNodes.end());
                if (!containsInOpen)
                    openNodes.push_back(node);
                // else: it was already in open, so no need to push again
            }
        }
    }

    if(path.size() <= 0)
    {
        std::cout << "Start Node " << startNode->X() << " " << startNode->Y() << std::endl;
        std::cout << "Start Node " << endNode->X() << " " << endNode->Y() << std::endl;
    }
    
    // std::cout << "Sizeof " << path.size() << std::endl;
    return path;
}

Node *AStarSystem::FindNodeWithLowestFCost(const std::list<Node *>& nodes, std::unordered_map<Node*, Weight>& weights)
{
    if (nodes.size() == 1)
        return nodes.front();

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
