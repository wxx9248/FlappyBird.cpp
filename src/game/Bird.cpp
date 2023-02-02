#include "game/Bird.hpp"
#include "core/Constants.hpp"
#include "core/ResourceManager.hpp"
#include "core/AudioManager.hpp"

#include <cmath>

namespace flappybird {

    Bird::Bird(ResourceManager *resource_manager, AudioManager *audio_manager)
            : Entity(kBirdDefaultX, kBirdDefaultY, 34, 24),
              resource_manager_(resource_manager),
              current_state_(State::kMidflap),
              animation_timer_(0.0),
              flap_timer_(0.0),
              flutter_timer_(0.0),
              is_static_animation_(true),
              velocity_(0.0),
              gravity_(kDefaultGravity),
              is_dead_(false),
              is_grounded_(false),
              audio_manager_(audio_manager) {

        // Load textures
        textures_[0] = resource_manager_->LoadTexture("bird_1.png");
        textures_[1] = resource_manager_->LoadTexture("bird_2.png");
        textures_[2] = resource_manager_->LoadTexture("bird_3.png");

        // Get dimensions from the first texture
        if (textures_[0]) {
            int w, h;
            SDL_QueryTexture(textures_[0], nullptr, nullptr, &w, &h);
            width_ = w;
            height_ = h;
        }
    }

    void Bird::Update(double delta_time) {
        if (!is_visible_) {
            return;
        }

        // Don't animate if grounded
        if (!is_grounded_) {
            UpdateAnimation(delta_time);
        }

        if (!is_static_animation_) {
            UpdatePhysics(delta_time);
        }
    }

    void Bird::Render(SDL_Renderer *renderer) {
        if (!is_visible_ || !textures_[static_cast<int>(current_state_)]) {
            return;
        }

        // Create destination rectangle
        SDL_Rect dest_rect = {
                static_cast<int>(x_),
                static_cast<int>(y_),
                width_,
                height_
        };

        // Calculate rotation angle based on velocity
        double angle = 0.0;
        if (!is_static_animation_) {
            // Map velocity to a rotation angle (max 90 degrees down, max 30 degrees up)
            const double max_velocity_down = 15.0;
            const double max_angle_down = 90.0;
            const double max_velocity_up = -10.0;
            const double max_angle_up = -30.0;

            if (velocity_ > 0) {
                // Falling
                angle = std::min(velocity_ / max_velocity_down * max_angle_down, max_angle_down);
            } else {
                // Rising
                angle = std::max(velocity_ / max_velocity_up * max_angle_up, max_angle_up);
            }
        }

        // Render the bird with rotation
        SDL_RenderCopyEx(
                renderer,
                textures_[static_cast<int>(current_state_)],
                nullptr,  // Use the entire source texture
                &dest_rect,
                angle,     // Rotation angle
                nullptr,   // Rotate around center
                SDL_FLIP_NONE  // No flipping
        );
    }

    void Bird::HandleInput(bool jump_pressed) {
        if (jump_pressed && !is_dead_ && !is_static_animation_) {
            Jump();
        }
    }

    void Bird::Reset() {
        // Reset position
        x_ = kBirdDefaultX;
        y_ = kBirdDefaultY;

        // Reset physics
        velocity_ = 0.0;
        is_dead_ = false;
        is_grounded_ = false;

        // Reset animation
        current_state_ = State::kMidflap;
        is_static_animation_ = true;
        animation_timer_ = 0.0;
        flap_timer_ = 0.0;
        flutter_timer_ = 0.0;
    }

    void Bird::Jump() {
        velocity_ = kJumpVelocity;

        // Play wing sound effect if audio manager is available
        if (audio_manager_) {
            audio_manager_->PlaySound(AudioManager::SoundEffect::kWing);
        }
    }

    void Bird::SetStaticAnimation(bool is_static) {
        is_static_animation_ = is_static;
    }

    void Bird::UpdateAnimation(double delta_time) {
        // Update timers
        animation_timer_ += delta_time;
        flap_timer_ += delta_time;
        flutter_timer_ += delta_time;

        if (is_static_animation_) {
            // In static mode, cycle through wing states periodically
            if (flap_timer_ >= kBirdWingFlapPeriod) {
                flap_timer_ = 0.0;

                // Cycle through states: up -> mid -> down -> mid -> up -> ...
                switch (current_state_) {
                    case State::kUpflap:
                        current_state_ = State::kMidflap;
                        break;
                    case State::kMidflap:
                        // Alternate between up and down flap
                        if (static_cast<int>(current_state_) == 1) {
                            current_state_ = State::kDownflap;
                        } else {
                            current_state_ = State::kUpflap;
                        }
                        break;
                    case State::kDownflap:
                        current_state_ = State::kMidflap;
                        break;
                }

                // Apply a subtle vertical flutter
                if (flutter_timer_ >= kBirdFluctuationPeriod) {
                    flutter_timer_ = 0.0;
                }
                y_ += kBirdFluctuationAmplitude * sin(flutter_timer_ * 2 * M_PI / kBirdFluctuationPeriod);
            }
        } else {
            // In dynamic mode, wing flap rate depends on velocity
            double flap_period = (velocity_ < 0)
                                 ? kBirdWingFlapPeriod * 0.5  // Flap faster when rising
                                 : kBirdWingFlapPeriod * 1.5; // Flap slower when falling

            if (flap_timer_ >= flap_period) {
                flap_timer_ = 0.0;

                // Cycle through states
                switch (current_state_) {
                    case State::kUpflap:
                        current_state_ = State::kMidflap;
                        break;
                    case State::kMidflap:
                        current_state_ = State::kDownflap;
                        break;
                    case State::kDownflap:
                        current_state_ = State::kUpflap;
                        break;
                }
            }
        }
    }

    void Bird::UpdatePhysics(double delta_time) {
        if (is_dead_ && is_grounded_) {
            return;
        }

        // Apply gravity
        velocity_ += gravity_;

        // Update position
        y_ += velocity_;

        // Check if bird has hit the ground
        if (y_ + height_ >= kWindowHeight - kGroundHeight) {
            y_ = kWindowHeight - kGroundHeight - height_;
            is_grounded_ = true;
            velocity_ = 0.0;
            is_dead_ = true; // Bird is dead when it hits the ground
        }

        // Keep bird within top bounds
        if (y_ < 0) {
            y_ = 0;
            velocity_ = 0.0;
        }
    }

    SDL_Rect Bird::GetCollisionBox() const {
        return {static_cast<int>(x_), static_cast<int>(y_), width_, height_};
    }

}  // namespace flappybird 