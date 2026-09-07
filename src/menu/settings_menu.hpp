#pragma once

#include <memory>
#include <vector>

#include "base/font.hpp"
#include "base/game_context.hpp"
#include "base/input_manager.hpp"
#include "base/settings.hpp"
#include "base/text.hpp"
#include "objects/button.hpp"

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
    GameContext *game;
    InputManager *input_manager;
    Settings *settings;
    Font font;
    std::unique_ptr<Text> fps_label;
    // Button is non-copyable and non-movable — must be held by pointer
    std::vector<std::unique_ptr<Button>> buttons;

    static constexpr float START_Y = 150.F;
    static constexpr float ROW_H = 60.F;

    void createButtons();
    void bindControls();
    void updateFpsLabel();
};
