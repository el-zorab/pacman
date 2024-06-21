#pragma once

#include <array>
#include <fstream>
#include <memory>
#include <SDL2/SDL.h>
#include <string>
#include <vector>

#include "fontRenderer.hpp"
#include "ghost.hpp"
#include "pacman.hpp"
#include "pelletManager.hpp"
#include "textureManager.hpp"
#include "tilingManager.hpp"
#include "timer.hpp"

class Game {

    struct _ctr_tag { explicit _ctr_tag() = default; };

public:
    Game(_ctr_tag);
    static Game &getInstance();

    void init(std::string title, int x, int y);
    void close();

    bool shouldQuit();

    SDL_Renderer *getRenderer();

    TextureManager &getTextureManager();
    TilingManager &getTilingManager();

    Pacman &getPacman();
    Ghost &getBlinky();
    
    void handleEvents();
    void update();
    void render();

private:
    static constexpr int PHYSICS_FRAMERATE = 60;
    static constexpr int PHYSICS_FRAME_DELTA_TIME = static_cast<int>(1000.f / PHYSICS_FRAMERATE);

    SDL_Window *window;
    int windowWidth;
    int windowHeight;
    SDL_Renderer *renderer;

    enum class GameState {
        WAITING_FOR_START,
        RUNNING,
        WIN_LEVEL,
        LOST_LIFE,
        LOST_GAME,
        QUIT
    };
    GameState gameState;
    int gameLevel;
    bool gameStateWaiting;
    int gameStateWaitMs;
    std::unique_ptr<Timer> gameStateWaitTimer;

    int remainingLives;
    int gameScore;
    int gameHighscore;
    std::ifstream gameHighscoreFileIn;
    std::ofstream gameHighscoreFileOut;

    static int constexpr GAME_LIVES = 3;

    static int constexpr GAME_SCORE_PELLET = 10;
    static int constexpr GAME_SCORE_ENERGIZER = 50;

    SDL_Texture *waitingForStartTexture;
    SDL_Texture *gameLostTexture;

    std::unique_ptr<FontRenderer> fontRenderer;
    std::unique_ptr<TextureManager> textureManager;
    std::unique_ptr<TilingManager> tilingManager;
    std::unique_ptr<PelletManager> pelletManager;

    std::unique_ptr<Pacman> pacman;

    static constexpr SDL_Color BACKGROUND_COLOR = { 0, 0, 0, 255 };
    static constexpr SDL_Color TEXT_COLOR = { 255, 255, 255, 255 };

    static constexpr SDL_Color TILE_COLOR_A = { 15, 15, 15, 255 };
    static constexpr SDL_Color TILE_COLOR_B = { 25, 25, 25, 255 };

    static constexpr int GHOST_HOUSE_HEIGHT = 6;
    static constexpr SDL_Rect GHOST_HOUSE_GATE_RECT = { 13 * GameConst::TILE_SIZE, 15 * GameConst::TILE_SIZE + GameConst::TILE_SIZE / 2 - GHOST_HOUSE_HEIGHT / 2, 2 * GameConst::TILE_SIZE, GHOST_HOUSE_HEIGHT };
    static constexpr SDL_Color GHOST_HOUSE_GATE_COLOR = { 255, 127, 255, 255 };

    std::array<int, Ghost::GHOST_COUNT> ghostsExitMinEatenPellets;

    static constexpr int GHOST_FRIGHTENED_MS = 6000;
    static constexpr int GHOST_START_FLASHING_MS = 4500;
    static constexpr int GHOST_FLASHING_INTERVAL_MS = 250;

    std::array<std::unique_ptr<Ghost>, Ghost::GHOST_COUNT> ghosts;

    struct GhostModeEntry {
        Ghost::Mode mode;
        int duration;
    };

    std::vector<GhostModeEntry> ghostModes;
    int ghostModeCurr;
    std::unique_ptr<Timer> currModeTimer;

    bool ghostsFrightened;
    std::unique_ptr<Timer> ghostsFrightenedTimer;
    
    static int constexpr GHOST_EATEN_POINTS_BASE = 200;
    static int constexpr GHOST_EATEN_POINTS_MUL  = 2;
    int ghostEatenPoints;

    std::unique_ptr<Timer> ghostsFlashingTimer;

    double frameTimeAccumulator;
    std::unique_ptr<Timer> frameTimer;

    void startLevel();
    void startNewLevel();
    void startGame();
    void updateFrame();
    void updateRunningFrame(int const deltaTime);
    void renderText(std::string text, int x, int y, SDL_Color color);
    void renderClear();
    void renderGhostHouseGate();
    void renderRemainingLives();
    void renderText();
    void renderMap();
};
