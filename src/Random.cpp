#include "Random.h"

Random::Random()
{
    std::random_device device;
    generator.seed(device());
}

int Random::Next(int min, int max)
{
    std::uniform_int_distribution<int> distribution(min, max);
    int returnValue = distribution(generator);
    return returnValue;
}

float Random::Next(float min, float max)
{
    std::uniform_real_distribution<float> distribution(min, max);
    float returnValue = distribution(generator);
    return returnValue;
}
