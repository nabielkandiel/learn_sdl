#pragma once

#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include "font.hpp"
#include "texture.hpp"

class Text
{
  public:
    // copy
    Text(const Text &text) = delete;
    Text &operator=(const Text &text) = delete;

    // move
    Text(Text &&text) = delete;
    Text &operator=(Text &&tex) = delete;

    Text(Font &font);
    Text(Font &font, SDL_Color colorVal);

    ~Text();

    [[nodiscard]] bool isValid() const;
    void setText(const std::string &text);
    void renderText(SDL_FPoint location, SDL_Renderer *sdl_renderer);
    [[nodiscard]] Texture &getTexture();

  private:
    Font *font;
    SDL_Color color{.r = 0, .g = 0, .b = 0, .a = 0xff};
    Texture textTexture;
    std::string currText;
    bool changed{false};
};