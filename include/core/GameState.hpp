#ifndef FLAPPYBIRD_GAME_STATE_HPP
#define FLAPPYBIRD_GAME_STATE_HPP

namespace flappybird {

/**
 * @brief Manages the current state of the game
 */
    class GameState {
    public:
        // Game states
        enum class State {
            kMenu,        // Main menu
            kCountdown,   // Countdown before game starts
            kPlaying,     // Game in progress
            kGameOver     // Game over screen
        };

        GameState();

        ~GameState() = default;

        // Get the current state
        [[nodiscard]] State GetState() const { return current_state_; }

        // Transition to a new state
        void SetState(State state);

        // Helper methods for state checking
        [[nodiscard]] bool IsMenu() const { return current_state_ == State::kMenu; }

        [[nodiscard]] bool IsCountdown() const { return current_state_ == State::kCountdown; }

        [[nodiscard]] bool IsPlaying() const { return current_state_ == State::kPlaying; }

        [[nodiscard]] bool IsGameOver() const { return current_state_ == State::kGameOver; }

        // Game state data
        [[nodiscard]] int GetScore() const { return score_; }

        void SetScore(int score) { score_ = score; }

        void IncrementScore() { ++score_; }

        [[nodiscard]] int GetHighScore() const { return high_score_; }

        // Check if a new high score was achieved
        [[nodiscard]] bool IsNewHighScore() const { return is_new_high_score_; }

        // Update high score if current score is higher
        void UpdateHighScore();

        // Reset the game state for a new game
        void Reset();

        // Get the countdown value (3, 2, 1)
        [[nodiscard]] int GetCountdown() const { return countdown_; }

        // Decrement the countdown
        bool DecrementCountdown();

    private:
        State current_state_;
        int score_;
        int high_score_;
        int countdown_;
        bool is_new_high_score_;
    };

}  // namespace flappybird

#endif  // FLAPPYBIRD_GAME_STATE_HPP
