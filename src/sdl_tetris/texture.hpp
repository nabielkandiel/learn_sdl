#pragma once
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <optional>
#include <string>

class Texture
{
  public:
    Texture();
    Texture(SDL_Color key);
    Texture(std::optional<SDL_Color> key);
    ~Texture();

    // copy
    Texture(const Texture &texture) = delete;
    Texture &operator=(const Texture &texture) = delete;

    // move
    Texture(Texture &&texture) = delete;
    Texture &operator=(Texture &&texture) = delete;

    bool loadFromFile(const std::string &path, SDL_Renderer *sdl_renderer);
    bool loadFromRenderedText(const std::string &textureText, SDL_Color textColor, TTF_Font *font,
                              SDL_Renderer *sdl_renderer);
    void destroy();
    void render(SDL_FPoint dst_cords, SDL_Renderer *sdl_renderer, const SDL_FRect *src_rect,
                const SDL_FPoint *dst_dims);
    void renderWithTransform(SDL_FPoint dst_cords, SDL_Renderer *sdl_renderer, const SDL_FRect *src_rect,
                             const SDL_FPoint *dst_dims, double degrees, SDL_FPoint *center, SDL_FlipMode flip_mode);

    [[nodiscard]] int getWidth() const
    {
        return m_width;
    }

    [[nodiscard]] int getHeight() const
    {
        return m_height;
    }

    [[nodiscard]] bool isLoaded() const
    {
        return sdl_texture != nullptr;
    }

  private:
    SDL_Texture *sdl_texture;
    std::optional<SDL_Color> color_key;
    int m_width;
    int m_height;
};
