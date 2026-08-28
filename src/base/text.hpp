#pragma once

#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include "font.hpp"
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

    Text(const Font &font);
    Text(const Font &font, SDL_Color colorVal);

    ~Text();

    [[nodiscard]] bool isValid() const;
    void setText(const std::string &text);
    void renderText(SDL_FPoint location, SDL_Renderer *sdl_renderer);
    [[nodiscard]] Texture &getTexture();

  private:
    const Font &font;
    SDL_Color color{.r = 0, .g = 0, .b = 0, .a = 0xff};
    Texture textTexture;
    std::string currText;
    bool changed{false};
};