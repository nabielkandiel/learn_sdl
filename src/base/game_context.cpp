#include "game_context.hpp"

GameContext::GameContext(Size2D screen) : width(screen.width), height(screen.height)
{
    init();
}

bool GameContext::init()
{
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("SDL could not initialize - SDL error :%s\n", SDL_GetError());
        return false;
    }

    if (!SDL_CreateWindowAndRenderer("Nasty Tetris", width, height, 0, &window, &renderer)) {
        SDL_Log("Create Window and Renderer failed - SDL error: %s\n", SDL_GetError());
        return false;
    }

    if (!TTF_Init()) {
        SDL_Log("TTF Init failed - SDL error: %s\n", SDL_GetError());
        return false;
    }

    loadTextures();

    return true;
}

void GameContext::close()
{
    destroyTextures();
    SDL_DestroyWindow(window);
    window = nullptr;
    SDL_DestroyRenderer(renderer);
    renderer = nullptr;
    TTF_Quit();
    SDL_Quit();
}

void GameContext::loadTextures()
{
    {
        auto &texture = texture_map[TEXTURES::BALL];
        texture.setColorKey(SDL_Color{.r = 0, .g = 180, .b = 180, .a = 0});
        if (!loadAsset(texture, renderer, "balls.png")) {
            SDL_Log("failed to load balls.png\n");
        }
    }
    {
        auto &texture = texture_map[TEXTURES::ARROW];
        if (!loadAsset(texture, renderer, "arrow.png")) {
            SDL_Log("failed to load arrow.png\n");
        }
    }
    {
        auto &texture = texture_map[TEXTURES::RESET_BUTTON];
        if (!loadAsset(texture, renderer, "reset_bttn.png")) {
            SDL_Log("failed to load reset_bttn.png \n");
        }
    }
}

void GameContext::destroyTextures()
{
    texture_map[TEXTURES::BALL].destroy();
    texture_map[TEXTURES::ARROW].destroy();
    texture_map[TEXTURES::RESET_BUTTON].destroy();
}