#pragma once

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_rect.h>
#include <functional>
#include <memory>
#include <optional>
#include <utility>

#include "base/game_context.hpp"
#include "base/object_base.hpp"
#include "base/sprites.hpp"
#include "base/text.hpp"
#include "common/CommonEnums.hpp"

struct ButtonStyle
{
    SDL_Color normal{.r = 80, .g = 80, .b = 80, .a = 255};
    SDL_Color hover{.r = 120, .g = 120, .b = 120, .a = 255};
    SDL_Color pressed{.r = 50, .g = 50, .b = 50, .a = 255};
    SDL_Color colActive{.r = 46, .g = 178, .b = 120, .a = 255}; // toggle ON
    SDL_Color colDisabled{.r = 170, .g = 174, .b = 180, .a = 255};
    std::optional<SDL_Color> border{
        SDL_Color{.r = 200, .g = 200, .b = 200, .a = 255}};
    float cornerRadius = 10.0F;
    float iconSize = 24.0F;
    float iconGap = 10.0F;
};

class Button : public ObjectBase
{
  public:
    enum class ButtonState : uint8_t
    {
        NORMAL = 0,
        MOUSE_OVER = 1,
        MOUSE_DOWN = 2,
    };

    enum class ButtonKind : uint8_t
    {
        PUSH = 0,
        TOGGLE = 1,
    };

    Button(const Button &) = delete;
    Button(Button &&) = delete;
    Button &operator=(const Button &) = delete;
    Button &operator=(Button &&) = delete;

    // Background-only button
    Button(GameContext &context, const SDL_FRect &bounds, ButtonKind kind,
           ButtonStyle style = {},
           InputState input_state = InputState::PLAYING);

    // Text-labeled button
    Button(GameContext &context, const SDL_FRect &bounds, ButtonKind kind,
           Font &font, const std::string &label,
           SDL_Color text_color = {.r = 255, .g = 255, .b = 255, .a = 255},
           ButtonStyle style = {},
           InputState input_state = InputState::PLAYING);

    // Sprite-icon button (horizontal sprite sheet: NORMAL frame left, ACTIVE
    // right)
    Button(GameContext &context, const SDL_FRect &bounds, ButtonKind kind,
           Texture &texture, float spr_w, float spr_h, ButtonStyle style = {},
           InputState input_state = InputState::PLAYING);

    ~Button() override;

    void setOnPress(std::function<void(void)> callback)
    {
        on_press = std::move(callback);
    }

    void setOnToggle(std::function<void(bool)> callback)
    {
        on_toggle = std::move(callback);
    }

    [[nodiscard]] bool isToggled() const
    {
        return toggled;
    }
    void setToggled(bool toggle)
    {
        toggled = toggle;
    }

    void setEnabled(bool enable)
    {
        enabled = enable;
    }
    [[nodiscard]] bool isEnabled() const
    {
        return enabled;
    }

    void setText(const std::string &string)
    {
        if (text) {
            text->setText(string);
        }
    }

    void setBounds(const SDL_FRect &new_bounds)
    {
        bounds = new_bounds;
    }
    [[nodiscard]] const SDL_FRect &getBounds() const
    {
        return bounds;
    }

    void setStyle(const ButtonStyle &new_style)
    {
        style = new_style;
    }
    [[nodiscard]] const ButtonStyle &getStyle() const
    {
        return style;
    }

    [[nodiscard]] ButtonState getState() const
    {
        return state;
    }

    void render(SDL_Renderer *renderer) override;
    void update(float /*delta_t*/) override {}

  private:
    SDL_FRect bounds;
    ButtonStyle style;
    ButtonState state{ButtonState::NORMAL};
    ButtonKind kind{ButtonKind::PUSH};
    bool enabled{true};
    bool toggled{false};
    std::function<void(void)> on_press;
    std::function<void(bool)> on_toggle;
    std::unique_ptr<Text> text;
    // Heap-allocated so Sprites (non-default-constructible) can be optional
    std::unique_ptr<Sprites<ButtonSprite>> sprite;

    static SDL_Color mix(SDL_Color first, SDL_Color second, float tran);

    [[nodiscard]] bool isInside(float mouse_x, float mouse_y) const;
    [[nodiscard]] SDL_Color baseColor() const;
    void fillRoundedRect(SDL_Renderer *renderer, SDL_Color color);

    void handleMouseMotion();
    void handleMouseButtonDown();
    void handleMouseButtonUp();
};
