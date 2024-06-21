#include "fontRenderer.hpp"

const std::string RESOURCE_DIRECTORY = "res/";

FontRenderer::FontRenderer(std::string filename, int ptsize, bool exitOnError /* = true */) {
    std::string path = RESOURCE_DIRECTORY + filename;
    font = TTF_OpenFont(path.c_str(), ptsize);
    if (font == nullptr) {
        printf("TTF_OpenFont error: %s\n", TTF_GetError());
        if (exitOnError) exit(-1);
    }
}

void FontRenderer::close() {
    TTF_CloseFont(font);
    font = nullptr;
}

TTF_Font *FontRenderer::getFont() {
    return font;
}

void FontRenderer::renderText(SDL_Renderer *renderer, std::string text, int x, int y, SDL_Color color) {
    SDL_Surface *surface = TTF_RenderText_Blended(font, text.c_str(), color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect rect = { x, y, surface->w, surface->h };
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}
