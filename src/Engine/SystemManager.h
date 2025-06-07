#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "Entity.h"
#include "Signature.h"

#include "Common/Debug.h"

#include <unordered_map>
#include <memory>
#include <typeindex>
#include <typeinfo>

class System;
class Engine;
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

    void SetSignature(std::type_index type, Signature signature);

	void EntityDestroyed(Entity entity);
	void EntitySignatureChanged(Entity entity, Signature entitySignature);

private:
	std::unordered_map<std::type_index, Signature> signatures;
	std::unordered_map<std::type_index, std::shared_ptr<System>> systems;
    
};

template <typename T, typename... Args>
inline std::shared_ptr<T> SystemManager::RegisterSystem(Engine& engine, Args&&... args)
{
	static_assert(std::is_base_of<System, T>::value, "T must inherit from System");

	std::type_index type(typeid(T));

	Debug::Assert(systems.find(type) == systems.end(), "Registering system more than once.");

	std::shared_ptr<T> system = std::make_shared<T>(engine, std::forward<Args>(args)...);
	systems.insert({type, system});
	return system;
}