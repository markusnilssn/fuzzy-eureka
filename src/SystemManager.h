#pragma once
#include <unordered_map>
#include "Entity.h"
#include "System.h"

class SystemManager
{
public:
	template<typename T>
	std::shared_ptr<T> RegisterSystem();

    template<typename T>
    void SetSignature(Signature signature);

	void EntityDestroyed(Entity entity);
	void EntitySignatureChanged(Entity entity, Signature entitySignature);

private:
	std::unordered_map<const char*, Signature> signatures;
	std::unordered_map<const char*, std::shared_ptr<System>> systems;
    
};