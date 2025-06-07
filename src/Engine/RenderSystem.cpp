#include "RenderSystem.h"
#include "Engine.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include <map>
#include <list>
#include <math.h>
#include "stl.h"

RenderSystem::RenderSystem(Engine& engine)
    : System(engine)
{
    RequireComponent<TransformComponent>();
    RequireComponent<SpriteComponent>();
    RequireComponent<AnimatorComponent>();
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

void RenderSystem::Update(const float deltaTime)
{   
    // a bit weird to update the deltaTime here, cannot be separated with a thread if this is kept
    for(auto& [entity, registry] : EntitiesWith<AnimatorComponent>())
    {
        auto& animator = std::get<0>(registry);

        if(animator.animation.size() == 0)
        {
            continue;
        }

        auto sequences = animator.spriteSheet.GetSequence(animator.animation);
        animator.tick += deltaTime;
        if(animator.tick >= animator.speed) 
        {
            animator.currentFrame = std::wrap<size_t>(++animator.currentFrame, 0, sequences.size());
            animator.tick = 0.0f;
        }
    }
}

void RenderSystem::Render(sf::RenderWindow& window)
{
    // don't know if map order itself?
    std::map<uint_fast16_t, std::vector<sf::Sprite>> collection{};
    for(auto& [entity, registry] : EntitiesWith<TransformComponent, SpriteComponent>())
    {
        auto& transform = std::get<0>(registry);
        auto& sprite = std::get<1>(registry);
        
        sf::Sprite drawable(sprite.texture);
        drawable.setPosition(transform.position);
        drawable.setColor(sprite.color);
        
        collection[sprite.sortLayer].push_back(drawable);
    }

    for(auto& [entity, registry] : EntitiesWith<TransformComponent, AnimatorComponent>())
    {
        auto& transform = std::get<0>(registry);
        auto& animator = std::get<1>(registry);

        sf::Sprite sprite(animator.spriteSheet.texture);
        sprite.setPosition(transform.position);

        auto sequences = animator.spriteSheet.GetSequence(animator.animation);
        sprite.setTextureRect(sequences[animator.currentFrame]);
    
        window.draw(sprite);
        
    }

    for(auto& [layer, drawables] : collection)
    {
        for(auto& drawable : drawables)
            window.draw(drawable);
    }
}