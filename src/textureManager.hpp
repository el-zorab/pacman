#pragma once

#include <map>
#include <SDL2/SDL.h>
#include <string>

class TextureManager {

public:
    TextureManager();
    void close();

    SDL_Texture *loadTexture(std::string filename, bool exitOnError = true);

private:    
    std::map<std::string, SDL_Texture*> textures;

};
