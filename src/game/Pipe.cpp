#include "game/Pipe.hpp"
#include "core/Constants.hpp"
#include "core/ResourceManager.hpp"
#include <algorithm>

namespace flappybird {

    // Initialize static member to a reasonable middle position
    int Pipe::last_gap_y_ = kWindowHeight / 2;

    Pipe::Pipe(ResourceManager *resource_manager)
            : Entity(kWindowWidth, 0, 0, 0),
              resource_manager_(resource_manager),
              pipe_top_texture_(nullptr),
              pipe_bottom_texture_(nullptr),
              gap_y_(0),
              gap_height_(kPipeGap),
              is_scored_(false) {

        // Load textures
        pipe_top_texture_ = resource_manager_->LoadTexture("pipe_up.png");
        pipe_bottom_texture_ = resource_manager_->LoadTexture("pipe_down.png");

        // Set dimensions from textures
        if (pipe_top_texture_) {
            int w, h;
            SDL_QueryTexture(pipe_top_texture_, nullptr, nullptr, &w, &h);
            width_ = w;
            height_ = kWindowHeight;  // Full height of the window
        }
    }

    void Pipe::Update(double delta_time) {
        if (!is_visible_) {
            return;
        }

        // Move pipe leftward
        x_ -= kPipeSpeed;
    }

    void Pipe::Render(SDL_Renderer *renderer) {
        if (!is_visible_ || !pipe_top_texture_ || !pipe_bottom_texture_) {
            return;
        }

        // Get texture dimensions
        int pipe_width, pipe_height;
        SDL_QueryTexture(pipe_top_texture_, nullptr, nullptr, &pipe_width, &pipe_height);

        // Draw top pipe
        SDL_Rect top_dest = {
                static_cast<int>(x_),
                gap_y_ - gap_height_ / 2 - pipe_height,  // Position above the gap
                pipe_width,
                pipe_height
        };

        SDL_RenderCopy(renderer, pipe_top_texture_, nullptr, &top_dest);

        // Draw bottom pipe
        SDL_Rect bottom_dest = {
                static_cast<int>(x_),
                gap_y_ + gap_height_ / 2,  // Position below the gap
                pipe_width,
                pipe_height
        };

        SDL_RenderCopy(renderer, pipe_bottom_texture_, nullptr, &bottom_dest);
    }

    void Pipe::SetGapY(int y) {
        gap_y_ = y;
    }

    void Pipe::Reset(int x_offset) {
        // Position the pipe off-screen to the right
        x_ = kWindowWidth + x_offset;

        // Safety margins from top and ground
        const int top_safety_margin = kPipeTopSafetyMargin;
        const int ground_safety_margin = kPipeGroundSafetyMargin;
        const int ground_height = kGroundHeight;

        // Calculate valid range for the gap center
        // The gap center needs to be at least (gap_height/2 + top_safety_margin) from the top
        // and at least (gap_height/2 + ground_safety_margin) from the ground
        const int min_gap_y = gap_height_ / 2 + top_safety_margin;
        const int max_gap_y = kWindowHeight - ground_height - ground_safety_margin - gap_height_ / 2;

        // Apply constraints to ensure smooth height transitions
        int min_allowed = std::max(min_gap_y, last_gap_y_ - max_height_variation_);
        int max_allowed = std::min(max_gap_y, last_gap_y_ + max_height_variation_);

        // Ensure the range is valid (in case the constraints are too tight)
        if (min_allowed > max_allowed) {
            // If constraints create an invalid range, prioritize safety margins
            min_allowed = min_gap_y;
            max_allowed = max_gap_y;
        }

        // Simple random number generation within the constrained range
        gap_y_ = min_allowed + (rand() % (max_allowed - min_allowed + 1));

        // Update the last gap position for the next pipe
        last_gap_y_ = gap_y_;

        // Reset scored flag
        is_scored_ = false;
    }

    bool Pipe::IsPassed(double bird_x) const {
        // The bird has passed the pipe when its left edge is to the right of the pipe's right edge
        return bird_x > x_ + width_;
    }

    bool Pipe::IsOffScreen() const {
        // The pipe is off-screen when its right edge is to the left of the screen
        return x_ + width_ < 0;
    }

    SDL_Rect Pipe::GetTopCollisionBox() const {
        // Get texture dimensions
        int pipe_width, pipe_height;
        SDL_QueryTexture(pipe_top_texture_, nullptr, nullptr, &pipe_width, &pipe_height);

        SDL_Rect box = {
                static_cast<int>(x_),
                gap_y_ - gap_height_ / 2 - pipe_height,
                pipe_width,
                pipe_height
        };

        return box;
    }

    SDL_Rect Pipe::GetBottomCollisionBox() const {
        // Get texture dimensions
        int pipe_width, pipe_height;
        SDL_QueryTexture(pipe_bottom_texture_, nullptr, nullptr, &pipe_width, &pipe_height);

        SDL_Rect box = {
                static_cast<int>(x_),
                gap_y_ + gap_height_ / 2,
                pipe_width,
                pipe_height
        };

        return box;
    }

}  // namespace flappybird 