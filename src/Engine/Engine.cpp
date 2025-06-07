#include "Engine.h"

Engine::Engine()
    : componentManager(nullptr)
    , systemManager(nullptr)
    , entityManager(nullptr)
{
    componentManager = std::make_unique<ComponentManager>();
    systemManager = std::make_unique<SystemManager>();
    entityManager = std::make_unique<EntityManager>();
}

void Engine::Start() 
{
    systemManager->Start();
}

void Engine::Destroy() 
{
    systemManager->Destroy();
}

void Engine::Update(const float deltaTime) 
{
    systemManager->Update(deltaTime);
}

void Engine::Render(sf::RenderWindow& window) 
{
    systemManager->Render(window);
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

ComponentType Engine::GetComponentType(std::type_index type)
{
    return componentManager->GetComponentType(type);
}

Signature Engine::GetEntitySignature(Entity entity)
{
    return entityManager->GetSignature(entity);
}

void Engine::SetSystemSignature(std::type_index type, Signature signature)
{
    systemManager->SetSignature(type, signature);
}