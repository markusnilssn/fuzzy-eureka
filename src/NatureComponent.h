#pragma once

enum struct Nature
{
    Gold,
    Wood,
    Stone, 
    Farm,
    Barn,
};

struct DestructComponent 
{
    int health{};
};

struct NatureComponent
{
    Nature type;
    int amount{};
};