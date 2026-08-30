#pragma once

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_keycode.h>
#include <functional>

class InputManager
{
  public:
    using ActionCallback = std::function<void()>;
    using ContinuousCallback = std::function<void(float delata_t)>;

    void bindKeyDown(SDL_Keycode key, ActionCallback callback)
    {
        keyDownBindings[key].push_back(std::move(callback));
    }

    void bindHeldKey(SDL_Scancode scanCode, ContinuousCallback callback)
    {
        heldKeyBindings[scanCode].push_back(std::move(callback));
    }

    void bindMouseEvent(uint32_t eventType, ActionCallback callback)
    {
        mouseBinding[eventType].push_back(std::move(callback));
    }

    void handleEvent(const SDL_Event &event)
    {
        if (event.type == SDL_EVENT_KEY_DOWN && !event.key.repeat) {
            auto itter = keyDownBindings.find(event.key.key);
            if (itter != keyDownBindings.end()) {
                for (const auto &callback : itter->second) {
                    callback();
                }
            }
        } else {
            auto itter = mouseBinding.find(event.type);
            if (itter != mouseBinding.end()) {
                for (const auto &callback : itter->second) {
                    callback();
                }
            }
        }
    }

    void update(float delta_t)
    {
        const bool *key_states = SDL_GetKeyboardState(nullptr);
        for (const auto &[scancode, callbacks] : heldKeyBindings) {
            if (key_states[scancode]) {
                for (const auto &callback : callbacks) {
                    callback(delta_t);
                }
            }
        }
    }

  private:
    std::unordered_map<SDL_Keycode, std::vector<ActionCallback>> keyDownBindings;
    //                 //event type
    std::unordered_map<uint32_t, std::vector<ActionCallback>> mouseBinding;
    std::unordered_map<SDL_Scancode, std::vector<ContinuousCallback>> heldKeyBindings;
};