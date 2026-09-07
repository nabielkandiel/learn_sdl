#include "button.hpp"

#include <cmath>
#include <numbers>

// ── Constructors ────────────────────────────────────────────────────────────

Button::Button(GameContext &context, const SDL_FRect &bounds, ButtonKind kind,
               ButtonStyle style, InputState input_state)
    : ObjectBase(context), bounds(bounds), style(style), kind(kind)
{
    auto &input_manager = context.getInputManager();
    input_manager.bindMouseEvent(this, SDL_EVENT_MOUSE_MOTION, input_state,
                                 [this]() { handleMouseMotion(); });
    input_manager.bindMouseEvent(this, SDL_EVENT_MOUSE_BUTTON_DOWN, input_state,
                                 [this]() { handleMouseButtonDown(); });
    input_manager.bindMouseEvent(this, SDL_EVENT_MOUSE_BUTTON_UP, input_state,
                                 [this]() { handleMouseButtonUp(); });
}

Button::Button(GameContext &context, const SDL_FRect &bounds, ButtonKind kind,
               Font &font, const std::string &label, SDL_Color text_color,
               ButtonStyle style, InputState input_state)
    : Button(context, bounds, kind, style, input_state)
{
    text = std::make_unique<Text>(font, text_color);
    text->setText(label);
}

Button::Button(GameContext &context, const SDL_FRect &bounds, ButtonKind kind,
               Texture &texture, float spr_w, float spr_h, ButtonStyle style,
               InputState input_state)
    : Button(context, bounds, kind, style, input_state)
{
    sprite = std::make_unique<Sprites<ButtonSprite>>(
        texture, std::array<ButtonSprite, 2>{ButtonSprite::NORMAL,
                                             ButtonSprite::ACTIVE});
    sprite->sliceTextureHorizontal(spr_w, spr_h);
    sprite->setActiveDir(ButtonSprite::NORMAL);
}

Button::~Button()
{
    getInputManager().unbindAll(this);
}

// ── Rendering ───────────────────────────────────────────────────────────────

void Button::render(SDL_Renderer *renderer)
{
    const SDL_Color color = baseColor();
    fillRoundedRect(renderer, color);

    if (kind == ButtonKind::TOGGLE && toggled && enabled) {
        const SDL_FRect inner{.x = bounds.x + 2,
                              .y = bounds.y + 2,
                              .w = bounds.w - 4,
                              .h = bounds.h - 4};
        const SDL_Color ring = mix(
            color, SDL_Color{.r = 255, .g = 255, .b = 255, .a = 255}, 0.25F);
        SDL_SetRenderDrawColor(renderer, ring.r, ring.g, ring.b, 90);
        SDL_RenderRect(renderer, &inner);
    }

    if (style.border.has_value()) {
        const auto &border_color = style.border.value();
        SDL_SetRenderDrawColor(renderer, border_color.r, border_color.g,
                               border_color.b, border_color.a);
        SDL_RenderRect(renderer, &bounds);
    }

    const float corner_x = bounds.x + (bounds.w / 2.0F);
    const float corner_y = bounds.y + (bounds.h / 2.0F);

    if (sprite) {
        const bool active =
            toggled || (state == ButtonState::MOUSE_DOWN && enabled);
        sprite->setActiveDir(active ? ButtonSprite::ACTIVE
                                    : ButtonSprite::NORMAL);

        if (text) {
            auto &tex = text->getTexture();
            const float text_w =
                tex.isLoaded() ? static_cast<float>(tex.getWidth()) : 0.0F;
            const float text_h =
                tex.isLoaded() ? static_cast<float>(tex.getHeight()) : 0.0F;
            const float content_w = style.iconSize + style.iconGap + text_w;
            const float left = bounds.x + ((bounds.w - content_w) / 2.0F);

            sprite->renderActive(
                renderer, {.x = left + (style.iconSize / 2.0F), .y = corner_y});
            text->renderText({.x = left + style.iconSize + style.iconGap,
                              .y = corner_y - (text_h / 2.0F)},
                             renderer);
        } else {
            sprite->renderActive(renderer, {.x = corner_x, .y = corner_y});
        }
        return;
    }

    if (text) {
        auto &tex = text->getTexture();
        const float text_w =
            tex.isLoaded() ? static_cast<float>(tex.getWidth()) : 0.0F;
        const float text_h =
            tex.isLoaded() ? static_cast<float>(tex.getHeight()) : 0.0F;
        text->renderText(
            {.x = corner_x - (text_w / 2.0F), .y = corner_y - (text_h / 2.0F)},
            renderer);
    }
}

// Color helpers

SDL_Color Button::mix(SDL_Color first, SDL_Color second, float tran)
{
    auto lerp = [](uint8_t one, uint8_t two, float fly) -> uint8_t {
        return static_cast<uint8_t>(std::lround(
            static_cast<float>(one) +
            ((static_cast<float>(two) - static_cast<float>(one)) * fly)));
    };
    return {.r = lerp(first.r, second.r, tran),
            .g = lerp(first.g, second.g, tran),
            .b = lerp(first.b, second.b, tran),
            .a = lerp(first.a, second.a, tran)};
}

