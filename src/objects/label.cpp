#include "label.hpp"

Label::Label(GameContext &context, Font &font, SDL_FPoint pos) : ObjectBase(context), text(font), position(pos) {}

void Label::update(float /*delta_t*/) {}

void Label::render(SDL_Renderer *render)
{
    text.renderText(position, render);
}