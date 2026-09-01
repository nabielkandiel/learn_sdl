#pragma once

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_keycode.h>
#include <functional>

class InputManager
{
  public:
    using ActionCallback = std::function<void()>;
    using ContinuousCallback = std::function<void(float delata_t)>;

    void bindKeyDown(const void *owner, SDL_Keycode key, ActionCallback callback)
    {
        keyDownBindings[key].push_back({.owner = owner, .callback = std::move(callback)});
    }

    void bindHeldKey(const void *owner, SDL_Scancode scanCode, ContinuousCallback callback)
    {
        heldKeyBindings[scanCode].push_back({.owner = owner, .callback = std::move(callback)});
    }

    void bindMouseEvent(const void *owner, uint32_t eventType, ActionCallback callback)
    {
        mouseBinding[eventType].push_back({.owner = owner, .callback = std::move(callback)});
    }

    void handleEvent(const SDL_Event &event)
    {
        if (event.type == SDL_EVENT_KEY_DOWN && !event.key.repeat) {
            auto itter = keyDownBindings.find(event.key.key);
            if (itter != keyDownBindings.end()) {
                for (const auto &binding : itter->second) {
                    binding.callback();
                }
            }
        } else {
            auto itter = mouseBinding.find(event.type);
            if (itter != mouseBinding.end()) {
                for (const auto &binding : itter->second) {
                    binding.callback();
                }
            }
        }
    }

    void update(float delta_t)
    {
        const bool *key_states = SDL_GetKeyboardState(nullptr);
        for (const auto &[scancode, callbacks] : heldKeyBindings) {
            if (key_states[scancode]) {
                for (const auto &binding : callbacks) {
                    binding.callback(delta_t);
                }
            }
        }
    }

    void unbindAll(const void *owner)
    {
        auto remove_owner = [owner](auto &map) {
            for (auto &[key, list] : map) {
                std::erase_if(list, [owner](const auto &bind) { return bind.owner == owner; });
            }
        };

        remove_owner(keyDownBindings);
        remove_owner(mouseBinding);
        remove_owner(heldKeyBindings);
    }

  private:
    template <typename CB> struct Binding
    {
        const void *owner;
        CB callback;
    };

    std::unordered_map<SDL_Keycode, std::vector<Binding<ActionCallback>>> keyDownBindings;
    //                 //event type
    std::unordered_map<uint32_t, std::vector<Binding<ActionCallback>>> mouseBinding;
    std::unordered_map<SDL_Scancode, std::vector<Binding<ContinuousCallback>>> heldKeyBindings;
};