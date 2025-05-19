#include "System.h"

System::System(Engine* engine)
    : engine(engine)
{

}

void System::RemoveEntity(Entity entity)
{
    entities.erase(entity);
}

void System::InsertEntity(Entity entity)
{
    entities.insert(entity);
}
