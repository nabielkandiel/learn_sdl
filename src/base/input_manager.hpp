#pragma once

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_keycode.h>
#include <functional>

#include "common/CommonEnums.hpp"

class InputManager
{
  public:
    using ActionCallback = std::function<void()>;
    using ContinuousCallback = std::function<void(float delata_t)>;

    void bindKeyDown(const void *owner, SDL_Keycode key, InputState state,
                     ActionCallback callback)
    {
        keyDownBindings[key].push_back(
            {.owner = owner, .callback = std::move(callback), .state = state});
    }

    void bindHeldKey(const void *owner, SDL_Scancode scanCode, InputState state,
                     ContinuousCallback callback)
    {
        heldKeyBindings[scanCode].push_back(
            {.owner = owner, .callback = std::move(callback), .state = state});
    }

    void bindMouseEvent(const void *owner, uint32_t eventType, InputState state,
                        ActionCallback callback)
    {
        mouseBinding[eventType].push_back(
            {.owner = owner, .callback = std::move(callback), .state = state});
    }

    void setInputState(InputState state)
    {
        input_state = state;
    }

    void handleEvent(const SDL_Event &event);
    void update(float delta_t);
    void unbindAll(const void *owner);

  private:
    template <typename CB> struct Binding
    {
        const void *owner;
        CB callback;
        InputState state;
    };

    InputState input_state{InputState::PLAYING};

    std::unordered_map<SDL_Keycode, std::vector<Binding<ActionCallback>>>
        keyDownBindings;
    //                 //event type
    std::unordered_map<uint32_t, std::vector<Binding<ActionCallback>>>
        mouseBinding;
    std::unordered_map<SDL_Scancode, std::vector<Binding<ContinuousCallback>>>
        heldKeyBindings;
};