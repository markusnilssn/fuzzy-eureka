#pragma once 
#include <queue>
#include <array>
#include "Component.h"
#include "Entity.h"

class EntityManager final {
public:
    explicit EntityManager();

    [[nodiscard]] Entity CreateEntity();
    void DestroyEntity(Entity entity);

    void SetRegistry(Entity entity, Registry signature);
	[[nodiscard]] Registry GetRegistry(Entity entity);
    
private:
    std::queue<Entity> availableEntities;
    std::array<Registry, MAX_ENTITIES> registries;
    Entity livingEntityCount;

};