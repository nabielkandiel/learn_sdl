#pragma once

#include "base/font.hpp"
#include "base/game_context.hpp"
#include "base/object_base.hpp"
#include "base/text.hpp"

class Label : public ObjectBase
{
  private:
    Text text;
    SDL_FPoint position{.x = 10.F, .y = 10.F};

  public:
    Label(const Label &) = delete;
    Label(Label &&) noexcept = delete;
    Label &operator=(const Label &) = delete;
    Label &operator=(Label &&) noexcept = delete;

    Label(GameContext &context, Font &font) : Label(context, font, {.x = 0.F, .y = 0.F}) {};
    Label(GameContext &context, Font &font, SDL_FPoint pos);
    ~Label() override = default;

    void update(float delta_t) override;
    void render(SDL_Renderer *renderer) override;
    void setText(const std::string &txt)
    {
        text.setText(txt);
    }
};
