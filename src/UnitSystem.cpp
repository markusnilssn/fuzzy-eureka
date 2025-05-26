#include "UnitSystem.h"
#include "Engine/Engine.h"

UnitSystem::UnitSystem(Engine& engine)
    : System(engine)
{

}

void UnitSystem::Start()
{

}

void UnitSystem::Destroy()
{

}

void UnitSystem::Update(float deltaTime)
{
    for (auto& entity : entities)
    {

    }
}

void UnitSystem::MoveUnit(Entity entity, const sf::Vector2f& move)
{

}