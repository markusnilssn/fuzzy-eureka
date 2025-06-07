#pragma once
#include <unordered_map>
#include <memory>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "ComponentManager.h"
#include "SystemManager.h"  
#include "EntityManager.h"

#include <typeinfo>
#include <typeinfo>

class Engine final {
public:
    explicit Engine();

    void Start();
    void Destroy();

    void Update(const float deltaTime);
    void Render(sf::RenderWindow& window);

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

	[[nodiscard]] ComponentType GetComponentType(std::type_index type); 

    template<typename T, typename... Args>
    std::shared_ptr<T> RegisterSystem(Args&&... args);

    Signature GetEntitySignature(Entity entity);

    void SetSystemSignature(std::type_index type, Signature signature);

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
    std::type_index type(typeid(T));
    signature.set(componentManager->GetComponentType(type), true);
    entityManager->SetSignature(entity, signature);

    systemManager->EntitySignatureChanged(entity, signature);
}

template<typename T>
inline void Engine::RemoveComponent(Entity entity)
{
    componentManager->RemoveComponent<T>(entity);

    auto signature = entityManager->GetSignature(entity);
    std::type_index type(typeid(T));
    signature.set(componentManager->GetComponentType(type), false);
    entityManager->SetSignature(entity, signature);

    systemManager->EntitySignatureChanged(entity, signature);
}

template<typename T>
inline T& Engine::GetComponent(Entity entity)
{
    return componentManager->GetComponent<T>(entity);
}

template<typename T, typename... Args>
inline std::shared_ptr<T> Engine::RegisterSystem(Args&&... args)
{
    static_assert(std::is_base_of<System, T>::value, "T must be derived from System");

    return systemManager->RegisterSystem<T>(*this, std::forward<Args>(args)...);
}
