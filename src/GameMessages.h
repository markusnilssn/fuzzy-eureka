#pragma once
#include <SFML/Graphics.hpp>

#include "Engine/Entity.h"
#include "Engine/MessageQueue.h"
#include "Node.h"

struct MoveEntity : Message 
{
    Node* node;
    Entity entity;

    MoveEntity(Node* node, Entity entity) 
        : node(node)
        , entity(entity)
    {
        
    }
};