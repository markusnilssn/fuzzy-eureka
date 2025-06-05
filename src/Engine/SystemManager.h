#pragma once
#include "Entity.h"

#include "System.h"

#include "Common/Debug.h"

#include <unordered_map>
#include <memory>
#include <typeinfo>

class SystemManager final
{
public:
	explicit SystemManager();

	void Start();
	void Destroy();

	void Update(float deltaTime);
	void Render(sf::RenderWindow& window);

	template<typename T, typename... Args>
	inline std::shared_ptr<T> RegisterSystem(Engine& engine, Args&&... args);

    void SetRegistry(const std::type_info& type, Registry registry);

	void EntityDestroyed(Entity entity);
	void EntitySignatureChanged(Entity entity, Registry entitySignature);

private:
	std::unordered_map<std::string, Registry> registries;
	std::unordered_map<std::string, std::shared_ptr<System>> systems;
    
};

template <typename T, typename... Args>
inline std::shared_ptr<T> SystemManager::RegisterSystem(Engine& engine, Args&&... args)
{
	static_assert(std::is_base_of<System, T>::value, "T must inherit from System");

	const char* typeName = typeid(T).name();

	Debug::Assert(systems.find(typeName) == systems.end(), "Registering system more than once.");

	std::shared_ptr<T> system = std::make_shared<T>(engine, std::forward<Args>(args)...);
	systems.insert({typeName, system});
	system->Start();
	return system;
}