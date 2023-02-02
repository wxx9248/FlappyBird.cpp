#ifndef FLAPPYBIRD_CONSTANTS_HPP
#define FLAPPYBIRD_CONSTANTS_HPP

#include <string>

namespace flappybird {

// Window settings
    constexpr int kWindowWidth = 768;
    constexpr int kWindowHeight = 1024;
    constexpr const char *kWindowTitle = "Flappy Bird";

// Game settings
    constexpr int kFps = 60;
    constexpr double kFrameDelay = 1000.0 / kFps;

// Bird settings
    constexpr int kBirdDefaultX = 200;
    constexpr int kBirdDefaultY = 450;
    constexpr double kBirdWingFlapPeriod = 0.25;  // in seconds
    constexpr double kBirdFluctuationPeriod = 1.0;  // in seconds
    constexpr double kBirdFluctuationAmplitude = 2.0;
    constexpr double kDefaultGravity = 0.5;
    constexpr double kJumpVelocity = -10.0;

    // Bird collision settings
    constexpr int kBirdCollisionMarginX = 5;      // Pixels to shrink collision box horizontally
    constexpr int kBirdCollisionMarginY = 5;      // Pixels to shrink collision box vertically
    constexpr int kBirdCollisionShrinkWidth = 10; // Total pixels to shrink collision width
    constexpr int kBirdCollisionShrinkHeight = 10; // Total pixels to shrink collision height

// Pipe settings
    constexpr int kPipeCount = 5;  // Only need a few pipes that are recycled
    constexpr int kPipeGap = 220;
    constexpr int kPipeHorizontalDistance = 300;
    constexpr int kPipeSpeed = 3;
    constexpr int kPipeTopSafetyMargin = 100;     // Minimum distance from top for pipe gap
    constexpr int kPipeGroundSafetyMargin = 100;  // Minimum distance from ground for pipe gap

// Ground settings
    constexpr int kGroundHeight = 112;            // Height of the ground texture

// Asset paths
    const std::string kAssetPath = "assets/";
    const std::string kImagesPath = kAssetPath + "images/";
    const std::string kSoundsPath = kAssetPath + "sounds/";
    const std::string kFontsPath = kAssetPath + "fonts/";

// Font settings
    const std::string kFontName = "font.ttf";
    constexpr int kScoreFontSize = 72;
    constexpr int kScoreboardFontSize = 48;
    constexpr int kHintFontSize = 32;
    constexpr int kTextStrokeSize = 5;            // Stroke size for text

// UI settings
    constexpr int kLogoY = 180;
    constexpr double kLogoScale = 2.5;
    constexpr int kGameOverY = 180;
    constexpr double kGameOverScale = 2.5;
    constexpr int kScoreboardY = 380;
    constexpr double kScoreboardScale = 2.0;
    constexpr int kHintY = 600;
    constexpr int kScoreX = 365;
    constexpr int kScoreY = 100;
    constexpr int kRestartButtonY = 700;

// Scoreboard elements positioning
    constexpr int kScoreboardCurrentScoreOffsetX = 170;  // X offset from scoreboard left for current score
    constexpr int kScoreboardCurrentScoreOffsetY = 33;   // Y offset from scoreboard top for current score
    constexpr int kScoreboardHighScoreOffsetX = 170;     // X offset from scoreboard left for high score
    constexpr int kScoreboardHighScoreOffsetY = 75;     // Y offset from scoreboard top for high score

// Medal positioning and scaling
    constexpr int kMedalOffsetX = 26;         // X offset from scoreboard left for medal
    constexpr int kMedalOffsetY = 43;         // Y offset from scoreboard top for medal
    constexpr double kMedalScale = 2;       // Scale factor for medal to make it larger

// Medal score thresholds
    constexpr int kBronzeMedalScore = 10;     // Score needed for bronze medal
    constexpr int kSilverMedalScore = 20;     // Score needed for silver medal
    constexpr int kGoldMedalScore = 30;       // Score needed for gold medal
    constexpr int kPlatinumMedalScore = 40;   // Score needed for platinum medal

// High score badge positioning
    constexpr int kHighScoreBadgeOffsetX = 205;  // X offset from scoreboard left for high score badge
    constexpr int kHighScoreBadgeOffsetY = 57;   // Y offset from scoreboard top for high score badge

}  // namespace flappybird

#endif  // FLAPPYBIRD_CONSTANTS_HPP
