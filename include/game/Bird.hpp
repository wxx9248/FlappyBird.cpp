#ifndef FLAPPYBIRD_BIRD_HPP
#define FLAPPYBIRD_BIRD_HPP

#include <SDL2/SDL.h>
#include <array>
#include "game/Entity.hpp"
#include "core/AudioManager.hpp"

namespace flappybird {

// Forward declaration
    class ResourceManager;

/**
 * @brief Bird player character
 */
    class Bird : public Entity {
    public:
        // Animation states
        enum class State {
            kUpflap = 0,
            kMidflap = 1,
            kDownflap = 2
        };

        explicit Bird(ResourceManager *resource_manager, AudioManager *audio_manager = nullptr);

        ~Bird() override = default;

        // Update bird state
        void Update(double delta_time) override;

        // Render bird
        void Render(SDL_Renderer *renderer) override;

        // Handle player input for the bird
        void HandleInput(bool jump_pressed);

        // Reset bird to starting position
        void Reset();

        // Make bird jump upward
        void Jump();

        // Set animation mode
        void SetStaticAnimation(bool is_static);

        // Check if bird is dead
        [[nodiscard]] bool IsDead() const { return is_dead_; }

        void SetDead(bool is_dead) { is_dead_ = is_dead; }

        // Get current vertical velocity
        [[nodiscard]] double GetVelocity() const { return velocity_; }

        // Get collision box
        [[nodiscard]] SDL_Rect GetCollisionBox() const;

    private:
        // Update flight animation
        void UpdateAnimation(double delta_time);

        // Update physics
        void UpdatePhysics(double delta_time);

        ResourceManager *resource_manager_;

        // Animation state
        State current_state_;
        double animation_timer_;
        double flap_timer_;
        double flutter_timer_;
        bool is_static_animation_;

        // Physics
        double velocity_;
        double gravity_;
        bool is_dead_;
        bool is_grounded_;

        // Textures
        std::array<SDL_Texture *, 3> textures_{};

        AudioManager *audio_manager_;
    };

}  // namespace flappybird

#endif  // FLAPPYBIRD_BIRD_HPP
