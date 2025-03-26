# Windows specific configurations for vcpkg
# If using manifest mode, vcpkg_installed should exist in project root
if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg_installed")
    message(STATUS "Using vcpkg in manifest mode")

    # Add architecture-specific path for Windows
    if (CMAKE_SIZEOF_VOID_P EQUAL 8)
        # 64-bit architecture
        set(VCPKG_INSTALLED_DIR "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg_installed/x64-windows")
    else ()
        # 32-bit architecture
        set(VCPKG_INSTALLED_DIR "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg_installed/x86-windows")
    endif ()

    # Add vcpkg paths to CMake's search paths
    list(APPEND CMAKE_PREFIX_PATH "${VCPKG_INSTALLED_DIR}")
    list(APPEND CMAKE_PREFIX_PATH "${VCPKG_INSTALLED_DIR}/share")

    # Set package config paths explicitly
    set(SDL2_DIR "${VCPKG_INSTALLED_DIR}/share/sdl2")
    set(SDL2_image_DIR "${VCPKG_INSTALLED_DIR}/share/SDL2_image")
    set(SDL2_mixer_DIR "${VCPKG_INSTALLED_DIR}/share/SDL2_mixer")
    set(SDL2_ttf_DIR "${VCPKG_INSTALLED_DIR}/share/SDL2_ttf")
else ()
    # Set vcpkg toolchain file if not already set and not using manifest mode
    if (NOT DEFINED CMAKE_TOOLCHAIN_FILE)
        set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
        if (NOT EXISTS "${CMAKE_TOOLCHAIN_FILE}")
            message(WARNING "VCPKG toolchain file not found: ${CMAKE_TOOLCHAIN_FILE}")
            message(STATUS "Please set VCPKG_ROOT environment variable to your vcpkg installation directory")
        endif ()
    endif ()
endif ()

# Use CONFIG mode for vcpkg packages
find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
find_package(SDL2_mixer CONFIG REQUIRED)
find_package(SDL2_ttf CONFIG REQUIRED)

# Windows-specific include directories
include_directories(
        ${PROJECT_SOURCE_DIR}/include
)

# Set Windows-specific properties for the executable
set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE TRUE
)

# Add the entry point for Windows GUI application
target_sources(${PROJECT_NAME} PRIVATE src/Main.cpp)

# Link Windows-specific libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
        $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
        $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
        $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>
        $<IF:$<TARGET_EXISTS:SDL2_mixer::SDL2_mixer>,SDL2_mixer::SDL2_mixer,SDL2_mixer::SDL2_mixer-static>
        $<IF:$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>,SDL2_ttf::SDL2_ttf,SDL2_ttf::SDL2_ttf-static>
        Threads::Threads
)

# Copy DLLs to output directory for Windows
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}>
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND_EXPAND_LISTS
)

# Copy additional required DLLs based on build type
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E echo "Copying all dependency DLLs..."
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "$<IF:$<CONFIG:Debug>,${VCPKG_INSTALLED_DIR}/debug/bin,${VCPKG_INSTALLED_DIR}/bin>"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
)

# Also copy assets to output directory for Windows
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
) 
