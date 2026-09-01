#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_pixels.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_stdinc.h"

#include "base/font.hpp"
#include "base/game_context.hpp"
#include "base/settings.hpp"
#include "objects/arrow.hpp"
#include "objects/ball.hpp"
#include "objects/button.hpp"
#include "objects/frame_label.hpp"
#include "objects/label.hpp"

int main()
{
    constexpr int k_screen_width{640};
    constexpr int k_screen_height{480};

    GameContext game({.width = k_screen_width, .height = k_screen_height});
    auto &input_manager = game.getInputManager();
    auto &entity_manager = game.getEntityManager();

    if (game.getSettings().enableVsync()) {
        SDL_Log("VSYNC ENABLED\n");
    }

    SDL_Log("Starting with dimension %u x %u\n", k_screen_width, k_screen_height);

    Font main_font("Montserrat/Montserrat-VariableFont_wght.ttf", 60);
    Font small_font("Montserrat/Montserrat-VariableFont_wght.ttf", 20);

    FrameLabel frame_label(game, small_font, {.x = 0, .y = 80});

    // character sprite
    Ball ball(game, {.x = k_screen_width, .y = k_screen_height});

    Button reset_btn(game);
    reset_btn.setOnPress([&ball]() { ball.makeCenter(); });

    Arrow arrow{game, std::nullopt, ball};
    Label hello(game, main_font);
    hello.setText("Hello World");

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
                input_manager.handleEvent(event);
            }
        }
        input_manager.update(dt_c);
        entity_manager.updateEntites(dt_c);

        // set background to white
        SDL_SetRenderDrawColor(game.getRenderer(), bg_color.r, bg_color.g, bg_color.b, 0xFF);
        SDL_RenderClear(game.getRenderer());

        // render image to screen
        entity_manager.renderEntities(game.getRenderer());

        // update screen
        SDL_RenderPresent(game.getRenderer());
    }

    return 0;
}
