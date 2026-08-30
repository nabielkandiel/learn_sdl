#pragma once
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include <utility>
#include "config.h"

class Font
{
  public:
    Font(const std::string &path, float fontSize)
    {
        std::string font_path = std::string(FONT_PATH) + path;
        m_font = TTF_OpenFont(font_path.c_str(), fontSize);
    }

    ~Font()
    {
        if (m_font != nullptr) {
            TTF_CloseFont(m_font);
            m_font = nullptr;
        }
    }

    // Prevent accidental copying
    Font(const Font &) = delete;
    Font &operator=(const Font &) = delete;
    Font(Font &&other) noexcept : m_font(std::exchange(other.m_font, nullptr)) {}
    Font &operator=(Font &&other) noexcept
    {
        if (this != &other) {
            if (m_font != nullptr) {
                TTF_CloseFont(m_font);
            }
            m_font = std::exchange(other.m_font, nullptr);
        }
        return *this;
    }

    [[nodiscard]] TTF_Font *get() const
    {
        return m_font;
    }
    [[nodiscard]] bool isValid() const
    {
        return m_font != nullptr;
    }

  private:
    TTF_Font *m_font{nullptr};
};