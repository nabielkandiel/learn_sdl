#include "game_context.hpp"

GameContext::GameContext(Size2D screen)
    : width(screen.width), height(screen.height)
{
    init();
    resourceManager.loadTextures(renderer);
}

bool GameContext::init()
{
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("SDL could not initialize - SDL error :%s\n", SDL_GetError());
        return false;
    }

    if (!SDL_CreateWindowAndRenderer("Nasty Tetris", width, height, 0, &window,
                                     &renderer)) {
        SDL_Log("Create Window and Renderer failed - SDL error: %s\n",
                SDL_GetError());
        return false;
    }

    if (!TTF_Init()) {
        SDL_Log("TTF Init failed - SDL error: %s\n", SDL_GetError());
        return false;
    }

    settings.setRenderer(renderer);
    inputManager.bindKeyDown(this, SDLK_ESCAPE, InputState::GLOBAL, [this]() {
        switch (state) {
        case State::PLAYING:
            setState(State::PAUSED);
            break;
        case State::PAUSED:
            setState(State::PLAYING);
            break;
        }
    });

    return true;
}

void GameContext::close()
{
    SDL_DestroyWindow(window);
    window = nullptr;
    SDL_DestroyRenderer(renderer);
    renderer = nullptr;
    TTF_Quit();
    SDL_Quit();
}

[[nodiscard]] int GameContext::getScreenWidth() const
{
    return width;
}

[[nodiscard]] int GameContext::getScreenHeight() const
{
    return height;
}

SDL_Renderer *GameContext::getRenderer()
{
    return renderer;
}

InputManager &GameContext::getInputManager()
{
    return inputManager;
}

EntityManager &GameContext::getEntityManager()
{
    return entityManager;
}

ResourceManager &GameContext::getResourceManager()
{
    return resourceManager;
}

Settings &GameContext::getSettings()
{
    return settings;
}

State GameContext::getState() const
{
    return state;
}

void GameContext::setState(State new_state)
{
    state = new_state;

    switch (state) {

    case State::PLAYING:
        inputManager.setInputState(InputState::PLAYING);
        break;
    case State::PAUSED:
        inputManager.setInputState(InputState::PAUSED);
        break;
    }
}