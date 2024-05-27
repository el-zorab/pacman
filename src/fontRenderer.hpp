#pragma once

#include <SDL2/SDL_ttf.h>
#include <string>

class FontRenderer {

public:
    FontRenderer(std::string filename, int ptsize, bool exitOnError = true);
    void close();

    void renderText(SDL_Renderer *renderer, std::string text, int x, int y, SDL_Color color);
    
private:
    TTF_Font *font;

};
