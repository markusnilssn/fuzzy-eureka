cmake_minimum_required(VERSION 3.28)
project(CMakeSFMLProject LANGUAGES CXX)

set(CMAKE_BUILD_TYPE Debug)


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
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Add the executable with all source files collected
add_executable(main ${SOURCES})

# Ensure that the project uses C++17
target_compile_features(main PRIVATE cxx_std_17)

# Add root directories to the include path
target_include_directories(main PRIVATE 
    ${CMAKE_SOURCE_DIR}/src  # This ensures src is also included for header search
)

# Link the necessary SFML libraries
target_link_libraries(main PRIVATE SFML::Graphics SFML::Window SFML::System)
