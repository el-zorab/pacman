#pragma once

#include <memory>
#include <SDL2/SDL.h>
#include <string>

#include "fontRenderer.hpp"
#include "pacman.hpp"
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

    FontRenderer &getFontRenderer();
    TextureManager &getTextureManager();
    TilingManager &getTilingManager();

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

    std::unique_ptr<Pacman> pacman;

    std::unique_ptr<Timer> frameTimer;

    void renderBackground();
    void renderMap();
};
