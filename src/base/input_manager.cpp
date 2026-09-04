#include "input_manager.hpp"

void InputManager::handleEvent(const SDL_Event &event)
{
    if (event.type == SDL_EVENT_KEY_DOWN && !event.key.repeat) {
        auto itter = keyDownBindings.find(event.key.key);
        if (itter != keyDownBindings.end()) {
            for (const auto &binding : itter->second) {
                if (binding.state == InputState::GLOBAL ||
                    input_state == binding.state) {
                    binding.callback();
                }
            }
        }
    } else {
        auto itter = mouseBinding.find(event.type);
        if (itter != mouseBinding.end()) {
            for (const auto &binding : itter->second) {
                if (binding.state == InputState::GLOBAL ||
                    input_state == binding.state) {
                    binding.callback();
                }
            }
        }
    }
}

void InputManager::update(float delta_t)
{
    const bool *key_states = SDL_GetKeyboardState(nullptr);
    for (const auto &[scancode, callbacks] : heldKeyBindings) {
        if (key_states[scancode]) {
            for (const auto &binding : callbacks) {
                if (binding.state == InputState::GLOBAL ||
                    input_state == binding.state) {
                    binding.callback(delta_t);
                }
            }
        }
    }
}

void InputManager::unbindAll(const void *owner)
{
    auto remove_owner = [owner](auto &map) {
        for (auto &[key, list] : map) {
            std::erase_if(list, [owner](const auto &bind) {
                return bind.owner == owner;
            });
        }
    };

    remove_owner(keyDownBindings);
    remove_owner(mouseBinding);
    remove_owner(heldKeyBindings);
}