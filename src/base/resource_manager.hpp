#pragma once

#include <unordered_map>

#include "base/texture.hpp"
#include "common/CommonEnums.hpp"

class ResourceManager
{

  public:
    ResourceManager();

    ResourceManager(const ResourceManager &) = delete;
    ResourceManager(ResourceManager &&) = delete;
    ResourceManager &operator=(const ResourceManager &) = delete;
    ResourceManager &operator=(ResourceManager &&) = delete;

    ~ResourceManager();

    std::string_view getFontPath(Fonts font_name)
    {
        return font_map[font_name];
    }

    Texture &getTexture(Textures text)
    {
        return texture_map[text];
    }

    void loadTextures(SDL_Renderer *renderer);

  private:
    std::unordered_map<Textures, Texture> texture_map;
    std::unordered_map<Fonts, std::string> font_map;
};