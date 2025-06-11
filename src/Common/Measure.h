#pragma once
#include <string>
#include <chrono>
#include <iostream>

struct Measure
{
    std::string functionName;
    std::chrono::steady_clock::time_point startTime;

    Measure(const std::string& functionName)
        : functionName(functionName) 
    {
        startTime = std::chrono::high_resolution_clock::now();
    }

    ~Measure()
    {
        auto endTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
        std::cout << "Function " << functionName << " took " << duration.count() << " ms" << std::endl;
    }
};