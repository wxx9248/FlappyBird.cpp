#include "core/Game.hpp"
#include "core/Constants.hpp"

#include <iostream>
#include <chrono>
#include <thread>
#include <algorithm>

namespace flappybird {

    Game::Game()
            : is_running_(false),
              last_frame_time_(0),
              window_(nullptr),
              renderer_(nullptr),
              background_texture_(nullptr),
              ground_texture_(nullptr),
              logo_texture_(nullptr),
              game_over_texture_(nullptr),
              scoreboard_texture_(nullptr),
              hint_texture_(nullptr),
              restart_button_texture_(nullptr),
              new_high_score_texture_(nullptr),
              should_update_pipes_(false),
              ground_x_(0) {}

    Game::~Game() {
        Clean();
    }

    bool Game::Init() {
        // Initialize SDL
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
            std::cerr << "SDL could not initialize! SDL Error: "
                      << SDL_GetError() << std::endl;
            return false;
        }

        // Create window
        window_ = SDL_CreateWindow(
                kWindowTitle,
                SDL_WINDOWPOS_CENTERED,
                SDL_WINDOWPOS_CENTERED,
                kWindowWidth,
                kWindowHeight,
                SDL_WINDOW_SHOWN);

        if (!window_) {
            std::cerr << "Window could not be created! SDL Error: "
                      << SDL_GetError() << std::endl;
            return false;
        }

        // Create renderer
        renderer_ = SDL_CreateRenderer(
                window_,
                -1,
                SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

        if (!renderer_) {
            std::cerr << "Renderer could not be created! SDL Error: "
                      << SDL_GetError() << std::endl;
            return false;
        }

        // Initialize renderer color
        SDL_SetRenderDrawColor(renderer_, 0xFF, 0xFF, 0xFF, 0xFF);

        // Initialize resource manager
        resource_manager_ = std::make_unique<ResourceManager>(renderer_);
        if (!resource_manager_->Init()) {
            std::cerr << "Resource manager could not be initialized!" << std::endl;
            return false;
        }

        // Initialize audio manager
        audio_manager_ = std::make_unique<AudioManager>(resource_manager_.get());
        if (!audio_manager_->Init() || !audio_manager_->LoadSounds()) {
            std::cerr << "Audio manager could not be initialized!" << std::endl;
            return false;
        }

        // Initialize game state
        game_state_ = std::make_unique<GameState>();

        // Load textures
        background_texture_ = resource_manager_->LoadTexture("background.png");
        ground_texture_ = resource_manager_->LoadTexture("ground.png");
        logo_texture_ = resource_manager_->LoadTexture("logo.png");
        game_over_texture_ = resource_manager_->LoadTexture("gameover.png");
        scoreboard_texture_ = resource_manager_->LoadTexture("scoreboard.png");
        hint_texture_ = resource_manager_->LoadTexture("hint.png");
        restart_button_texture_ = resource_manager_->LoadTexture("restart.png");
        new_high_score_texture_ = resource_manager_->LoadTexture("highscore.png");

        // Load medal textures
        medal_textures_.push_back(resource_manager_->LoadTexture("bronze_medal.png"));
        medal_textures_.push_back(resource_manager_->LoadTexture("silver_medal.png"));
        medal_textures_.push_back(resource_manager_->LoadTexture("golden_medal.png"));
        medal_textures_.push_back(resource_manager_->LoadTexture("platinum_medal.png"));

        // Create bird
        bird_ = std::make_unique<Bird>(resource_manager_.get(), audio_manager_.get());

        // Create pipes with gradually changing heights
        for (int i = 0; i < kPipeCount; ++i) {
            auto pipe = std::make_unique<Pipe>(resource_manager_.get());
            pipe->Reset(i * kPipeHorizontalDistance);
            pipe->SetVisible(false);  // Initially hidden until the game starts
            pipes_.push_back(std::move(pipe));
        }

        // Initialize threads
        InitThreads();

        return true;
    }

