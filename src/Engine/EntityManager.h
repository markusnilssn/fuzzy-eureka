#pragma once 
#include <queue>
#include <array>

#include "Signature.h"
#include "Entity.h"

class EntityManager final {
public:
    explicit EntityManager();

    [[nodiscard]] Entity CreateEntity();
    void DestroyEntity(Entity entity);

    void SetSignature(Entity entity, Signature signature);
	[[nodiscard]] Signature GetSignature(Entity entity);
    
private:
    std::queue<Entity> availableEntities;
    std::array<Signature, MAX_ENTITIES> signatures;
    Entity livingEntityCount;

};