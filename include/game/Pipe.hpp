#ifndef FLAPPYBIRD_PIPE_HPP
#define FLAPPYBIRD_PIPE_HPP

#include <SDL.h>
#include "game/Entity.hpp"

namespace flappybird {

// Forward declarations
    class ResourceManager;

/**
 * @brief Pipe obstacle
 */
    class Pipe : public Entity {
    public:
        explicit Pipe(ResourceManager *resource_manager);

        ~Pipe() override = default;

        // Update pipe state
        void Update(double delta_time) override;

        // Render pipe
        void Render(SDL_Renderer *renderer) override;

        // Set the center position of the gap in the pipe
        void SetGapY(int y);

        // Reset pipe to a position off-screen
        void Reset(int x_offset);

        // Check if bird passed through the pipe
        [[nodiscard]] bool IsPassed(double bird_x) const;

        // Check if pipe is off-screen
        [[nodiscard]] bool IsOffScreen() const;

        // Get collision boxes for collision detection
        [[nodiscard]] SDL_Rect GetTopCollisionBox() const;

        [[nodiscard]] SDL_Rect GetBottomCollisionBox() const;

        // Check if the pipe has been passed (for scoring)
        [[nodiscard]] bool IsScored() const { return is_scored_; }

        void SetScored(bool scored) { is_scored_ = scored; }

    private:
        ResourceManager *resource_manager_;
        SDL_Texture *pipe_top_texture_;
        SDL_Texture *pipe_bottom_texture_;
        int gap_y_;
        int gap_height_;
        bool is_scored_;

        // Static variable to remember the last gap Y position
        static int last_gap_y_;
        // Maximum variation allowed between consecutive pipes
        static constexpr int max_height_variation_ = 100;
    };

}  // namespace flappybird

#endif  // FLAPPYBIRD_PIPE_HPP
