#pragma once

#include <array>
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

    bool isGameRunning();
    void stopRunning();

    SDL_Renderer *getRenderer();

    TextureManager &getTextureManager();
    TilingManager &getTilingManager();

    Pacman &getPacman();
    Ghost &getBlinky();

    void renderText(std::string text, int x, int y, SDL_Color color);
    
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
    bool gameRunning;

    Entity2D mousePos;

    std::unique_ptr<FontRenderer> fontRenderer;
    std::unique_ptr<TextureManager> textureManager;
    std::unique_ptr<TilingManager> tilingManager;
    std::unique_ptr<PelletManager> pelletManager;

    std::unique_ptr<Pacman> pacman;

    static constexpr SDL_Color BACKGROUND_COLOR = { 0, 0, 0, 255 };
    static constexpr SDL_Color TEXT_COLOR = { 255, 255, 255, 255 };

    static constexpr SDL_Color TILE_COLOR_A = { 15, 15, 15, 255 };
    static constexpr SDL_Color TILE_COLOR_B = { 25, 25, 25, 255 };

    static constexpr Entity2D GHOST_HOUSE_TILE = { 13, 15 };
    static constexpr int GHOST_HOUSE_GATE_WIDTH  = 2 * GameConst::TILE_SIZE;
    static constexpr int GHOST_HOUSE_GATE_HEIGHT = 6;
    static constexpr SDL_Color GHOST_HOUSE_GATE_COLOR = { 255, 127, 255, 255 };

    static constexpr int EATEN_PELLETS_PINKY_EXIT = 0;
    static constexpr int EATEN_PELLETS_INKY_EXIT  = 30;
    static constexpr int EATEN_PELLETS_CLYDE_EXIT = 60;

    std::array<int, Ghost::GHOST_COUNT> ghostsExitMinEatenPellets;

    static constexpr int GHOST_FRIGHTENED_MS = 25000;
    static constexpr int GHOST_START_FLASHING_MS = 4000;
    static constexpr int GHOST_FLASHING_INTERVAL_MS = 250;

    static constexpr SDL_Color GHOST_FRIGHTENED_COLOR = { 63, 255, 63, 255 };
    static constexpr SDL_Color GHOST_FLASH_COLOR = { 255, 63, 63, 255 };
    static constexpr SDL_Color GHOST_EATEN_COLOR = { 255, 255, 255, 255 };

    std::array<std::unique_ptr<Ghost>, Ghost::GHOST_COUNT> ghosts;

    struct GhostModeEntry {
        Ghost::Mode mode;
        int duration;
    };

    std::vector<GhostModeEntry> ghostModes;
    int currModeIndex;
    std::unique_ptr<Timer> currModeTimer;

    bool ghostsFrightened;
    std::unique_ptr<Timer> ghostsFrightenedTimer;

    bool ghostsFlashing;
    std::unique_ptr<Timer> ghostsFlashTimer;

    double frameTimeAccumulator;
    std::unique_ptr<Timer> frameTimer;

    void updatePhysicsFrame(int const deltaTime);
    void renderBackground();
    void renderGhostHouseGate();
};
