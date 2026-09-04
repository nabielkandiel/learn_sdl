#include "button.hpp"

#include "common/CommonEnums.hpp"

Button::Button(GameContext &context)
    : ObjectBase(context),
      sprite(context.getResourceManager().getTexture(Textures::RESET_BUTTON),
             {ButtonSprite::NORMAL, ButtonSprite::ACTIVE})
{
    auto &input_manager = context.getInputManager();
    input_manager.bindMouseEvent(this, SDL_EVENT_MOUSE_MOTION,
                                 InputState::PLAYING,
                                 [this]() { handleMouseMotion(); });
    input_manager.bindMouseEvent(this, SDL_EVENT_MOUSE_BUTTON_DOWN,
                                 InputState::PLAYING,
                                 [this]() { handleMouseButtonDown(); });
    input_manager.bindMouseEvent(this, SDL_EVENT_MOUSE_BUTTON_UP,
                                 InputState::PLAYING,
                                 [this]() { handleMouseButtonUp(); });

    setupSprite(64.0F, 64.0F);
    position.x = (static_cast<float>(context.getScreenWidth()) -
                  (sprite.getActiveRect().w / 2.F));
    position.y = (static_cast<float>(context.getScreenHeight()) -
                  (sprite.getActiveRect().h / 2.F));
}

Button::~Button()
{
    getInputManager().unbindAll(this);
}

void Button::handleMouseMotion()
{
    if (!isInside()) {
        return;
    }
    state = ButtonState::MouseOver;
    sprite.setActiveDir(ButtonSprite::ACTIVE);
}

void Button::handleMouseButtonDown()
{
    if (!isInside()) {
        return;
    }
    state = ButtonState::MouseDown;
    if (on_press) {
        on_press();
    }
}

void Button::handleMouseButtonUp()
{
    if (!isInside()) {
        return;
    }
    state = ButtonState::MouseUp;
    sprite.setActiveDir(ButtonSprite::NORMAL);
}

bool Button::isInside()
{
    float mouse_x = -1.F;
    float mouse_y = -1.F;
    float width = sprite.getActiveRect().w;
    float height = sprite.getActiveRect().h;
    SDL_GetMouseState(&mouse_x, &mouse_y);
    bool inside = true;
    if ((mouse_x < (position.x - (width / 2))) ||
        (mouse_x > (position.x + (width / 2))) ||
        (mouse_y < (position.y - (height / 2))) ||
        (mouse_y > (position.y + (height / 2)))) {
        inside = false;
    }
    if (!inside) {
        state = ButtonState::MouseOut;
        sprite.setActiveDir(ButtonSprite::NORMAL);
        return false;
    }
    return true;
}