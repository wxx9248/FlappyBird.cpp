# FlappyBird.cpp

A modern C++ implementation of the classic Flappy Bird game using SDL2.

## Description

This project is a reimplementation of the Flappy Bird game using C++23 and SDL2. The game features smooth animations,
sound effects, and a familiar gameplay experience.

## Features

- Modern C++23 standard
- Cross-platform compatibility using SDL2
- Smooth animations with frame rate stabilization
- Clean object-oriented design
- Multithreaded game logic for better performance
- Score tracking and medals

## Dependencies

- C++23 compatible compiler
- SDL2
- SDL2_image
- SDL2_mixer
- SDL2_ttf
- CMake 3.20 or higher

## Building

### Linux/macOS

To build the project on Linux or macOS, follow these steps:

```bash
mkdir build && cd build
cmake ..
make
```

### Windows with vcpkg and Visual Studio

#### Install required packages:

```cmd
vcpkg install sdl2 sdl2-image sdl2-mixer sdl2-ttf --triplet=x64-windows
```

#### Generate Visual Studio project:

```cmd
mkdir build && cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE="%VCPKG_ROOT%/scripts/buildsystems/vcpkg.cmake" -DCMAKE_GENERATOR_PLATFORM=x64
```

#### Open the generated .sln file in Visual Studio and build the project, or build from command line:

```cmd
cmake --build . --config Release
```

## Running

### Linux/macOS

After building, you can run the game from the build directory:

```bash
./FlappyBird
```

### Windows

After building, you can run the game from the build directory:

```cmd
.\Release\FlappyBird.exe
```

## Controls

- Press Space or Up Arrow to make the bird flap its wings
- Press M to toggle mute
- Press Escape to quit the game

## Project Structure

The project is organized into several modules:

- `core`: Core game functionality (game loop, resource management)
- `game`: Game entities (bird, pipes)
- `audio`: Audio management
- `graphics`: Rendering functionality

## License

[MIT License](LICENSE)
