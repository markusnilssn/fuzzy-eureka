#include "SystemManager.h"
#include "System.h"

SystemManager::SystemManager()
    : signatures()
    , systems()
{

}

void SystemManager::Start() 
{
    for (auto& [type, system] : systems)
    {
        system->Start();
    }
}
void SystemManager::Destroy()
{
    for (auto& [type, system] : systems)
    {
        system->Destroy();
    }
}

void SystemManager::Update(float deltaTime)
{
    for (auto& [type, system] : systems)
    {
        system->Update(deltaTime);
    }
}
void SystemManager::Render(sf::RenderWindow& window)
{
    for (auto& [type, system] : systems)
    {
        system->Render(window);
    }
}

void SystemManager::SetSignature(std::type_index type, Signature signature)
{
	Debug::Assert(systems.find(type) != systems.end(), "System used before registered.");

	signatures.insert({type, signature});
}

void SystemManager::EntityDestroyed(Entity entity)
{
	for (auto& [type, system] : systems)
    {
        system->RemoveEntity(entity);
    }
}

void SystemManager::EntitySignatureChanged(Entity entity, Signature entitySignature) 
{
    for (auto& [type, system] : systems)
    {
        auto systemSignature = signatures[type];

        if ((entitySignature & systemSignature) == systemSignature)
        {
            system->InsertEntity(entity);
        }
        else
        {
            system->RemoveEntity(entity);
        }
    }
}