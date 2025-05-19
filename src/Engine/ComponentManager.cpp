#include "ComponentManager.h"

void ComponentManager::EntityDestroyed(Entity entity) 
{
    for (auto const& [type, componentArray] : componentArrays)
    {
        componentArray->EntityDestroyed(entity);
    }
}