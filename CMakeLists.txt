cmake_minimum_required(VERSION 3.20)
project(FlappyBird)

# macOS specific configuration
if (APPLE)
    include(${CMAKE_CURRENT_SOURCE_DIR}/macos-toolchain.cmake)
endif ()

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Configuration options
option(FLAPPYBIRD_BUILD_TESTS "Build test applications" OFF)

# Find required packages
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(Threads REQUIRED)

# Set include directories
include_directories(
        ${PROJECT_SOURCE_DIR}/include
        ${SDL2_INCLUDE_DIRS}
        ${SDL2_IMAGE_INCLUDE_DIRS}
        ${SDL2_MIXER_INCLUDE_DIRS}
        ${SDL2_TTF_INCLUDE_DIRS}
)

# Source files
file(GLOB_RECURSE SOURCES
        ${PROJECT_SOURCE_DIR}/src/*.cpp
)

# Add executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link libraries
if (APPLE)
    target_link_libraries(${PROJECT_NAME}
            ${SDL2_LIBRARIES}
            ${SDL2_IMAGE_LIBRARIES}
            ${SDL2_MIXER_LIBRARIES}
            ${SDL2_TTF_LIBRARIES}
            Threads::Threads
    )
else ()
    target_link_libraries(${PROJECT_NAME}
            ${SDL2_LIBRARIES}
            SDL2_image
            SDL2_mixer
            SDL2_ttf
            Threads::Threads
    )
endif ()

# Install targets
install(TARGETS ${PROJECT_NAME} DESTINATION bin)

# Copy assets
install(DIRECTORY assets DESTINATION share/${PROJECT_NAME})

# If building tests
if (FLAPPYBIRD_BUILD_TESTS)
    add_subdirectory(tests)
endif ()
