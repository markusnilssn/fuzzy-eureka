#include "System.h"
#include "Common/Debug.h"
#include "Engine.h"


System::System(Engine& engine)
    : engine(engine)
{
    // Register the system with the engine
    // engine.SetSystemRegistry(typeid(*this), registry);
    // engine.SetSystemRegistry<>(registry);
}

System::~System() = default;

const std::set<Entity> &System::GetEntities() const
{
    return entities;
}

void System::RemoveEntity(Entity entity)
{
    if (entities.find(entity) == entities.end())
    {
        Debug::Assert(false, "Entity not found in system");
        return;
    }

    entities.erase(entity);
    OnEntityRemoved(entity);
}

void System::InsertEntity(Entity entity)
{
    entities.insert(entity);
    OnEntityAdded(entity);
}

ComponentType System::GetComponentType(const std::type_info &type) const
{
    return engine.GetComponentType(type);
}
