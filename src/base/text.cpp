#include "text.hpp"
#include "config.h"

Text::Text(const Font &font) : font(font) {}

Text::Text(const Font &font, SDL_Color colorVal) : Text(font)
{
    color = colorVal;
}

Text::~Text() = default;

[[nodiscard]] bool Text::isValid() const
{
    return font.isValid();
}

void Text::setText(const std::string &text)
{
    currText = text;
    changed = true;
}

void Text::renderText(SDL_FPoint location, SDL_Renderer *sdl_renderer)
{
    if (currText.empty()) {
        return;
    }

    if (changed) {
        textTexture.loadFromRenderedText(currText, color, font.get(), sdl_renderer);
        changed = false;
    }
    textTexture.render(location, sdl_renderer, nullptr, nullptr);
}

[[nodiscard]] Texture &Text::getTexture()
{
    return textTexture;
}