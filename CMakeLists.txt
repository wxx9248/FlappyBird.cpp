cmake_minimum_required(VERSION 3.20)
project(FlappyBird VERSION 2.0.1)

# Platform-specific configurations for vcpkg
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    # Windows vcpkg integration
    if(WIN32 AND DEFINED ENV{VCPKG_ROOT})
        set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" 
            CACHE STRING "Vcpkg toolchain file")
        if(NOT EXISTS "${CMAKE_TOOLCHAIN_FILE}")
            message(WARNING "VCPKG toolchain file not found: ${CMAKE_TOOLCHAIN_FILE}")
            message(STATUS "Please set VCPKG_ROOT environment variable to your vcpkg installation directory")
        endif()
    # macOS integration
    elseif(APPLE)
        include(${CMAKE_CURRENT_SOURCE_DIR}/macos-toolchain.cmake)
    endif()
endif()

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Configuration options
option(FLAPPYBIRD_BUILD_TESTS "Build test applications" OFF)

# Find required packages - platform specific approaches
if(WIN32)
    # On Windows with vcpkg, we use the config mode
    find_package(SDL2 CONFIG REQUIRED)
    find_package(SDL2_image CONFIG REQUIRED)
    find_package(SDL2_mixer CONFIG REQUIRED)
    find_package(SDL2_ttf CONFIG REQUIRED)
else()
    # On Linux/macOS, use the standard approach
    find_package(SDL2 REQUIRED)
    find_package(SDL2_image REQUIRED)
    find_package(SDL2_mixer REQUIRED)
    find_package(SDL2_ttf REQUIRED)
endif()

# Common dependencies
find_package(Threads REQUIRED)

# Set include directories - platform specific
if(WIN32)
    # On Windows, the SDL2 headers are available through the imported targets
    include_directories(
        ${PROJECT_SOURCE_DIR}/include
    )
else()
    # On other platforms, we need to include the SDL2 headers explicitly
    include_directories(
        ${PROJECT_SOURCE_DIR}/include
        ${SDL2_INCLUDE_DIRS}
        ${SDL2_IMAGE_INCLUDE_DIRS}
        ${SDL2_MIXER_INCLUDE_DIRS}
        ${SDL2_TTF_INCLUDE_DIRS}
    )
endif()

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
endif()

# Link libraries based on platform
if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE
            SDL2::SDL2
            SDL2::SDL2main
            SDL2_image::SDL2_image
            SDL2_mixer::SDL2_mixer
            SDL2_ttf::SDL2_ttf
            Threads::Threads
    )
elseif(APPLE)
    target_link_libraries(${PROJECT_NAME}
            ${SDL2_LIBRARIES}
            ${SDL2_IMAGE_LIBRARIES}
            ${SDL2_MIXER_LIBRARIES}
            ${SDL2_TTF_LIBRARIES}
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
    # Add custom command to copy all dependency DLLs to the output directory
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}>
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND_EXPAND_LISTS
    )
    
    # Additional DLLs that might not be covered by the above command
    # This is especially important for SDL2 and its extensions
    if(DEFINED ENV{VCPKG_ROOT})
        file(GLOB SDL2_DLLS 
            "$ENV{VCPKG_ROOT}/installed/x64-windows/bin/SDL2*.dll"
        )
        foreach(SDL2_DLL ${SDL2_DLLS})
            add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    ${SDL2_DLL}
                    $<TARGET_FILE_DIR:${PROJECT_NAME}>
            )
        endforeach()
    endif()
endif()

# Install targets
install(TARGETS ${PROJECT_NAME} DESTINATION bin)

# Copy assets
install(DIRECTORY assets DESTINATION share/${PROJECT_NAME})

# If building tests
if(FLAPPYBIRD_BUILD_TESTS)
    add_subdirectory(tests)
endif()
