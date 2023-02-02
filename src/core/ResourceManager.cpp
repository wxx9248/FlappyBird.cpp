#include "core/ResourceManager.hpp"
#include "core/Constants.hpp"

#include <iostream>

namespace flappybird {

    ResourceManager::ResourceManager(SDL_Renderer *renderer)
            : renderer_(renderer) {}

    ResourceManager::~ResourceManager() {
        Clean();
    }

    bool ResourceManager::Init() {
        // Initialize SDL_image
        if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
            std::cerr << "SDL_image could not initialize! SDL_image Error: "
                      << IMG_GetError() << std::endl;
            return false;
        }

        // Initialize SDL_ttf
        if (TTF_Init() == -1) {
            std::cerr << "SDL_ttf could not initialize! SDL_ttf Error: "
                      << TTF_GetError() << std::endl;
            return false;
        }

        // Initialize SDL_mixer
        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
            std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: "
                      << Mix_GetError() << std::endl;
            return false;
        }

        return true;
    }

    SDL_Texture *ResourceManager::LoadTexture(const std::string &filename) {
        // Full path to the image
        std::string full_path = kImagesPath + filename;

        // Check if texture is already loaded
        auto it = textures_.find(filename);
        if (it != textures_.end()) {
            return it->second;
        }

        // Load the texture from file
        SDL_Surface *surface = IMG_Load(full_path.c_str());
        if (!surface) {
            std::cerr << "Unable to load image " << full_path << "! SDL_image Error: "
                      << IMG_GetError() << std::endl;
            return nullptr;
        }

        // Create texture from surface
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer_, surface);
        if (!texture) {
            std::cerr << "Unable to create texture from " << full_path
                      << "! SDL Error: " << SDL_GetError() << std::endl;
            SDL_FreeSurface(surface);
            return nullptr;
        }

        // Free the loaded surface
        SDL_FreeSurface(surface);

        // Store and return the texture
        textures_[filename] = texture;
        return texture;
    }

    SDL_Texture *ResourceManager::GetTexture(const std::string &name) {
        auto it = textures_.find(name);
        if (it != textures_.end()) {
            return it->second;
        }
        return nullptr;
    }

    TTF_Font *ResourceManager::LoadFont(const std::string &filename, int size) {
        // Full path to the font
        std::string full_path = kFontsPath + filename;

        // Create a unique key for the font with its size
        std::string font_key = filename + "_" + std::to_string(size);

        // Check if font is already loaded
        auto it = fonts_.find(font_key);
        if (it != fonts_.end()) {
            return it->second;
        }

        // Load the font from file
        TTF_Font *font = TTF_OpenFont(full_path.c_str(), size);
        if (!font) {
            std::cerr << "Failed to load font " << full_path << "! SDL_ttf Error: "
                      << TTF_GetError() << std::endl;
            return nullptr;
        }

        // Store and return the font
        fonts_[font_key] = font;
        return font;
    }

    TTF_Font *ResourceManager::GetFont(const std::string &name, int size) {
        // Create the key for the font
        std::string font_key = name + "_" + std::to_string(size);

        auto it = fonts_.find(font_key);
        if (it != fonts_.end()) {
            return it->second;
        }
        return nullptr;
    }

    Mix_Chunk *ResourceManager::LoadSound(const std::string &filename) {
        // Full path to the sound
        std::string full_path = kSoundsPath + filename;

        // Check if sound is already loaded
        auto it = sounds_.find(filename);
        if (it != sounds_.end()) {
            return it->second;
        }

        // Load the sound from file
        Mix_Chunk *sound = Mix_LoadWAV(full_path.c_str());
        if (!sound) {
            std::cerr << "Failed to load sound " << full_path << "! SDL_mixer Error: "
                      << Mix_GetError() << std::endl;
            return nullptr;
        }

        // Store and return the sound
        sounds_[filename] = sound;
        return sound;
    }

    Mix_Chunk *ResourceManager::GetSound(const std::string &name) {
        auto it = sounds_.find(name);
        if (it != sounds_.end()) {
            return it->second;
        }
        return nullptr;
    }

    void ResourceManager::Clean() {
        // Free all textures
        for (auto &pair: textures_) {
            SDL_DestroyTexture(pair.second);
        }
        textures_.clear();

        // Free all fonts
        for (auto &pair: fonts_) {
            TTF_CloseFont(pair.second);
        }
        fonts_.clear();

        // Free all sounds
        for (auto &pair: sounds_) {
            Mix_FreeChunk(pair.second);
        }
        sounds_.clear();

        // Quit SDL_image, SDL_ttf, and SDL_mixer
        IMG_Quit();
        TTF_Quit();
        Mix_CloseAudio();
    }

}  // namespace flappybird 