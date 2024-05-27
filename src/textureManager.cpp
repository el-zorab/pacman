#include <memory>
#include <SDL2/SDL_image.h>

#include "textureManager.hpp"

const std::string RESOURCE_DIRECTORY = "res/";

TextureManager::TextureManager(){}

SDL_Texture *TextureManager::loadTexture(SDL_Renderer *renderer, std::string filename, bool exitOnError /* = true */) {
    std::string path = RESOURCE_DIRECTORY + filename;
    SDL_Texture *texture = IMG_LoadTexture(renderer, path.c_str());
    if (texture == nullptr) {
        printf("IMG_LoadTexture error: %s\n", IMG_GetError());
        if (exitOnError) exit(-1);
    }
    textures[path] = texture;
    return texture;
}

void TextureManager::close() {
    for (auto &x : textures) {
        SDL_DestroyTexture(x.second);
        x.second = nullptr;
        SDL_Log("SDL_DestroyTexture(%s)\n", x.first.c_str());
    }
}
