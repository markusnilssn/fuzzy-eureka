#pragma once
#include "Entity.h"
#include "Component.h"

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "Common/Debug.h"

#include <set>

class Engine;
class System
{
public:
    explicit System(Engine& engine);
    virtual ~System();

    virtual void Start() {}
    virtual void Destroy() {}
    
    virtual void Update(float deltaTime) {}
    virtual void LateUpdate(float deltaTime) {}
    
    virtual void Render(sf::RenderWindow& window) {}
    virtual void HandleEvent(const sf::Event& event) {}

    virtual void OnEntityAdded(Entity entity) {}
    virtual void OnEntityRemoved(Entity entity) {}

    [[nodiscard]] const std::set<Entity>& GetEntities() const;

    void RemoveEntity(Entity entity);
    void InsertEntity(Entity entity);

protected:
    Engine& engine;
    std::set<Entity> entities;

    template<typename T>
    void RequireComponent()
    {
        const std::type_info& type = typeid(T);

        auto componentType = GetComponentType(type);

        registry.set(componentType);
    }

private:
    Registry registry;

    [[nodiscard]] ComponentType GetComponentType(const std::type_info& type) const;    
};