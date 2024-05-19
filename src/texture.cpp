#include <SDL2/SDL_image.h>

#include "texture.hpp"

SDL_Texture *loadTexture(SDL_Renderer *renderer, std::string path) {
    SDL_Texture *texture = IMG_LoadTexture(renderer, path.c_str());
    if (texture == nullptr) {
        printf("IMG_LoadTexture error: %s\n", IMG_GetError());
    }
    return texture;
}
