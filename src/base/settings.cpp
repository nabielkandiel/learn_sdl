#include "settings.hpp"

#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_log.h>

Settings::Settings(GameContext &context) : renderer(context.getRenderer())
{
    auto &input_manager = context.getInputManager();

    input_manager.bindKeyDown(SDLK_V, [this]() { vSyncEnabled ? disableVsync() : enableVsync(); });
};

bool Settings::enableVsync()
{
    if (vSyncEnabled) {
        return true;
    }

    vSyncEnabled = true;

    if (!SDL_SetRenderVSync(renderer, 1)) {
        SDL_Log("Failed to enable vsync - SDL error: %s\n", SDL_GetError());
        return false;
    }
    SDL_Log("Enable vsync \n");
    return true;
}

bool Settings::disableVsync()
{
    if (!vSyncEnabled) {
        return true;
    }
    vSyncEnabled = false;

    if (!SDL_SetRenderVSync(renderer, 0)) {
        SDL_Log("Failed to disable vsync - SDL error: %s\n", SDL_GetError());
        return false;
    }
    SDL_Log("Disable vsync \n");
    return true;
}
