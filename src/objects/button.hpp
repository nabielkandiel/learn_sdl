#pragma once

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_rect.h>
#include <functional>
#include <utility>
#include "base/game_context.hpp"
#include "base/sprites.hpp"

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
        ACTIVE = 1,
        COUNT = 2,
    };

    ButtonState state{ButtonState::NONE};
    SDL_FPoint position{.x = 0.0, .y = 0.0};
    Sprites<ButtonSprite> sprite;
    std::function<void(void)> on_press;

    static constexpr size_t TOTAL_DIRS = static_cast<size_t>(ButtonSprite::COUNT);

    bool isInside();
    void setupSprite(float spr_w, float spr_h)
    {
        sprite.sliceTextureHorizontal(spr_w, spr_h);
        sprite.setActiveDir(ButtonSprite::NORMAL);
    }

  public:
    Button(GameContext &context)
        : sprite(context.getTexture(GameContext::TEXTURES::RESET_BUTTON), {ButtonSprite::NORMAL, ButtonSprite::ACTIVE})
    {
        auto &input_manager = context.getInputManager();
        input_manager.bindMouseEvent(SDL_EVENT_MOUSE_MOTION, [this]() { handleMouseMotion(); });
        input_manager.bindMouseEvent(SDL_EVENT_MOUSE_BUTTON_DOWN, [this]() { handleMouseButtonDown(); });
        input_manager.bindMouseEvent(SDL_EVENT_MOUSE_BUTTON_UP, [this]() { handleMouseButtonUp(); });

        context.getEntityManager().registerEntity(*this);

        setupSprite(64.0F, 64.0F);
        position.x = (static_cast<float>(context.getScreenWidth()) - (sprite.getActiveRect().w / 2.F));
        position.y = (static_cast<float>(context.getScreenHeight()) - (sprite.getActiveRect().h / 2.F));
    }

    void setOnPress(std::function<void(void)> callback)
    {
        on_press = std::move(callback);
    }

    void render(SDL_Renderer *render) override
    {
        sprite.renderActive(render, position);
    }

    void update(float /*delat_t*/) override {}

    [[nodiscard]] Texture &getTexture()
    {
        return sprite.getTexture();
    }

    [[nodiscard]] const SDL_FRect &getActiveRect()
    {
        return sprite.getActiveRect();
    }

    void handleMouseMotion();
    void handleMouseButtonDown();
    void handleMouseButtonUp();
};