# Linux specific configurations

# Use MODULE mode for packages
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)

# Linux-specific include directories
include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_IMAGE_INCLUDE_DIRS}
    ${SDL2_MIXER_INCLUDE_DIRS}
    ${SDL2_TTF_INCLUDE_DIRS}
)

# Link Linux-specific libraries
target_link_libraries(${PROJECT_NAME}
    ${SDL2_LIBRARIES}
    SDL2_image
    SDL2_mixer
    SDL2_ttf
    Threads::Threads
) 