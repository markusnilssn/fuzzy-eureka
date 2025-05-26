#include "ComponentManager.h"

ComponentManager::ComponentManager()
    : componentTypes(),
      componentArrays(),
      nextComponentType(0)
{

}

void ComponentManager::EntityDestroyed(Entity entity)
{
    for (auto const& [type, componentArray] : componentArrays)
    {
        componentArray->EntityDestroyed(entity);
    }
}

ComponentType ComponentManager::GetComponentType(const std::type_info& type)
{
	const char* typeName = type.name();

	Debug::Assert(componentTypes.find(typeName) != componentTypes.end(), "Component not registered before use.");

	return componentTypes[typeName];
}
