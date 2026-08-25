#pragma once

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_rect.h>
#include "../sprites.hpp"
#include "object_base.hpp"

class Button : public ObjectBase
{
  private:
    enum class ButtonState : Uint8
    {
        NONE = 0,
        MouseOut = 1,
        MouseOver = 2,
        MouseDown = 3,
        MouseUp = 4,
    };

    enum class ButtonSprite : uint8_t
    {
        NORMAL = 0,
        CLICKED = 1,
        COUNT = 2,
    };

    ButtonState state{ButtonState::NONE};
    SDL_FPoint position{.x = 0.0, .y = 0.0};
    Sprites<ButtonSprite> sprite;

    static constexpr size_t TOTAL_DIRS = static_cast<size_t>(ButtonSprite::COUNT);

  public:
    Button(std::optional<SDL_Color> color = std::nullopt)
        : sprite({ButtonSprite::NORMAL, ButtonSprite::CLICKED}, color.value_or(SDL_Color{}))
    {
    }

    void setPosition(SDL_FPoint pos)
    {
        position = pos;
    }

    void setupSprite(float spr_w, float spr_h)
    {
        sprite.sliceTextureHorizontal(spr_w, spr_h);
        sprite.setActiveDir(ButtonSprite::NORMAL);
    }

    void update(float delta_t) override
    {
        (void)delta_t;
    }

    void render(SDL_Renderer *render) override
    {
        sprite.renderActive(render, position);
    }

    [[nodiscard]] Texture &getTexture() override
    {
        return sprite.getTexture();
    }

    [[nodiscard]] const SDL_FRect &getActiveRect()
    {
        return sprite.getActiveRect();
    }

    void handleUpdate(const SDL_Event &event) override;
};