#pragma once

#include <cstdint>
#include <sstream>
#include "../base/font.hpp"
#include "../base/text.hpp"
#include "object_base.hpp"

class Timer : public ObjectBase
{
  private:
    uint64_t timer{0};
    std::stringstream time_text;
    Text text;
    SDL_FPoint position{.x = 10.F, .y = 10.F};

  public:
    Timer(Font &font) : timer(SDL_GetTicks()), text(font) {};
    Timer(Font &font, SDL_FPoint pos) : timer(SDL_GetTicks()), text(font), position(pos) {};

    Texture &getTexture() override
    {
        return text.getTexture();
    }

    void handleInput(const SDL_Event &input) override;
    void update(float delta_t) override;
    void render(SDL_Renderer *renderer) override;
};
