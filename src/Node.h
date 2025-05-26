#pragma once 
enum struct Resource 
{
    None,
    Wood,
    Stone,
    Gold,
};


struct Node 
{
    using Index = int;

    enum struct Type
    {
        Start,
        End,
        Blocked,
        Walkable,
    };

    Index x;
    Index y;

    Type type;
    Node* parent;
    bool isBlocked;

    float gCost;
    float hCost;
    float fCost() const { return gCost + hCost; }
};

struct Terrain 
{
    enum struct Type
    {
        Sand,
        Grass,
        Water,
        Rock,
    };

    float elevation;
    Type type;
    Node* node;
};
