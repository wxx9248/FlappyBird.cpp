#include "core/Game.hpp"

#include <iostream>
#include <memory>

int main() {
    // Create game instance
    auto game = std::make_unique<flappybird::Game>();

    // Initialize game
    if (!game->Init()) {
        std::cerr << "Failed to initialize game! Exiting..." << std::endl;
        return 1;
    }

    // Run game
    game->Run();

    return 0;
}