SDL_Color Button::baseColor() const
{
    if (!enabled) {
        return style.colDisabled;
    }
    switch (state) {
    case ButtonState::MOUSE_OVER:
        return toggled ? mix(style.colActive, style.hover, 0.5F) : style.hover;
    case ButtonState::MOUSE_DOWN:
        return toggled ? mix(style.colActive, style.pressed, 0.5F)
                       : style.pressed;
    case ButtonState::NORMAL:
    default:
        return toggled ? style.colActive : style.normal;
    }
}

// Rounded-rect geometry

void Button::fillRoundedRect(SDL_Renderer *renderer, SDL_Color color)
{
    const float radius =
        std::min(style.cornerRadius, std::min(bounds.w, bounds.h) * 0.5F);

    if (radius < 1.0F) {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderFillRect(renderer, &bounds);
        return;
    }

    const SDL_FColor float_c{.r = static_cast<float>(color.r) / 255.F,
                             .g = static_cast<float>(color.g) / 255.F,
                             .b = static_cast<float>(color.b) / 255.F,
                             .a = static_cast<float>(color.a) / 255.F};
    constexpr int seg = 6; // arc segments per corner
    std::vector<SDL_Vertex> verts;
    std::vector<int> idx;
    // Centre vertex (index 0)
    verts.push_back({.position = {.x = bounds.x + (bounds.w * 0.5F),
                                  .y = bounds.y + (bounds.h * 0.5F)},
                     .color = float_c,
                     .tex_coord = {}});

    struct Corner
    {
        float ox, oy, a0;
    };
    const std::array<Corner, 4> corners{
        {{.ox = bounds.x + radius, .oy = bounds.y + radius, .a0 = 180.F},
         {.ox = bounds.x + bounds.w - radius,
          .oy = bounds.y + radius,
          .a0 = 270.F},
         {.ox = bounds.x + bounds.w - radius,
          .oy = bounds.y + bounds.h - radius,
          .a0 = 0.F},
         {.ox = bounds.x + radius,
          .oy = bounds.y + bounds.h - radius,
          .a0 = 90.F}}};

    for (const auto &corner : corners) {
        for (int i = 0; i <= seg; ++i) {
            const float angle =
                (corner.a0 + (90.F * static_cast<float>(i) / seg)) *
                std::numbers::pi_v<float> / 180.F;
            verts.push_back(
                {.position = {.x = corner.ox + (std::cos(angle) * radius),
                              .y = corner.oy + (std::sin(angle) * radius)},
                 .color = float_c,
                 .tex_coord = {}});
        }
    }

    const int rim = static_cast<int>(verts.size()) - 1;
    for (int i = 1; i <= rim; ++i) {
        const int next = (i % rim) + 1;
        idx.push_back(0);
        idx.push_back(i);
        idx.push_back(next);
    }
    SDL_RenderGeometry(renderer, nullptr, verts.data(),
                       static_cast<int>(verts.size()), idx.data(),
                       static_cast<int>(idx.size()));
}

// ── Input handlers ───────────────────────────────────────────────────────────

bool Button::isInside(float mouse_x, float mouse_y) const
{
    const SDL_FPoint point{.x = mouse_x, .y = mouse_y};
    return SDL_PointInRectFloat(&point, &bounds);
}

void Button::handleMouseMotion()
{
    if (!enabled) {
        return;
    }
    float mouse_x = 0.0F;
    float mouse_y = 0.0F;
    SDL_GetMouseState(&mouse_x, &mouse_y);
    if (isInside(mouse_x, mouse_y)) {
        if (state != ButtonState::MOUSE_DOWN) {
            state = ButtonState::MOUSE_OVER;
        }
    } else {
        state = ButtonState::NORMAL;
    }
}

void Button::handleMouseButtonDown()
{
    if (!enabled) {
        return;
    }
    float mouse_x = 0.0F;
    float mouse_y = 0.0F;
    SDL_GetMouseState(&mouse_x, &mouse_y);
    if (!isInside(mouse_x, mouse_y)) {
        return;
    }

    state = ButtonState::MOUSE_DOWN;

    if (kind == ButtonKind::TOGGLE) {
        toggled = !toggled;
        if (on_toggle) {
            on_toggle(toggled);
        }
    }

    if (on_press) {
        on_press();
    }
}

void Button::handleMouseButtonUp()
{
    if (!enabled) {
        return;
    }
    float mouse_x = 0.0F;
    float mouse_y = 0.0F;
    SDL_GetMouseState(&mouse_x, &mouse_y);
    state = isInside(mouse_x, mouse_y) ? ButtonState::MOUSE_OVER
                                       : ButtonState::NORMAL;
}