    void Game::Run() {
        is_running_ = true;
        last_frame_time_ = SDL_GetTicks();

        // Main game loop
        while (is_running_) {
            // Calculate delta time
            Uint32 current_time = SDL_GetTicks();
            double delta_time = current_time - last_frame_time_ / 1000.0;
            last_frame_time_ = current_time;

            // Cap delta time to prevent physics issues after pausing
            if (delta_time > 0.05) {
                delta_time = 0.05;
            }

            HandleEvents();
            Update(delta_time);
            Render();

            // Cap framerate
            Uint32 frame_time = SDL_GetTicks() - current_time;
            if (frame_time < kFrameDelay) {
                SDL_Delay(static_cast<Uint32>(kFrameDelay - frame_time));
            }
        }
    }

    void Game::HandleEvents() {
        SDL_Event event;
        bool jump_pressed = false;

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    is_running_ = false;
                    break;

                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_ESCAPE) {
                        is_running_ = false;
                    } else if (event.key.keysym.sym == SDLK_SPACE ||
                               event.key.keysym.sym == SDLK_UP) {
                        jump_pressed = true;
                    } else if (event.key.keysym.sym == SDLK_m) {
                        audio_manager_->ToggleMute();
                    }
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        jump_pressed = true;

                        // Check for restart button click in game over state
                        if (game_state_->IsGameOver()) {
                            int mouse_x = event.button.x;
                            int mouse_y = event.button.y;

                            // Get restart button dimensions
                            int button_width, button_height;
                            SDL_QueryTexture(restart_button_texture_, nullptr, nullptr,
                                             &button_width, &button_height);

                            // Calculate button position
                            int button_x = (kWindowWidth - button_width) / 2;
                            int button_y = kRestartButtonY;

                            // Check if click is within button bounds
                            if (mouse_x >= button_x && mouse_x <= button_x + button_width &&
                                mouse_y >= button_y && mouse_y <= button_y + button_height) {
                                ResetGame();
                            }
                        }
                    }
                    break;
            }
        }

        // Handle bird input
        bird_->HandleInput(jump_pressed);

        // State transitions based on input
        if (jump_pressed) {
            if (game_state_->IsMenu()) {
                game_state_->SetState(GameState::State::kCountdown);
                bird_->SetVisible(true);
            }
        }
    }

    void Game::Update(double delta_time) {
        static double countdown_timer;

        // Update game based on current state
        switch (game_state_->GetState()) {
            case GameState::State::kMenu:
                // Only update bird animation in menu
                bird_->SetStaticAnimation(true);
                bird_->Update(delta_time);
                break;

            case GameState::State::kCountdown:
                // Update countdown timer
                countdown_timer = 0.0;
                countdown_timer += delta_time;

                if (countdown_timer >= 1.0) {
                    countdown_timer = 0.0;
                    if (!game_state_->DecrementCountdown()) {
                        game_state_->SetState(GameState::State::kPlaying);
                        bird_->SetStaticAnimation(false);
                        should_update_pipes_ = true;

                        // Make all pipes visible
                        for (auto &pipe: pipes_) {
                            pipe->SetVisible(true);
                        }
                    }
                }

                bird_->Update(delta_time);
                break;

            case GameState::State::kPlaying:
                // Update bird
                bird_->Update(delta_time);

                // Update pipes
                for (auto &pipe: pipes_) {
                    pipe->Update(delta_time);

                    // If pipe is off-screen, reset it
                    if (pipe->IsOffScreen()) {
                        // Find the rightmost pipe to place this pipe after it
                        double max_x = 0;
                        for (const auto &p: pipes_) {
                            if (p.get() != pipe.get() && p->GetX() > max_x) {
                                max_x = p->GetX();
                            }
                        }
                        // Place this pipe after the rightmost pipe
                        pipe->Reset(static_cast<int>(max_x) + kPipeHorizontalDistance - kWindowWidth);
                    }
                }

                // Check for collisions
                if (CheckCollisions()) {
                    audio_manager_->PlaySound(AudioManager::SoundEffect::kHit);
                    should_update_pipes_ = false;
                    bird_->SetDead(true);
                    game_state_->SetState(GameState::State::kGameOver);
                    game_state_->UpdateHighScore();
                    audio_manager_->PlaySound(AudioManager::SoundEffect::kDie);
                }

                // Check if bird passed any pipes for scoring
                for (auto &pipe: pipes_) {
                    if (pipe->IsPassed(bird_->GetX() + bird_->GetWidth() / 2.0) && !pipe->IsScored()) {
                        pipe->SetScored(true);
                        game_state_->IncrementScore();
                        audio_manager_->PlaySound(AudioManager::SoundEffect::kPoint);
                    }
                }
                break;

            case GameState::State::kGameOver:
                // Only update bird physics in game over state
                bird_->Update(delta_time);
                break;
        }

        // Update ground position for scrolling effect
        // Don't update if bird is dead
        if (!bird_->IsDead()) {
            ground_x_ -= 3;
            if (ground_x_ <= -kWindowWidth) {
                ground_x_ = 0;
            }
        }
    }

    void Game::Render() {
        // Clear screen
        SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
        SDL_RenderClear(renderer_);

        // Use lock_guard to protect rendering operations
        std::lock_guard<std::mutex> lock(render_mutex_);

        // Render background
        SDL_RenderCopy(renderer_, background_texture_, nullptr, nullptr);

        // First render pipes based on the current state
        if (game_state_->IsPlaying() || game_state_->IsGameOver()) {
            // Render pipes
            for (auto &pipe: pipes_) {
                pipe->Render(renderer_);
            }
        }

        // Then render ground (two copies for seamless scrolling)
        SDL_Rect ground_rect1 = {ground_x_, kWindowHeight - kGroundHeight, kWindowWidth, kGroundHeight};
        SDL_Rect ground_rect2 = {ground_x_ + kWindowWidth, kWindowHeight - kGroundHeight, kWindowWidth, kGroundHeight};
        SDL_RenderCopy(renderer_, ground_texture_, nullptr, &ground_rect1);
        SDL_RenderCopy(renderer_, ground_texture_, nullptr, &ground_rect2);

        // Render state-specific elements (excluding pipes which are now rendered above)
        switch (game_state_->GetState()) {
            case GameState::State::kMenu:
                RenderMenu();
                break;

            case GameState::State::kCountdown:
                RenderCountdown();
                break;

            case GameState::State::kPlaying:
                // Skip pipe rendering since we did it above
                bird_->Render(renderer_);
                RenderScore();
                break;

            case GameState::State::kGameOver:
                // Skip pipe rendering since we did it above
                bird_->Render(renderer_);
                RenderGameOver();
                break;
        }

        // Present render
        SDL_RenderPresent(renderer_);
    }

    void Game::RenderMenu() {
        // Render logo
        int logo_width, logo_height;
        SDL_QueryTexture(logo_texture_, nullptr, nullptr, &logo_width, &logo_height);

        SDL_Rect logo_rect = {
                (kWindowWidth - static_cast<int>(logo_width * kLogoScale)) / 2,
                kLogoY,
                static_cast<int>(logo_width * kLogoScale),
                static_cast<int>(logo_height * kLogoScale)
        };

        SDL_RenderCopy(renderer_, logo_texture_, nullptr, &logo_rect);

        // Render hint text
        int hint_width, hint_height;
        SDL_QueryTexture(hint_texture_, nullptr, nullptr, &hint_width, &hint_height);

        SDL_Rect hint_rect = {
                (kWindowWidth - hint_width * 2) / 2,
                kHintY,
                hint_width * 2,
                hint_height * 2
        };

        SDL_RenderCopy(renderer_, hint_texture_, nullptr, &hint_rect);

        // Render bird in menu state
        bird_->Render(renderer_);
    }

    void Game::RenderCountdown() {
        // Render countdown number
        TTF_Font *font = resource_manager_->LoadFont(kFontName, kScoreFontSize);
        if (!font) {
            return;
        }

        // Create text surface
        SDL_Color outline_color = {0, 0, 0, 255}; // Black outline
        SDL_Color text_color = {255, 255, 255, 255}; // White text

        // Display the countdown value directly
        std::string countdown_text = std::to_string(game_state_->GetCountdown());

        // Create outline by rendering text at offsets
        SDL_Surface *outline_surface = TTF_RenderText_Solid(font, countdown_text.c_str(), outline_color);
        if (!outline_surface) {
            return;
        }

        // Create texture from outline surface
        SDL_Texture *outline_texture = SDL_CreateTextureFromSurface(renderer_, outline_surface);
        int text_width = outline_surface->w;
        int text_height = outline_surface->h;
        SDL_FreeSurface(outline_surface);

        if (!outline_texture) {
            return;
        }

        // Render the main text
        SDL_Surface *text_surface = TTF_RenderText_Solid(font, countdown_text.c_str(), text_color);
        if (!text_surface) {
            SDL_DestroyTexture(outline_texture);
            return;
        }

        SDL_Texture *text_texture = SDL_CreateTextureFromSurface(renderer_, text_surface);
        SDL_FreeSurface(text_surface);

        if (!text_texture) {
            SDL_DestroyTexture(outline_texture);
            return;
        }

        // Position for text at the same location as the in-game score
        SDL_Rect text_rect = {
                kScoreX - text_width / 2,
                kScoreY,
                text_width,
                text_height
        };

        // Render the outline (stroke) by drawing the text at offsets
        const int stroke_size = kTextStrokeSize;

        // Top-left
        SDL_Rect outline_rect = {text_rect.x - stroke_size, text_rect.y - stroke_size, text_width, text_height};
        SDL_RenderCopy(renderer_, outline_texture, nullptr, &outline_rect);

        // Top-right
        outline_rect = {text_rect.x + stroke_size, text_rect.y - stroke_size, text_width, text_height};
        SDL_RenderCopy(renderer_, outline_texture, nullptr, &outline_rect);

        // Bottom-left
        outline_rect = {text_rect.x - stroke_size, text_rect.y + stroke_size, text_width, text_height};
        SDL_RenderCopy(renderer_, outline_texture, nullptr, &outline_rect);

        // Bottom-right
        outline_rect = {text_rect.x + stroke_size, text_rect.y + stroke_size, text_width, text_height};
        SDL_RenderCopy(renderer_, outline_texture, nullptr, &outline_rect);

        // Also draw at the four cardinal directions
        outline_rect = {text_rect.x, text_rect.y - stroke_size, text_width, text_height};
        SDL_RenderCopy(renderer_, outline_texture, nullptr, &outline_rect);

        outline_rect = {text_rect.x, text_rect.y + stroke_size, text_width, text_height};
        SDL_RenderCopy(renderer_, outline_texture, nullptr, &outline_rect);

        outline_rect = {text_rect.x - stroke_size, text_rect.y, text_width, text_height};
        SDL_RenderCopy(renderer_, outline_texture, nullptr, &outline_rect);

        outline_rect = {text_rect.x + stroke_size, text_rect.y, text_width, text_height};
        SDL_RenderCopy(renderer_, outline_texture, nullptr, &outline_rect);

        // Finally, render the main text on top
        SDL_RenderCopy(renderer_, text_texture, nullptr, &text_rect);

        // Clean up
        SDL_DestroyTexture(outline_texture);
        SDL_DestroyTexture(text_texture);

        // Render bird in countdown state
        bird_->Render(renderer_);
    }

    void Game::RenderPlaying() {
        // Render pipes
        for (auto &pipe: pipes_) {
            pipe->Render(renderer_);
        }

        // Render bird
        bird_->Render(renderer_);

        // Render score
        RenderScore();
    }

    void Game::RenderGameOver() {
        // Pipes are now rendered in the main Render method

        // Render game over text
        int go_width, go_height;
        SDL_QueryTexture(game_over_texture_, nullptr, nullptr, &go_width, &go_height);

        SDL_Rect go_rect = {
                (kWindowWidth - static_cast<int>(go_width * kGameOverScale)) / 2,
                kGameOverY,
                static_cast<int>(go_width * kGameOverScale),
                static_cast<int>(go_height * kGameOverScale)
        };

        SDL_RenderCopy(renderer_, game_over_texture_, nullptr, &go_rect);

        // Render scoreboard
        int sb_width, sb_height;
        SDL_QueryTexture(scoreboard_texture_, nullptr, nullptr, &sb_width, &sb_height);

        SDL_Rect sb_rect = {
                (kWindowWidth - static_cast<int>(sb_width * kScoreboardScale)) / 2,
                kScoreboardY,
                static_cast<int>(sb_width * kScoreboardScale),
                static_cast<int>(sb_height * kScoreboardScale)
        };

        SDL_RenderCopy(renderer_, scoreboard_texture_, nullptr, &sb_rect);

        // Render medal
        RenderMedal();

        // Render high score badge if a new high score was achieved
        if (game_state_->IsNewHighScore()) {
            RenderNewHighScore();
        }

        // Render scores on scoreboard
        RenderScore();
        RenderHighScore();

        // Render restart button
        int button_width, button_height;
        SDL_QueryTexture(restart_button_texture_, nullptr, nullptr, &button_width, &button_height);

        SDL_Rect button_rect = {
                (kWindowWidth - button_width) / 2,
                kRestartButtonY,
                button_width,
                button_height
        };

        SDL_RenderCopy(renderer_, restart_button_texture_, nullptr, &button_rect);
    }

    void Game::RenderScore() {
        TTF_Font *font = nullptr;
        SDL_Rect text_rect;

        if (game_state_->IsPlaying()) {
            font = resource_manager_->LoadFont(kFontName, kScoreFontSize);
            text_rect = {kScoreX, kScoreY, 0, 0};
        } else if (game_state_->IsGameOver()) {
            font = resource_manager_->LoadFont(kFontName, kScoreboardFontSize);

            // Position on scoreboard
            int sb_width, sb_height;
            SDL_QueryTexture(scoreboard_texture_, nullptr, nullptr, &sb_width, &sb_height);

            int sb_x = (kWindowWidth - static_cast<int>(sb_width * kScoreboardScale)) / 2;
            text_rect = {
                    sb_x + static_cast<int>(kScoreboardCurrentScoreOffsetX * kScoreboardScale),
                    kScoreboardY + static_cast<int>(kScoreboardCurrentScoreOffsetY * kScoreboardScale),
                    0,
                    0
            };
        } else {
            return;
        }

        if (!font) {
            return;
        }

        // Create text surface
        SDL_Color outline_color = {0, 0, 0, 255}; // Black outline
        SDL_Color text_color = {255, 255, 255, 255}; // White text

        std::string score_text = std::to_string(game_state_->GetScore());

        // Create outline surface first
        SDL_Surface *outline_surface = TTF_RenderText_Solid(font, score_text.c_str(), outline_color);
        if (!outline_surface) {
            return;
        }

        // Create outline texture
        SDL_Texture *outline_texture = SDL_CreateTextureFromSurface(renderer_, outline_surface);
        int text_width = outline_surface->w;
        int text_height = outline_surface->h;
        SDL_FreeSurface(outline_surface);

        if (!outline_texture) {
            return;
        }

        // Create main text surface
        SDL_Surface *text_surface = TTF_RenderText_Solid(font, score_text.c_str(), text_color);
        if (!text_surface) {
            SDL_DestroyTexture(outline_texture);
            return;
        }

        // Create texture from surface
        SDL_Texture *text_texture = SDL_CreateTextureFromSurface(renderer_, text_surface);
        SDL_FreeSurface(text_surface);

        if (!text_texture) {
            SDL_DestroyTexture(outline_texture);
            return;
        }

        // Center the text
        text_rect.x -= text_width / 2;
        text_rect.w = text_width;
        text_rect.h = text_height;

        // Render the outline/stroke by drawing the text at offsets
        const int stroke_size = kTextStrokeSize;

        // Diagonal positions
        SDL_Rect outline_rect = {text_rect.x - stroke_size, text_rect.y - stroke_size, text_width, text_height};
        SDL_RenderCopy(renderer_, outline_texture, nullptr, &outline_rect);

        outline_rect = {text_rect.x + stroke_size, text_rect.y - stroke_size, text_width, text_height};
        SDL_RenderCopy(renderer_, outline_texture, nullptr, &outline_rect);

        outline_rect = {text_rect.x - stroke_size, text_rect.y + stroke_size, text_width, text_height};
        SDL_RenderCopy(renderer_, outline_texture, nullptr, &outline_rect);

        outline_rect = {text_rect.x + stroke_size, text_rect.y + stroke_size, text_width, text_height};
        SDL_RenderCopy(renderer_, outline_texture, nullptr, &outline_rect);

        // Cardinal directions
        outline_rect = {text_rect.x, text_rect.y - stroke_size, text_width, text_height};
        SDL_RenderCopy(renderer_, outline_texture, nullptr, &outline_rect);

        outline_rect = {text_rect.x, text_rect.y + stroke_size, text_width, text_height};
        SDL_RenderCopy(renderer_, outline_texture, nullptr, &outline_rect);

        outline_rect = {text_rect.x - stroke_size, text_rect.y, text_width, text_height};
        SDL_RenderCopy(renderer_, outline_texture, nullptr, &outline_rect);

        outline_rect = {text_rect.x + stroke_size, text_rect.y, text_width, text_height};
        SDL_RenderCopy(renderer_, outline_texture, nullptr, &outline_rect);

        // Render the main text on top
        SDL_RenderCopy(renderer_, text_texture, nullptr, &text_rect);

        // Clean up
        SDL_DestroyTexture(outline_texture);
        SDL_DestroyTexture(text_texture);
    }

    void Game::RenderHighScore() {
        if (!game_state_->IsGameOver()) {
            return;
        }

        TTF_Font *font = resource_manager_->LoadFont(kFontName, kScoreboardFontSize);
        if (!font) {
            return;
        }

        // Create text surface
        SDL_Color outline_color = {0, 0, 0, 255}; // Black outline
        SDL_Color text_color = {255, 255, 255, 255}; // White text

        std::string score_text = std::to_string(game_state_->GetHighScore());

        // Create outline surface
        SDL_Surface *outline_surface = TTF_RenderText_Solid(font, score_text.c_str(), outline_color);
        if (!outline_surface) {
            return;
        }

        // Create outline texture
        SDL_Texture *outline_texture = SDL_CreateTextureFromSurface(renderer_, outline_surface);
        int text_width = outline_surface->w;
        int text_height = outline_surface->h;
        SDL_FreeSurface(outline_surface);

        if (!outline_texture) {
            return;
        }

        // Create main text surface
        SDL_Surface *text_surface = TTF_RenderText_Solid(font, score_text.c_str(), text_color);
        if (!text_surface) {
            SDL_DestroyTexture(outline_texture);
            return;
        }

        // Create texture from surface
        SDL_Texture *text_texture = SDL_CreateTextureFromSurface(renderer_, text_surface);
        SDL_FreeSurface(text_surface);

        if (!text_texture) {
            SDL_DestroyTexture(outline_texture);
            return;
        }

        // Position on scoreboard
        int sb_width, sb_height;
        SDL_QueryTexture(scoreboard_texture_, nullptr, nullptr, &sb_width, &sb_height);

        int sb_x = (kWindowWidth - static_cast<int>(sb_width * kScoreboardScale)) / 2;

        // Position for high score
        SDL_Rect text_rect = {
                sb_x + static_cast<int>(kScoreboardHighScoreOffsetX * kScoreboardScale) - text_width / 2,
                kScoreboardY + static_cast<int>(kScoreboardHighScoreOffsetY * kScoreboardScale),
                text_width,
                text_height
        };

        // Render the outline/stroke by drawing the text at offsets
        const int stroke_size = kTextStrokeSize;

        // Diagonal positions
        SDL_Rect outline_rect = {text_rect.x - stroke_size, text_rect.y - stroke_size, text_width, text_height};
        SDL_RenderCopy(renderer_, outline_texture, nullptr, &outline_rect);

        outline_rect = {text_rect.x + stroke_size, text_rect.y - stroke_size, text_width, text_height};
        SDL_RenderCopy(renderer_, outline_texture, nullptr, &outline_rect);

        outline_rect = {text_rect.x - stroke_size, text_rect.y + stroke_size, text_width, text_height};
        SDL_RenderCopy(renderer_, outline_texture, nullptr, &outline_rect);

        outline_rect = {text_rect.x + stroke_size, text_rect.y + stroke_size, text_width, text_height};
        SDL_RenderCopy(renderer_, outline_texture, nullptr, &outline_rect);

        // Cardinal directions
        outline_rect = {text_rect.x, text_rect.y - stroke_size, text_width, text_height};
        SDL_RenderCopy(renderer_, outline_texture, nullptr, &outline_rect);

        outline_rect = {text_rect.x, text_rect.y + stroke_size, text_width, text_height};
        SDL_RenderCopy(renderer_, outline_texture, nullptr, &outline_rect);

        outline_rect = {text_rect.x - stroke_size, text_rect.y, text_width, text_height};
        SDL_RenderCopy(renderer_, outline_texture, nullptr, &outline_rect);

        outline_rect = {text_rect.x + stroke_size, text_rect.y, text_width, text_height};
        SDL_RenderCopy(renderer_, outline_texture, nullptr, &outline_rect);

        // Render the main text on top
        SDL_RenderCopy(renderer_, text_texture, nullptr, &text_rect);

        // Clean up
        SDL_DestroyTexture(outline_texture);
        SDL_DestroyTexture(text_texture);
    }

    void Game::RenderMedal() {
        if (!game_state_->IsGameOver()) {
            return;
        }

        // Determine which medal to show based on score
        int score = game_state_->GetScore();
        int medal_index = 0;  // Bronze by default

        if (score >= kPlatinumMedalScore) {
            medal_index = 3;  // Platinum
        } else if (score >= kGoldMedalScore) {
            medal_index = 2;  // Gold
        } else if (score >= kSilverMedalScore) {
            medal_index = 1;  // Silver
        } else if (score >= kBronzeMedalScore) {
            medal_index = 0;  // Bronze
        } else {
            return;  // No medal
        }

        if (medal_index >= medal_textures_.size() || !medal_textures_[medal_index]) {
            return;
        }

        // Position on scoreboard
        int sb_width, sb_height;
        SDL_QueryTexture(scoreboard_texture_, nullptr, nullptr, &sb_width, &sb_height);

        int sb_x = (kWindowWidth - static_cast<int>(sb_width * kScoreboardScale)) / 2;

        // Get medal dimensions
        int medal_width, medal_height;
        SDL_QueryTexture(medal_textures_[medal_index], nullptr, nullptr, &medal_width, &medal_height);

        // Apply scale factor to medal
        medal_width = static_cast<int>(medal_width * kMedalScale);
        medal_height = static_cast<int>(medal_height * kMedalScale);

        // Position for medal
        SDL_Rect medal_rect = {
                sb_x + static_cast<int>(kMedalOffsetX * kScoreboardScale),
                kScoreboardY + static_cast<int>(kMedalOffsetY * kScoreboardScale),
                medal_width,
                medal_height
        };

        // Render medal
        SDL_RenderCopy(renderer_, medal_textures_[medal_index], nullptr, &medal_rect);
    }

    void Game::RenderNewHighScore() {
        if (!new_high_score_texture_) {
            return;
        }

        // Position on scoreboard
        int sb_width, sb_height;
        SDL_QueryTexture(scoreboard_texture_, nullptr, nullptr, &sb_width, &sb_height);

        int sb_x = (kWindowWidth - static_cast<int>(sb_width * kScoreboardScale)) / 2;

        // Get high score badge dimensions
        int badge_width, badge_height;
        SDL_QueryTexture(new_high_score_texture_, nullptr, nullptr, &badge_width, &badge_height);

        // Position for high score badge (near the high score number)
        SDL_Rect badge_rect = {
                sb_x + static_cast<int>(kHighScoreBadgeOffsetX * kScoreboardScale) - badge_width / 2,
                kScoreboardY + static_cast<int>(kHighScoreBadgeOffsetY * kScoreboardScale),
                badge_width,
                badge_height
        };

        // Render high score badge
        SDL_RenderCopy(renderer_, new_high_score_texture_, nullptr, &badge_rect);
    }

    void Game::ResetGame() {
        // Reset game state
        game_state_->Reset();
        game_state_->SetState(GameState::State::kMenu);

        // Reset bird
        bird_->Reset();
        bird_->SetVisible(true);

        // Reset pipes
        should_update_pipes_ = false;

        for (int i = 0; i < pipes_.size(); ++i) {
            pipes_[i]->Reset(i * kPipeHorizontalDistance);
            pipes_[i]->SetVisible(false);
        }
    }

    void Game::Clean() {
        // Stop threads
        should_update_pipes_ = false;
        if (pipe_update_thread_.joinable()) {
            pipe_update_thread_.join();
        }
        if (physics_update_thread_.joinable()) {
            physics_update_thread_.join();
        }

        // Clean resources
        if (resource_manager_) {
            resource_manager_->Clean();
        }

        // Destroy renderer and window
        if (renderer_) {
            SDL_DestroyRenderer(renderer_);
            renderer_ = nullptr;
        }

        if (window_) {
            SDL_DestroyWindow(window_);
            window_ = nullptr;
        }

        // Quit SDL
        SDL_Quit();
    }

    void Game::InitThreads() {
        // Start physics update thread
        physics_update_thread_ = std::thread(&Game::PhysicsUpdateLoop, this);

        // Start pipe update thread
        pipe_update_thread_ = std::thread(&Game::PipeUpdateLoop, this);
    }

    void Game::PipeUpdateLoop() {
        using namespace std::chrono_literals;

        while (is_running_) {
            // Only update pipes when playing and allowed
            if (should_update_pipes_ && game_state_->GetState() == GameState::State::kPlaying) {
                // Lock rendering operations
                std::lock_guard<std::mutex> lock(render_mutex_);

                // Moving pipes is now handled in the main Update method
                // This thread is now mainly for synchronization and additional updates
            }

            // Sleep to avoid consuming too much CPU
            std::this_thread::sleep_for(16ms);
        }
    }

    void Game::PhysicsUpdateLoop() {
        using namespace std::chrono_literals;

        while (is_running_) {
            // This thread could be used for more complex physics calculations
            // that need to be separated from the main thread
            // For now, it's just a placeholder

            // Sleep to avoid consuming too much CPU
            std::this_thread::sleep_for(16ms);
        }
    }

    bool Game::CheckCollisions() {
        // Check if bird has hit the ground
        if (bird_->GetY() + bird_->GetHeight() >= kWindowHeight - kGroundHeight) {
            return true;
        }

        // Check collisions with pipes
        for (const auto &pipe: pipes_) {
            if (!pipe->IsVisible()) {
                continue;
            }

            // Create custom collision boxes for pipes
            SDL_Rect top_pipe = pipe->GetTopCollisionBox();
            SDL_Rect bottom_pipe = pipe->GetBottomCollisionBox();

            // Get bird collision box
            SDL_Rect bird_box = bird_->GetCollisionBox();

            // Shrink bird collision box slightly for more forgiving collisions
            bird_box.x += kBirdCollisionMarginX;
            bird_box.y += kBirdCollisionMarginY;
            bird_box.w -= kBirdCollisionShrinkWidth;
            bird_box.h -= kBirdCollisionShrinkHeight;

            // Check collision with top pipe
            if (SDL_HasIntersection(&bird_box, &top_pipe)) {
                return true;
            }

            // Check collision with bottom pipe
            if (SDL_HasIntersection(&bird_box, &bottom_pipe)) {
                return true;
            }
        }

        return false;
    }

}  // namespace flappybird 
