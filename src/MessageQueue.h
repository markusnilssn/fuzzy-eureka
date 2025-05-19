#pragma once
#include <queue>
#include <any>
#include <unordered_map>
#include <functional>
#include <string>

class Message {};

class MessageQueue final
{
public:
    struct Handle {
        std::string name;
    };

public:
    template<typename T = Message>
    void Send(T&& message);

    template<typename T = Message>
    [[nodiscard]] Handle Subscribe(std::function<void(T)> callback);

    bool Unsubscribe(Handle handle);

    void Process();
    void Clear();

private:
    std::unordered_map<Handle, std::function<void(std::any)>> subscribers;
    std::queue<std::any> messages;

};