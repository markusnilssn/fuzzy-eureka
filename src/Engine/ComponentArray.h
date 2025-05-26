#pragma once 
#include "Entity.h"
#include "Component.h"

#include "Common/Debug.h"

#include <array>
#include <unordered_map>

class IComponentArray {
public:
	virtual ~IComponentArray() = default;
	virtual void EntityDestroyed(Entity entity) = 0;
};

template<typename T>
class ComponentArray : public IComponentArray {
public: 
    void InsertData(Entity entity, T component);
    void RemoveData(Entity entity);
    
    [[nodiscard]] T& GetData(Entity entity);
    
    void EntityDestroyed(Entity entity) override;

private: 
    std::array<T, MAX_ENTITIES> componentArray;
    std::unordered_map<Entity, size_t> entityToIndex;
    std::unordered_map<size_t, Entity> indexToEntity;
    size_t size;

};

template <typename T>
inline void ComponentArray<T>::InsertData(Entity entity, T component)
{
    Debug::Assert(entityToIndex.find(entity) == entityToIndex.end(), "Component added to same entity more than once.");

    // Put new entry at end and update the maps
    size_t newIndex = size;
    entityToIndex[entity] = newIndex;
    indexToEntity[newIndex] = entity;
    componentArray[newIndex] = component;
    ++size;
}

template <typename T>
inline void ComponentArray<T>::RemoveData(Entity entity)
{
    Debug::Assert(entityToIndex.find(entity) != entityToIndex.end(), "Removing non-existent component.");

    size_t indexOfRemovedEntity = entityToIndex[entity];
    size_t indexOfLastElement = size - 1;
    componentArray[indexOfRemovedEntity] = componentArray[indexOfLastElement];

    Entity entityOfLastElement = indexToEntity[indexOfLastElement];
    entityToIndex[entityOfLastElement] = indexOfRemovedEntity;
    indexToEntity[indexOfRemovedEntity] = entityOfLastElement;

    entityToIndex.erase(entity);
    indexToEntity.erase(indexOfLastElement);

    --size;
}

template <typename T>
inline T &ComponentArray<T>::GetData(Entity entity)
{
    Debug::Assert(entityToIndex.find(entity) != entityToIndex.end(), "Retrieving non-existent component.");

    return componentArray[entityToIndex[entity]];
}

template <typename T>
inline void ComponentArray<T>::EntityDestroyed(Entity entity)
{
    auto iterator = entityToIndex.find(entity);
    if(iterator != entityToIndex.end())
    {
        RemoveData(entity);
    }
}

