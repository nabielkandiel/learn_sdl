#include "label.hpp"

Label::Label(GameContext &context, Font &font, SDL_FPoint pos) : text(font), position(pos)
{
    context.getEntityManager().registerEntity(*this);
}

void Label::update(float /*delta_t*/) {}

void Label::render(SDL_Renderer *render)
{
    text.renderText(position, render);
}