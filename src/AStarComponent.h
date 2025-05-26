#include "AStarSystem.h"
#include "Engine/Engine.h"

AStarSystem::AStarSystem(Engine& engine)
    : System(engine)
{
    // RequireComponent<AStarComponent>();
}

void AStarSystem::Start()
{

}

void AStarSystem::Destroy()
{

}

void AStarSystem::Update(float deltaTime) { }
void AStarSystem::Render(sf::RenderWindow& window) { }