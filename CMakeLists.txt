cmake_minimum_required(VERSION 3.20)
project(FlappyBird VERSION 2.0.1)

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Configuration options
option(FLAPPYBIRD_BUILD_TESTS "Build test applications" OFF)

# Source files
file(GLOB_RECURSE SOURCES
    ${PROJECT_SOURCE_DIR}/src/*.cpp
)

# Add executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Find Threads package (used by all platforms)
find_package(Threads REQUIRED)

# Platform-specific configurations
if(APPLE)
    include(${CMAKE_CURRENT_SOURCE_DIR}/macos-toolchain.cmake)
elseif(WIN32)
    include(${CMAKE_CURRENT_SOURCE_DIR}/windows-toolchain.cmake)
else() # Linux/Unix
    include(${CMAKE_CURRENT_SOURCE_DIR}/linux-toolchain.cmake)
endif()

# Install targets
install(TARGETS ${PROJECT_NAME} DESTINATION bin)

# Copy assets
install(DIRECTORY assets DESTINATION share/${PROJECT_NAME})

# If building tests
if(FLAPPYBIRD_BUILD_TESTS)
    add_subdirectory(tests)
endif()
