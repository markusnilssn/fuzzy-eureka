#include "RenderSystem.h"
#include "Engine.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include <map>
#include <list>

RenderSystem::RenderSystem(Engine& engine)
    : System(engine)
{
    RequireComponent<TransformComponent>();
    RequireComponent<SpriteComponent>();
}

void RenderSystem::Start()
{

}

void RenderSystem::Destroy()
{   

}

bool SortByLayer(const std::pair<uint_fast16_t, sf::Sprite>& a
    , const std::pair<uint_fast16_t, sf::Sprite>& b)
{
    return a.first > b.first;
}

void RenderSystem::Render(sf::RenderWindow& window)
{
    // don't know if map order itself?
    std::map<uint_fast16_t, std::list<sf::Sprite>> collection;
    // Debug::Log(std::to_string(entities.size()));
    for(auto& entity : entities)
    {
        auto& sprite = engine.GetComponent<SpriteComponent>(entity);
        auto& transform = engine.GetComponent<TransformComponent>(entity);
        
        sf::Sprite drawable(sprite.texture);
        drawable.setPosition(transform.position);
        
        collection[sprite.sortLayer].push_back(drawable);
    }

    for(auto& [layer, drawables] : collection)
    {
        for(auto& drawable : drawables)
            window.draw(drawable);
    }
}