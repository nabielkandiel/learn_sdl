#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_keyboard.h"
#include "SDL3/SDL_pixels.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_stdinc.h"
#include "base/utility.h"

#include "base/context.hpp"
#include "base/font.hpp"
#include "base/text.hpp"
#include "objects/ball.hpp"
#include "objects/button.hpp"
#include "objects/timer.hpp"

#include <cmath>
#include <numbers>

int main()
{
    constexpr int k_screen_width{640};
    constexpr int k_screen_height{480};

    Context game({.width = k_screen_width, .height = k_screen_height});
    SDL_Log("Starting with dimension %u x %u\n", k_screen_width, k_screen_height);

    Font main_font("Montserrat/Montserrat-VariableFont_wght.ttf", 60);

    Timer timer(main_font, {.x = 0, .y = 80});

    // character sprite
    Ball ball({.x = k_screen_width, .y = k_screen_height}, SDL_Color{.r = 0, .g = 180, .b = 180, .a = 0});
    ball.setupSprite(32.0F, 32.0F, {.rows = 2, .cols = 2});

    Button reset_btn;
    reset_btn.setupSprite(64.0F, 64.0F);
    reset_btn.setOnPress([&ball]() { ball.makeCenter(); });

    Texture arrow_texture{};
    Text text(main_font);
    text.setText("Hello World");

    if (!loadAsset(ball.getTexture(), game.renderer, "balls.png")) {
        SDL_Log("failed to load media\n");
        return 2;
    }

    if (!loadAsset(reset_btn.getTexture(), game.renderer, "reset_bttn.png")) {
        SDL_Log("failed to load media\n");
        return 2;
    }

    float rst_x = (k_screen_width - (reset_btn.getActiveRect().w / 2.F));
    float rst_y = (k_screen_height - (reset_btn.getActiveRect().h / 2.F));
    reset_btn.setPosition({.x = rst_x, .y = rst_y});

    if (!loadAsset(arrow_texture, game.renderer, "arrow.png")) {
        SDL_Log("failed to load media\n");
        return 2;
    }

    bool quit{false};
    SDL_Event event;
    SDL_zero(event);

    // default background to white
    SDL_Color bg_color{.r = 0xFF, .g = 0xFF, .b = 0xFF, .a = 0xFF};

    uint64_t last = SDL_GetTicksNS();
    while (!quit) {
        Uint64 now = SDL_GetTicksNS();
        float dt_c = static_cast<float>(now - last) / 1'000'000'000.0F; // ns → seconds
        last = now;
        // handle input
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                quit = true;
            } else {
                ball.handleInput(event);
                reset_btn.handleInput(event);
                timer.handleInput(event);
            }
        }

        bg_color.r = 0xFF;
        bg_color.g = 0xFF;
        bg_color.b = 0xFF;

        // set background color based on keys state
        const bool *key_states = SDL_GetKeyboardState(nullptr);
        if (key_states[SDL_SCANCODE_UP]) {
            bg_color.r = 0xFF;
            bg_color.g = 0x00;
            bg_color.b = 0x00;
            ball.tryUp(dt_c);
        }
        if (key_states[SDL_SCANCODE_DOWN]) {
            bg_color.r = 0x00;
            bg_color.g = 0xFF;
            bg_color.b = 0x00;
            ball.tryDown(dt_c);
        }
        if (key_states[SDL_SCANCODE_LEFT]) {
            bg_color.r = 0xFF;
            bg_color.g = 0xFF;
            bg_color.b = 0x00;
            ball.tryLeft(dt_c);
        }
        if (key_states[SDL_SCANCODE_RIGHT]) {
            bg_color.r = 0x00;
            bg_color.g = 0x00;
            bg_color.b = 0xFF;
            ball.tryRight(dt_c);
        }
        ball.update(dt_c);
        timer.update(dt_c);
        // set background to white
        SDL_SetRenderDrawColor(game.renderer, bg_color.r, bg_color.g, bg_color.b, 0xFF);
        SDL_RenderClear(game.renderer);
        // render image to screen
        text.renderText({.x = 0, .y = 0}, game.renderer);
        ball.render(game.renderer);
        reset_btn.render(game.renderer);
        timer.render(game.renderer);

        const SDL_FPoint arrow_pos{.x = 0.0F, .y = static_cast<float>(k_screen_height - arrow_texture.getHeight())};
        const SDL_FPoint arrow_center{.x = arrow_pos.x + (static_cast<float>(arrow_texture.getWidth()) / 2.0F),
                                      .y = arrow_pos.y + (static_cast<float>(arrow_texture.getHeight()) / 2.0F)};
        const SDL_FPoint ball_pos = ball.getPosition();
        const float dxx = ball_pos.x - arrow_center.x;
        const float dyy = ball_pos.y - arrow_center.y;
        const double degrees = std::atan2(dxx, -dyy) * (180.0 / std::numbers::pi);

        arrow_texture.renderWithTransform(arrow_pos, game.renderer, nullptr, nullptr, degrees, nullptr, SDL_FLIP_NONE);

        // update screen
        SDL_RenderPresent(game.renderer);
    }

    ball.getTexture().destroy();
    reset_btn.getTexture().destroy();
    arrow_texture.destroy();
    text.getTexture().destroy();
    timer.getTexture().destroy();

    return 0;
}
