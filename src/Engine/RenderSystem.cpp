#include "RenderSystem.h"
#include "Engine.h"
#include "Components.h"

RenderSystem::RenderSystem(Engine* engine)
    : System(engine)
 
{
    // RequireComponent<Sprite>();
    // RequireComponent<Transform>();
}

void RenderSystem::Start()
{

}

void RenderSystem::Destroy()
{   

}

void RenderSystem::Render(sf::RenderWindow& window)
{
    for(auto& entity : entities)
    {
        auto& sprite = engine->GetComponent<SpriteComponent>(entity);
        auto& transform = engine->GetComponent<TransformComponent>(entity);

        sprite.sprite.setPosition(transform.position);
        sprite.sprite.setRotation(transform.rotation);
        sprite.sprite.setTexture(sprite.texture);
        sprite.sprite.setTextureRect(sprite.bound);
    }
}