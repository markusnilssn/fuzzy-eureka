#pragma once
#include <unordered_map>
#include <memory>

#include "ComponentManager.h"
#include "SystemManager.h"  
#include "EntityManager.h"

class Engine {
public:
    Engine();

    void Start();
    void Destroy();

    [[nodiscard]] Entity CreateEntity();
    void DestroyEntity(Entity entity);

    template<typename T>
    void RegisterComponent();
    template<typename T>
    void AddComponent(Entity entity, T component);

    template<typename T>
    void RemoveComponent(Entity entity);
    template<typename T>
    [[nodiscard]] T& GetComponent(Entity entity);
    template<typename T>
	[[nodiscard]] ComponentType GetComponentType(); 

    template<typename T>
    std::shared_ptr<T> RegisterSystem();

    template<typename T>
    void SetSystemSignature(Signature signature);

private:
    std::unique_ptr<ComponentManager> componentManager;
    std::unique_ptr<SystemManager> systemManager;
    std::unique_ptr<EntityManager> entityManager;

};

template<typename T>
inline void Engine::RegisterComponent()
{
    componentManager->RegisterComponent<T>();
}

template<typename T>
inline void Engine::AddComponent(Entity entity, T component)
{
    componentManager->AddComponent<T>(entity, component);

    auto signature = entityManager->GetSignature(entity);
    signature.set(componentManager->GetComponentType<T>(), true);
    entityManager->SetSignature(entity, signature);

    systemManager->EntitySignatureChanged(entity, signature);
}

template<typename T>
inline void Engine::RemoveComponent(Entity entity)
{
    componentManager->RemoveComponent<T>(entity);

    auto signature = entityManager->GetSignature(entity);
    signature.set(componentManager->GetComponentType<T>(), false);
    entityManager->SetSignature(entity, signature);

    systemManager->EntitySignatureChanged(entity, signature);
}

template<typename T>
inline T& Engine::GetComponent(Entity entity)
{
    return componentManager->GetComponent<T>(entity);
}

template<typename T>
inline ComponentType Engine::GetComponentType()
{
    return componentManager->GetComponentType<T>();
}

template<typename T>
inline std::shared_ptr<T> Engine::RegisterSystem()
{
    return systemManager->RegisterSystem<T>();
}

template<typename T>
inline void Engine::SetSystemSignature(Signature signature)
{
    systemManager->SetSignature<T>(signature);
}