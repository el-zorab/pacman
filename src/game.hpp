#pragma once

#include <array>
#include <memory>
#include <SDL2/SDL.h>
#include <string>

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

    FontRenderer &getFontRenderer();
    TextureManager &getTextureManager();
    TilingManager &getTilingManager();

    Pacman &getPacman();
    Ghost &getBlinky();

    void handleEvents();
    void update();
    void render();

private:
    SDL_Window *window;
    int windowWidth;
    int windowHeight;
    SDL_Renderer *renderer;
    bool gameRunning;

    std::unique_ptr<FontRenderer> fontRenderer;
    std::unique_ptr<TextureManager> textureManager;
    std::unique_ptr<TilingManager> tilingManager;
    std::unique_ptr<PelletManager> pelletManager;

    std::unique_ptr<Pacman> pacman;
    
    enum GhostName : int {
        BLINKY = 0,
        PINKY = 1,
        INKY = 2,
        CLYDE = 3
    };

    static const int GHOST_COUNT = 4;

    std::array<std::unique_ptr<Ghost>, GHOST_COUNT> ghosts; 

    double frameAccumulator;
    std::unique_ptr<Timer> frameTimer;

    void renderBackground();
};
