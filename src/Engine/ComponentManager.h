#pragma once
#include "Component.h"
#include "ComponentArray.h"

#include "Common/Debug.h"	

#include <unordered_map>
#include <memory>


class ComponentManager {
public:
	template<typename T>
	void RegisterComponent();

    template<typename T>
    [[nodiscard]] ComponentType GetComponentType();

    template<typename T>
    void AddComponent(Entity entity, T component);
    template<typename T>
    void RemoveComponent(Entity entity);
    template<typename T>
    [[nodiscard]] T& GetComponent(Entity entity);

    void EntityDestroyed(Entity entity);

private:
	std::unordered_map<const char*, ComponentType> componentTypes;
	std::unordered_map<const char*, std::shared_ptr<IComponentArray>> componentArrays;
	ComponentType nextComponentType;

	template<typename T>
	std::shared_ptr<ComponentArray<T>> GetComponentArray();
};

template <typename T>
inline void ComponentManager::RegisterComponent()
{
	const char* typeName = typeid(T).name();

	Debug::Assert(nextComponentType < MAX_COMPONENTS, "Too many component types registered.");
    Debug::Assert(componentTypes.find(typeName) == componentTypes.end(), "Registering component type more than once.");

    componentTypes[typeName] = nextComponentType;
	componentArrays[typeName] = std::make_shared<ComponentArray<T>>();
	++nextComponentType;
}

template <typename T>
inline ComponentType ComponentManager::GetComponentType()
{
	const char* typeName = typeid(T).name();

	Debug::Assert(componentTypes.find(typeName) != componentTypes.end(), "Component not registered before use.");

	return componentTypes[typeName];
}

template <typename T>
inline void ComponentManager::AddComponent(Entity entity, T component)
{
	Debug::Assert(componentTypes.find(typeid(T).name()) != componentTypes.end(), "Component not registered before use.");
	
	auto componentArray = GetComponentArray<T>();

	componentArray->InsertData(entity, component);
}

template <typename T>
inline void ComponentManager::RemoveComponent(Entity entity)
{
	Debug::Assert(componentTypes.find(typeid(T).name()) != componentTypes.end(), "Component not registered before use.");

	auto componentArray = GetComponentArray<T>();

	componentArray->RemoveData(entity);
}

template<typename T>
inline T& ComponentManager::GetComponent(Entity entity)
{
	Debug::Assert(componentTypes.find(typeid(T).name()) != componentTypes.end(), "Component not registered before use.");

	auto componentArray = GetComponentArray<T>();

	return componentArray->GetData(entity);
}

template <typename T>
inline std::shared_ptr<ComponentArray<T>> ComponentManager::GetComponentArray()
{	
	const char* typeName = typeid(T).name();

	Debug::Assert(componentTypes.find(typeName) != componentTypes.end(), "Component not registered before use.");
	
	return std::static_pointer_cast<ComponentArray<T>>(componentArrays[typeName]);
}
