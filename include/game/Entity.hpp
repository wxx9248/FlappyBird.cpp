#ifndef FLAPPYBIRD_ENTITY_HPP
#define FLAPPYBIRD_ENTITY_HPP

#include <SDL2/SDL.h>

namespace flappybird {

/**
 * @brief Base class for all game entities
 */
    class Entity {
    public:
        Entity() = default;

        Entity(double x, double y, int width, int height);

        virtual ~Entity() = default;

        // Updates entity state
        virtual void Update(double delta_time) = 0;

        // Renders entity
        virtual void Render(SDL_Renderer *renderer) = 0;

        // Get entity position and dimensions
        [[nodiscard]] double GetX() const { return x_; }

        [[nodiscard]] double GetY() const { return y_; }

        [[nodiscard]] int GetWidth() const { return width_; }

        [[nodiscard]] int GetHeight() const { return height_; }

        // Set entity position
        void SetX(double x) { x_ = x; }

        void SetY(double y) { y_ = y; }

        // Set entity dimensions
        void SetWidth(int width) { width_ = width; }

        void SetHeight(int height) { height_ = height; }

        // Set entity visibility
        void SetVisible(bool visible) { is_visible_ = visible; }

        [[nodiscard]] bool IsVisible() const { return is_visible_; }

        // Collision detection
        [[nodiscard]] SDL_Rect GetCollisionBox() const;

        [[nodiscard]] bool CollidesWith(const Entity &other) const;

    protected:
        double x_ = 0.0;
        double y_ = 0.0;
        int width_ = 0;
        int height_ = 0;
        bool is_visible_ = false;
    };

}  // namespace flappybird

#endif  // FLAPPYBIRD_ENTITY_HPP
