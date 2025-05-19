#pragma once 
#include "Entity.h"
#include "Component.h"

class IComponentArray {
public:
	virtual ~IComponentArray() = default;
	virtual void EntityDestroyed(Entity entity) = 0;
};


class ComponentManager {
public:
	template<typename T>
	void RegisterComponent();

    template<typename T>
    ComponentType GetComponentType();

    template<typename T>
    void AddComponent(Entity entity, T component);
    template<typename T>
    void RemoveComponent(Entity entity);
    template<typename T>
    T& GetComponent(Entity entity);

    void EntityDestroyed(Entity entity);

private:


};

