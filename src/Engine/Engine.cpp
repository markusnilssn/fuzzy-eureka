#include "Engine.h"

Engine::Engine()
    : componentManager(nullptr)
    , systemManager(nullptr)
    , entityManager(nullptr)
{

}

void Engine::Start() 
{
    componentManager = std::make_unique<ComponentManager>();
    systemManager = std::make_unique<SystemManager>();
    entityManager = std::make_unique<EntityManager>();
}

void Engine::Update(const float deltaTime) 
{
    systemManager->Update(deltaTime);
}

void Engine::Render(sf::RenderWindow& window) 
{
    systemManager->Render(window);
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

ComponentType Engine::GetComponentType(const std::type_info& type)
{
    return componentManager->GetComponentType(type);
}

void Engine::SetSystemRegistry(const std::type_info &type, Registry signature)
{
    systemManager->SetRegistry(type, signature);
}