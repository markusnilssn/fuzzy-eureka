#pragma once 
#include <stdint.h>
#include <bitset>

using ComponentType = uint8_t;

constexpr ComponentType MAX_COMPONENTS = 32;

using Registry = std::bitset<MAX_COMPONENTS>;