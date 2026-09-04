
#include "resource_manager.hpp"
#include "base/utility.h"

ResourceManager::ResourceManager()
{

    font_map[Fonts::MONTSERRAT] = "Montserrat/Montserrat-VariableFont_wght.ttf";
}

ResourceManager::~ResourceManager()
{
    for (auto &[key, texture] : texture_map) {
        texture.destroy();
    }
}

void ResourceManager::loadTextures(SDL_Renderer *renderer)
{
    {
        auto &texture = texture_map[Textures::BALL];
        texture.setColorKey(SDL_Color{.r = 0, .g = 180, .b = 180, .a = 0});
        if (!loadAsset(texture, renderer, "balls.png")) {
            SDL_Log("failed to load balls.png\n");
        }
    }
    {
        auto &texture = texture_map[Textures::ARROW];
        if (!loadAsset(texture, renderer, "arrow.png")) {
            SDL_Log("failed to load arrow.png\n");
        }
    }
    {
        auto &texture = texture_map[Textures::RESET_BUTTON];
        if (!loadAsset(texture, renderer, "reset_bttn.png")) {
            SDL_Log("failed to load reset_bttn.png \n");
        }
    }
}