#include "EntityManager.h"
#include "Common/Debug.h"

EntityManager::EntityManager() 
    : livingEntityCount(0)
{
    for (Entity entity = 0; entity < MAX_ENTITIES; ++entity)
    {
        availableEntities.push(entity);
    }
}

Entity EntityManager::CreateEntity()
{
    Debug::Assert(livingEntityCount < MAX_ENTITIES, "Too many entities in existence.");

    // Take an ID from the front of the queue
    Entity id = availableEntities.front();
    availableEntities.pop();
    ++livingEntityCount;

    return id;
}

void EntityManager::DestroyEntity(Entity entity)
{
    Debug::Assert(entity < MAX_ENTITIES, "Entity out of range.");
    Debug::Assert(livingEntityCount > 0, "Destroying entity when none exist.");

    signatures[entity].reset();

    availableEntities.push(entity);
    --livingEntityCount;
}

void EntityManager::SetSignature(Entity entity, Signature signature)
{
    Debug::Assert(entity < MAX_ENTITIES, "Entity out of range.");
    Debug::Assert(livingEntityCount > 0, "Setting signature on entity when none exist.");

    signatures[entity] = signature;
}

Signature EntityManager::GetSignature(Entity entity)
{
    Debug::Assert(entity < MAX_ENTITIES, "Entity out of range.");
    Debug::Assert(livingEntityCount > 0, "Getting signature on entity when none exist.");

    return signatures[entity];
}
