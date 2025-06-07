#pragma once
#include "Entity.h"
#include "Component.h"

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "Common/Debug.h"

#include <set>
#include <memory.h>
#include <typeindex>
#include <list>
#include <tuple>
#include <vector>

#include "Engine.h"

#include <iostream>

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
    template<typename T>
    void RequireComponent()
    {
        const std::type_info& type = typeid(T);

        auto componentType = GetComponentType(type);

        signature.set(componentType);
    }

    template<typename... Ts>
    [[nodiscard]] std::vector<std::type_index> ComponentTypes()
    {
        if constexpr (sizeof...(Ts) == 0)
            return std::vector<std::type_index>();
            
        return { std::type_index(typeid(Ts))... };
    }

    template<typename... Ts>
    [[nodiscard]] std::vector<std::pair<Entity, std::tuple<Ts&...>>> EntitiesWith()
    {
        // Build these when a entity gets inserted within the system 
        std::vector<std::type_index> componentTypes = ComponentTypes<Ts...>();
        Signature requirements{};
        for(std::type_index componentType : componentTypes)
            requirements.set(GetComponentType(componentType));
        
        std::vector<std::pair<Entity, std::tuple<Ts&...>>> returnValue;
        for(auto& entity : entities)
        {
            Signature signature = engine.GetEntitySignature(entity);
            if((signature & requirements) == requirements)
            {   
                // std::make_tuple makes an copy, std::tie creates an reference https://en.cppreference.com/w/cpp/utility/tuple/tie
                auto tuple = std::tie(engine.GetComponent<Ts>(entity)...);  
                returnValue.emplace_back(std::make_pair(entity, tuple));
            }
        }

        return returnValue;
    }

    Engine& engine; // Move to private 
private:
    Signature signature;
    std::set<Entity> entities;

    [[nodiscard]] ComponentType GetComponentType(std::type_index type) const;    
};