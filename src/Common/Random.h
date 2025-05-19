#pragma once
#include <random>

class Random {
public:
    Random();

    int Next(int min, int max);
    float Next(float min, float max);

private: 
    std::mt19937 generator;

}; 