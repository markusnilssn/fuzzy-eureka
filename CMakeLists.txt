cmake_minimum_required(VERSION 3.28)
project(CMakeSFMLProject LANGUAGES CXX)

# Set the output directory for the executable
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Fetch SFML from GitHub repository
include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.1
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

# Add the executable for the project
add_executable(main src/main.cpp src/application.cpp)

# Ensure that the project uses C++17
target_compile_features(main PRIVATE cxx_std_17)

# Link the necessary SFML libraries
target_link_libraries(main PRIVATE SFML::Graphics SFML::Window SFML::System)
