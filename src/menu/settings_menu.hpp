#pragma once

#include <functional>
#include <memory>
#include <string>

#include "base/font.hpp"
#include "base/game_context.hpp"
#include "base/input_manager.hpp"
#include "base/settings.hpp"
#include "base/text.hpp"

class SettingsMenu
{
  public:
    SettingsMenu(const SettingsMenu &) = delete;
    SettingsMenu(SettingsMenu &&) = delete;
    SettingsMenu &operator=(const SettingsMenu &) = delete;
    SettingsMenu &operator=(SettingsMenu &&) = delete;
    SettingsMenu(GameContext &context);
    ~SettingsMenu();

    void render(SDL_Renderer *renderer);

  private:
    struct MenuOption
    {
        std::string label;
        std::unique_ptr<Text> text;
        std::function<void()> onActivate;
    };

    GameContext *game;
    InputManager *input_manager;
    Settings *settings;
    std::vector<MenuOption> options;
    Font font;

    void refreshLabels();
    void bindControls();
};