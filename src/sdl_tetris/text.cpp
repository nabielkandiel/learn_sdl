#include "text.hpp"
#include "config.h"

Text::Text(const std::string &fontPath, float fontSize)
{
    std::string path{FONT_PATH};
    path += fontPath;
    font = TTF_OpenFont(path.c_str(), fontSize);
    if (font == nullptr) {
        SDL_Log("Could not load font file %s, SDL_TTF error %s\n", fontPath.c_str(), SDL_GetError());
        return;
    }

    valid = true;
}

Text::Text(const std::string &fontPath, float fontSize, SDL_Color colorVal) : Text(fontPath, fontSize)
{
    color = colorVal;
}

Text::~Text()
{
    textTexture.destroy();
    TTF_CloseFont(font);
    font = nullptr;
    valid = false;
}

[[nodiscard]] bool Text::isValid() const
{
    return valid;
}

bool Text::loadText(const std::string &text, SDL_Renderer *sdl_renderer)
{
    if (!valid) {
        return false;
    }
    return textTexture.loadFromRenderedText(text, color, font, sdl_renderer);
}

void Text::renderText(SDL_FPoint location, SDL_Renderer *sdl_renderer)
{
    textTexture.render(location, sdl_renderer, nullptr, nullptr);
}

[[nodiscard]] Texture &Text::getTexture()
{
    return textTexture;
}