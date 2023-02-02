#include "core/AudioManager.hpp"
#include "core/ResourceManager.hpp"

#include <iostream>

namespace flappybird {

    AudioManager::AudioManager(ResourceManager *resource_manager)
            : resource_manager_(resource_manager),
              volume_(MIX_MAX_VOLUME),
              is_muted_(false) {}

    bool AudioManager::Init() {
        // Initialize SDL_mixer
        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
            std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: "
                      << Mix_GetError() << std::endl;
            return false;
        }

        return true;
    }

    bool AudioManager::LoadSounds() {
        // Load sound files
        Mix_Chunk *wing = resource_manager_->LoadSound("wing.wav");
        Mix_Chunk *hit = resource_manager_->LoadSound("hit.wav");
        Mix_Chunk *die = resource_manager_->LoadSound("die.wav");
        Mix_Chunk *point = resource_manager_->LoadSound("point.wav");
        Mix_Chunk *mute = resource_manager_->LoadSound("mute.wav");

        // Store in the map
        sounds_[SoundEffect::kWing] = wing;
        sounds_[SoundEffect::kHit] = hit;
        sounds_[SoundEffect::kDie] = die;
        sounds_[SoundEffect::kPoint] = point;
        sounds_[SoundEffect::kMute] = mute;

        // Check if all sounds were loaded correctly
        return wing && hit && die && point && mute;
    }

    void AudioManager::PlaySound(SoundEffect sound) {
        if (is_muted_) {
            return;
        }

        auto it = sounds_.find(sound);
        if (it != sounds_.end() && it->second) {
            Mix_PlayChannel(-1, it->second, 0);
        }
    }

    void AudioManager::SetVolume(int volume) {
        // Ensure volume is within range
        volume_ = std::max(0, std::min(MIX_MAX_VOLUME, volume));

        // Apply to all loaded sounds
        for (auto &[type, sound]: sounds_) {
            if (sound) {
                Mix_VolumeChunk(sound, volume_);
            }
        }
    }

    void AudioManager::SetMuted(bool muted) {
        is_muted_ = muted;

        // If unmuting, restore volume; if muting, set volume to 0
        for (auto &[type, sound]: sounds_) {
            if (sound) {
                Mix_VolumeChunk(sound, is_muted_ ? 0 : volume_);
            }
        }

        // Play the mute sound effect when toggling
        if (is_muted_) {
            // Force play the mute sound even when muted
            auto it = sounds_.find(SoundEffect::kMute);
            if (it != sounds_.end() && it->second) {
                Mix_VolumeChunk(it->second, volume_);
                Mix_PlayChannel(-1, it->second, 0);
                Mix_VolumeChunk(it->second, 0);
            }
        }
    }

}  // namespace flappybird 