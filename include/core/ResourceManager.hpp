#ifndef FLAPPYBIRD_RESOURCE_MANAGER_HPP
#define FLAPPYBIRD_RESOURCE_MANAGER_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include <memory>
#include <string>
#include <unordered_map>

namespace flappybird {

// Forward declaration
    class Game;

/**
 * @brief Manages all game resources (textures, fonts, sounds)
 */
    class ResourceManager {
    public:
        explicit ResourceManager(SDL_Renderer *renderer);

        ~ResourceManager();

        // Prevents copying
        ResourceManager(const ResourceManager &) = delete;

        ResourceManager &operator=(const ResourceManager &) = delete;

        // Initialize the resource manager
        static bool Init();

        // Load a texture from file
        SDL_Texture *LoadTexture(const std::string &filename);

        // Get a texture that has been loaded
        SDL_Texture *GetTexture(const std::string &name);

        // Load a font at a specific size
        TTF_Font *LoadFont(const std::string &filename, int size);

        // Get a font that has been loaded
        TTF_Font *GetFont(const std::string &name, int size);

        // Load a sound effect
        Mix_Chunk *LoadSound(const std::string &filename);

        // Get a sound effect that has been loaded
        Mix_Chunk *GetSound(const std::string &name);

        // Clean up resources
        void Clean();

    private:
        SDL_Renderer *renderer_;
        std::unordered_map<std::string, SDL_Texture *> textures_;
        std::unordered_map<std::string, TTF_Font *> fonts_;
        std::unordered_map<std::string, Mix_Chunk *> sounds_;
    };

}  // namespace flappybird

#endif  // FLAPPYBIRD_RESOURCE_MANAGER_HPP
