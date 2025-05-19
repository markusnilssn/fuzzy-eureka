#pragma once
#include <stdint.h>
#include <queue>
#include <array>
#include "Signature.h"

using Entity = uint32_t;

constexpr Entity MAX_ENTITIES = 1000;

class EntityManager {
public:
    EntityManager();

    Entity Instantiate();
    void Destroy(Entity entity);

    void SetSignature(Entity entity, Signature signature);
	Signature GetSignature(Entity entity);
private:
    std::queue<Entity> availableEntities;
    std::array<Signature, MAX_ENTITIES> signatures;
    Entity livingEntityCount;

};