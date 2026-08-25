#include "button.hpp"

void Button::handleUpdate(const SDL_Event &event)
{
    if (event.type == SDL_EVENT_MOUSE_MOTION || event.type == SDL_EVENT_MOUSE_BUTTON_DOWN ||
        event.type == SDL_EVENT_MOUSE_BUTTON_UP) {
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
            return;
        }

        switch (event.type) {
        case SDL_EVENT_MOUSE_MOTION:
            state = ButtonState::MouseOver;
            break;
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            state = ButtonState::MouseDown;
            sprite.setActiveDir(ButtonSprite::CLICKED);
            break;
        case SDL_EVENT_MOUSE_BUTTON_UP:
            state = ButtonState::MouseUp;
            sprite.setActiveDir(ButtonSprite::NORMAL);
            break;
        default:
        }
    }
}