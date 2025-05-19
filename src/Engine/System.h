#pragma once
#include "Entity.h"
#include "SFML/Window.hpp"  
#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"

#include <set>

class Engine;

class System
{
public:
    System(Engine* engine);

    virtual void Start() {}
    virtual void Destroy() {}
    
    virtual void Update(float deltaTime) {}
    virtual void LateUpdate(float deltaTime) {}
    
    virtual void Render(sf::RenderWindow& window) {}
    virtual void HandleEvent(const sf::Event& event) {}

    void RemoveEntity(Entity entity);
    void InsertEntity(Entity entity);

protected:
    std::set<Entity> entities;
    Engine* engine;

    template<typename T>
    void RequireComponent();

};