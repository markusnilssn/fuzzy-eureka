#include "Engine.h"

Engine::Engine()
    :  componentManager(std::make_unique<ComponentManager>())
    ,  systemManager(std::make_unique<SystemManager>())
    ,  entityManager(std::make_unique<EntityManager>())
{

}

void Engine::Start() 
{

}

void Engine::Destroy() 
{

}

Entity Engine::CreateEntity() 
{
    return entityManager->CreateEntity();
}

void Engine::DestroyEntity(Entity entity) 
{
    entityManager->DestroyEntity(entity);
    componentManager->EntityDestroyed(entity);
    systemManager->EntityDestroyed(entity);
}