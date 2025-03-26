cmake_minimum_required(VERSION 3.20)
project(FlappyBird)

# macOS specific configurations for vcpkg
if (APPLE)
    include(${CMAKE_CURRENT_SOURCE_DIR}/macos-toolchain.cmake)
endif ()

# Windows specific configurations for vcpkg
if (WIN32)
    # Set vcpkg toolchain file if not already set
    if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
        set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
        if(NOT EXISTS "${CMAKE_TOOLCHAIN_FILE}")
            message(WARNING "VCPKG toolchain file not found: ${CMAKE_TOOLCHAIN_FILE}")
            message(STATUS "Please set VCPKG_ROOT environment variable to your vcpkg installation directory")
        endif()
    endif()
endif()

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

# Set Windows-specific properties
if(WIN32)
    # Add windows subsystem for GUI applications
    set_target_properties(${PROJECT_NAME} PROPERTIES
            WIN32_EXECUTABLE TRUE
    )
    
    # Add the entry point for Windows GUI application
    target_sources(${PROJECT_NAME} PRIVATE src/Main.cpp)
endif()

# Link libraries based on platform
if (APPLE)
    target_link_libraries(${PROJECT_NAME}
            ${SDL2_LIBRARIES}
            ${SDL2_IMAGE_LIBRARIES}
            ${SDL2_MIXER_LIBRARIES}
            ${SDL2_TTF_LIBRARIES}
            Threads::Threads
    )
elseif(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE
            ${SDL2_LIBRARIES}
            SDL2::SDL2
            SDL2::SDL2main
            SDL2_image::SDL2_image
            SDL2_mixer::SDL2_mixer
            SDL2_ttf::SDL2_ttf
            Threads::Threads
    )
else()
    target_link_libraries(${PROJECT_NAME}
            ${SDL2_LIBRARIES}
            SDL2_image
            SDL2_mixer
            SDL2_ttf
            Threads::Threads
    )
endif()

# Copy DLLs to output directory for Windows
if(WIN32)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}>
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND_EXPAND_LISTS
    )
endif()

# Install targets
install(TARGETS ${PROJECT_NAME} DESTINATION bin)

# Copy assets
install(DIRECTORY assets DESTINATION share/${PROJECT_NAME})

# If building tests
if (FLAPPYBIRD_BUILD_TESTS)
    add_subdirectory(tests)
endif()
