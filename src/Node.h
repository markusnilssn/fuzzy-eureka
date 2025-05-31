#pragma once 
#include "Engine/Entity.h"

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

    const bool IsLocked() { return blocked; }
    const Entity Owner() { return owner; }

private:
    int x;
    int y;
    Entity owner = InvalidEntity;
    bool blocked = false;
    
};
