#include "core/GameState.hpp"

namespace flappybird {

    GameState::GameState()
            : current_state_(State::kMenu),
              score_(0),
              high_score_(0),
              countdown_(3),
              is_new_high_score_(false) {}

    void GameState::SetState(State state) {
        current_state_ = state;

        // Reset countdown when entering countdown state
        if (state == State::kCountdown) {
            countdown_ = 3;
        }
    }

    void GameState::UpdateHighScore() {
        if (score_ > high_score_) {
            high_score_ = score_;
            is_new_high_score_ = true;
        } else {
            is_new_high_score_ = false;
        }
    }

    void GameState::Reset() {
        score_ = 0;
        countdown_ = 3;
        is_new_high_score_ = false;
        current_state_ = State::kMenu;
    }

    bool GameState::DecrementCountdown() {
        if (countdown_ > 0) {
            --countdown_;
            return true;
        }
        return false;
    }

}  // namespace flappybird 