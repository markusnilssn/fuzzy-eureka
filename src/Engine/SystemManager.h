#pragma once
#include "Entity.h"
#include "System.h"
#include "Signature.h"
#include "Common/Debug.h"
#include <unordered_map>

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
	std::unordered_map<std::string, Signature> signatures;
	std::unordered_map<std::string, std::shared_ptr<System>> systems;
    
};

template <typename T>
inline std::shared_ptr<T> SystemManager::RegisterSystem()
{
	const char* typeName = typeid(T).name();

	Debug::Assert(systems.find(typeName) == systems.end(), "Registering system more than once.");

	// Create a pointer to the system and return it so it can be used externally
	auto system = std::make_shared<T>();
	systems.insert({typeName, system});
	return system;
}

template <typename T>
inline void SystemManager::SetSignature(Signature signature)
{
	const char* typeName = typeid(T).name();

	Debug::Assert(systems.find(typeName) != systems.end(), "System used before registered.");

	signatures.insert({typeName, signature});
}
