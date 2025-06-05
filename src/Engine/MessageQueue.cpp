#include "MessageQueue.h"
#include <iostream>

void MessageQueue::Unsubscribe()
{

}

void MessageQueue::Update()
{
    while (messages.size() > 0)
    {
        auto&& [typeName, message] = messages.front();

        for(auto& subscriber : subscribers[typeName])
        {
            subscriber.callback(*message);
        }

        messages.pop();
    }
}