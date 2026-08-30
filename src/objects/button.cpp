#include "button.hpp"

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
    if ((mouse_x < (position.x - (width / 2))) || (mouse_x > (position.x + (width / 2))) ||
        (mouse_y < (position.y - (height / 2))) || (mouse_y > (position.y + (height / 2)))) {
        inside = false;
    }
    if (!inside) {
        state = ButtonState::MouseOut;
        sprite.setActiveDir(ButtonSprite::NORMAL);
        return false;
    }
    return true;
}