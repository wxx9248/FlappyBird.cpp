#include "core/Game.hpp"

#include <iostream>
#include <memory>

#ifdef _WIN32
#include <Windows.h>
#endif

#ifdef _WIN32
// Windows entry point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Redirect stdout to a console when in debug mode
#ifdef _DEBUG
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);
#endif
#else

// Standard entry point for other platforms
int main() {
#endif
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
