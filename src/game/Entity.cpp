#include "game/Entity.hpp"

namespace flappybird {

    Entity::Entity(double x, double y, int width, int height)
            : x_(x), y_(y), width_(width), height_(height), is_visible_(false) {}

    SDL_Rect Entity::GetCollisionBox() const {
        SDL_Rect rect = {
                static_cast<int>(x_),
                static_cast<int>(y_),
                width_,
                height_
        };
        return rect;
    }

    bool Entity::CollidesWith(const Entity &other) const {
        // If either entity is not visible, no collision
        if (!is_visible_ || !other.IsVisible()) {
            return false;
        }

        // Create collision boxes
        SDL_Rect this_box = GetCollisionBox();
        SDL_Rect other_box = other.GetCollisionBox();

        // Check for collision
        return SDL_HasIntersection(&this_box, &other_box) == SDL_TRUE;
    }

}  // namespace flappybird 