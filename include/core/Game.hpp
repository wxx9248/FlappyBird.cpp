#ifndef FLAPPYBIRD_GAME_HPP
#define FLAPPYBIRD_GAME_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <memory>
#include <mutex>
#include <thread>
#include <atomic>

#include "core/GameState.hpp"
#include "core/ResourceManager.hpp"
#include "AudioManager.hpp"
#include "game/Bird.hpp"
#include "game/Pipe.hpp"

namespace flappybird {

/**
 * @brief Main game class
 */
    class Game {
    public:
        Game();

        ~Game();

        // Initialize the game
        bool Init();

        // Main game loop
        void Run();

        // Handle user input
        void HandleEvents();

        // Update game state
        void Update(double delta_time);

        // Render the game
        void Render();

        // Clean up resources
        void Clean();

    private:
        // Game loop control
        bool is_running_;
        Uint32 last_frame_time_;

        // SDL components
        SDL_Window *window_;
        SDL_Renderer *renderer_;

        // Game components
        std::unique_ptr<ResourceManager> resource_manager_;
        std::unique_ptr<AudioManager> audio_manager_;
        std::unique_ptr<GameState> game_state_;

        // Game entities
        std::unique_ptr<Bird> bird_;
        std::vector<std::unique_ptr<Pipe>> pipes_;

        // Background textures
        SDL_Texture *background_texture_;
        SDL_Texture *ground_texture_;
        SDL_Texture *logo_texture_;
        SDL_Texture *game_over_texture_;
        SDL_Texture *scoreboard_texture_;
        SDL_Texture *hint_texture_;
        SDL_Texture *restart_button_texture_;
        SDL_Texture *new_high_score_texture_;

        // Multithreading components
        std::mutex render_mutex_;
        std::atomic<bool> should_update_pipes_;
        std::thread pipe_update_thread_;
        std::thread physics_update_thread_;

        // Coordinates for ground animation
        int ground_x_;

        // Medal textures and selection
        std::vector<SDL_Texture *> medal_textures_;

        // Private helper methods
        void RenderMenu();

        void RenderCountdown();

        void RenderPlaying();

        void RenderGameOver();

        // Render UI elements
        void RenderScore();

        void RenderHighScore();

        void RenderMedal();

        void RenderNewHighScore();

        // Reset game for a new round
        void ResetGame();

        // Initialize threads
        void InitThreads();

        // Thread functions
        void PipeUpdateLoop();

        void PhysicsUpdateLoop();

        // Check for collisions
        bool CheckCollisions();
    };

}  // namespace flappybird

#endif  // FLAPPYBIRD_GAME_HPP
