#pragma once

#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include "texture.hpp"

class Text
{
  public:
    // copy
    Text(const Text &texture) = delete;
    Text &operator=(const Text &texture) = delete;

    // move
    Text(Text &&texture) = delete;
    Text &operator=(Text &&texture) = delete;

    Text(const std::string &fontPath, float fontSize);
    Text(const std::string &fontPath, float fontSize, SDL_Color colorVal);

    ~Text();

    [[nodiscard]] bool isValid() const;
    bool loadText(const std::string &text, SDL_Renderer *sdl_renderer);
    void renderText(SDL_FPoint location, SDL_Renderer *sdl_renderer);
    [[nodiscard]] Texture &getTexture();

  private:
    TTF_Font *font{nullptr};
    bool valid{false};
    SDL_Color color;
    Texture textTexture;
};