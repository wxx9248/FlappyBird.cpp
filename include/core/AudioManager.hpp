#ifndef FLAPPYBIRD_AUDIO_MANAGER_HPP
#define FLAPPYBIRD_AUDIO_MANAGER_HPP

#include <SDL_mixer.h>
#include <string>
#include <unordered_map>

namespace flappybird {

// Forward declarations
    class ResourceManager;

/**
 * @brief Manages audio playback for the game
 */
    class AudioManager {
    public:
        // Sound effect types
        enum class SoundEffect {
            kWing,
            kHit,
            kDie,
            kPoint,
            kMute
        };

        explicit AudioManager(ResourceManager *resource_manager);

        ~AudioManager() = default;

        // Prevent copying
        AudioManager(const AudioManager &) = delete;

        AudioManager &operator=(const AudioManager &) = delete;

        // Initialize the audio system
        static bool Init();

        // Load all sound effects
        bool LoadSounds();

        // Play a sound effect
        void PlaySound(SoundEffect sound);

        // Set global volume (0-100)
        void SetVolume(int volume);

        // Mute/unmute audio
        void SetMuted(bool muted);

        bool IsMuted() const { return is_muted_; }

        // Toggle mute state
        void ToggleMute() { SetMuted(!is_muted_); }

    private:
        ResourceManager *resource_manager_;
        std::unordered_map<SoundEffect, Mix_Chunk *> sounds_;
        int volume_;
        bool is_muted_;
    };

}  // namespace flappybird

#endif  // FLAPPYBIRD_AUDIO_MANAGER_HPP
