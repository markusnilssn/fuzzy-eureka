#pragma once 
#include "Engine/Entity.h"
#include <SFML/Graphics.hpp>

enum struct Resource 
{
    None,
    Wood,
    Stone,
    Gold,
};


class Node 
{
    friend class Grid;
public: 
    int X() { return x; }
    int Y() { return y; }

    const sf::Vector2f& WorldPosition() { return worldPosition; }
    // const sf::Vector2f& CenterPosition() { return centerPosition; }

    const bool IsLocked() { return blocked; }
    const Entity Owner() { return owner; }

private:
    int x;
    int y;
    sf::Vector2f worldPosition;
    // sf::Vector2f centerPosition;
    Entity owner = InvalidEntity;
    bool blocked = false;
    
};
