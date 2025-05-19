#include "SystemManager.h"

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