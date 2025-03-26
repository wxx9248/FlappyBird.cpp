# macOS specific toolchain file for Homebrew-installed dependencies

# Set Homebrew prefix for Apple Silicon or Intel Mac
if (CMAKE_HOST_SYSTEM_PROCESSOR STREQUAL "arm64")
    set(HOMEBREW_PREFIX "/opt/homebrew")
else ()
    set(HOMEBREW_PREFIX "/usr/local")
endif ()

# Add Homebrew include and library paths
list(APPEND CMAKE_PREFIX_PATH "${HOMEBREW_PREFIX}")

# Make sure SDL2 and its extensions are found correctly
set(SDL2_DIR "${HOMEBREW_PREFIX}/lib/cmake/SDL2")
set(SDL2_image_DIR "${HOMEBREW_PREFIX}/lib/cmake/SDL2_image")
set(SDL2_mixer_DIR "${HOMEBREW_PREFIX}/lib/cmake/SDL2_mixer")
set(SDL2_ttf_DIR "${HOMEBREW_PREFIX}/lib/cmake/SDL2_ttf")

# Set full paths to libraries for macOS
set(SDL2_LIBRARIES "${HOMEBREW_PREFIX}/lib/libSDL2.dylib")
set(SDL2_IMAGE_LIBRARIES "${HOMEBREW_PREFIX}/lib/libSDL2_image.dylib")
set(SDL2_MIXER_LIBRARIES "${HOMEBREW_PREFIX}/lib/libSDL2_mixer.dylib")
set(SDL2_TTF_LIBRARIES "${HOMEBREW_PREFIX}/lib/libSDL2_ttf.dylib")

# Add Homebrew include directories directly
include_directories(SYSTEM
        "${HOMEBREW_PREFIX}/include"
        "${HOMEBREW_PREFIX}/include/SDL2"
)
