#include <SDL2/SDL_image.h>

#include "textureLoader.hpp"

SDL_Texture *loadTexture(SDL_Renderer *renderer, std::string path, bool exitOnError) {
    SDL_Texture *texture = IMG_LoadTexture(renderer, path.c_str());
    if (texture == nullptr) {
        printf("IMG_LoadTexture error: %s\n", IMG_GetError());
        if (exitOnError) exit(-1);
    }
    return texture;
}
