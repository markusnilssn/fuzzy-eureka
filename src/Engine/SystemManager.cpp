#include "SystemManager.h"

SystemManager::SystemManager()
    : registries()
    , systems()
{

}

void SystemManager::Start() 
{

}
void SystemManager::Destroy()
{

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

void SystemManager::SetRegistry(const std::type_info& type, Registry registry)
{
    const char* typeName = type.name();

	Debug::Assert(systems.find(typeName) != systems.end(), "System used before registered.");

	registries.insert({typeName, registry});
}

void SystemManager::EntityDestroyed(Entity entity)
{
	for (auto& [type, system] : systems)
    {
        system->RemoveEntity(entity);
    }
}

void SystemManager::EntitySignatureChanged(Entity entity, Registry entityRegistry) 
{
    for (auto& [type, system] : systems)
    {
        auto systemRegistry = registries[type];

        if ((entityRegistry & systemRegistry) == systemRegistry)
        {
            system->InsertEntity(entity);
        }
        else
        {
            system->RemoveEntity(entity);
        }
    }
}