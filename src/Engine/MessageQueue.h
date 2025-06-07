#pragma once
#include <queue>
#include <any>
#include <unordered_map>
#include <functional>
#include <string>
#include <list>
#include <iostream>
#include <memory>

class Message 
{
public:
    virtual ~Message() = default;
};

class MessageQueue final
{
    struct Subscriber
    {
        std::function<void(const Message&)> callback;
    };

public:
    template <typename T = Message, typename... Args>
    inline void Send(Args&&... args);

    template<typename T = Message>
    void Subscribe(std::function<void(const T&)> callback);
    void Unsubscribe();

    void Update();

private:
    std::unordered_map<size_t, std::list<Subscriber>> subscribers;
    std::queue<std::pair<size_t, std::unique_ptr<Message>>> messages;
    
};

template <typename T, typename... Args>
inline void MessageQueue::Send(Args&&... args)
{
    static_assert(std::is_base_of<Message, T>::value, "T must be derived from Message");

    size_t hashCode = typeid(T).hash_code();

    std::unique_ptr<T> message = std::make_unique<T>(std::forward<Args>(args)...);

    messages.push({hashCode, std::move(message)});
}

template <typename T>
inline void MessageQueue::Subscribe(std::function<void(const T&)> callback)
{
    static_assert(std::is_base_of<Message, T>::value, "T must be derived from Message");

    size_t hashCode = typeid(T).hash_code();

    Subscriber subscriber{};
    subscriber.callback = [callback](const Message& message)
    {
        try
        {
            const T& data = static_cast<const T&>(message);
            callback(data);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << " while casting " << typeid(T).name() << std::endl;
        }
    };

    subscribers[hashCode].push_back(std::move(subscriber));
}
