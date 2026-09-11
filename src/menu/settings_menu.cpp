#include "settings_menu.hpp"

#include "common/CommonEnums.hpp"

SettingsMenu::SettingsMenu(GameContext &context)
    : game(std::addressof(context)),
      input_manager(std::addressof(context.getInputManager())),
      settings(std::addressof(context.getSettings())),
      font(context.getResourceManager().getFontPath(Fonts::MONTSERRAT), 40),
      fps_label(std::make_unique<Text>(font))
{
    updateFpsLabel();
    createButtons();
    bindControls();
}

SettingsMenu::~SettingsMenu()
{
    input_manager->unbindAll(this);
}

void SettingsMenu::render(SDL_Renderer *renderer)
{
    // Dark semi-transparent backdrop
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 180);
    const SDL_FRect backdrop{.x = 0,
                             .y = 0,
                             .w = static_cast<float>(game->getScreenWidth()),
                             .h = static_cast<float>(game->getScreenHeight())};
    SDL_RenderFillRect(renderer, &backdrop);

    // FPS is keyboard-only — show it as a plain label between the two buttons
    fps_label->renderText({.x = 150.F, .y = START_Y + ROW_H}, renderer);

    for (auto &btn : buttons) {
        btn->render(renderer);
    }
}

void SettingsMenu::createButtons()
{
    // VSync toggle (row 0)
    auto vsync_btn = std::make_unique<Button>(
        *game, SDL_FRect{.x = 150.F, .y = START_Y, .w = 280.F, .h = 48.F},
        Button::ButtonKind::TOGGLE, font,
        std::string("VSync: ") + (settings->vsyncEnabled() ? "ON" : "OFF"),
        SDL_Color{.r = 255, .g = 255, .b = 255, .a = 255}, ButtonStyle{},
        InputState::PAUSED);

    vsync_btn->setToggled(settings->vsyncEnabled());
    vsync_btn->setOnToggle([this](bool new_state) {
        if (new_state != settings->vsyncEnabled()) {
            settings->toggleVsync();
        }
        buttons[0]->setText(std::string("VSync: ") +
                            (settings->vsyncEnabled() ? "ON" : "OFF"));
    });
    buttons.push_back(std::move(vsync_btn));

    // ── Back to game (row 2, below the FPS label) ───────────────────────────
    auto back_btn = std::make_unique<Button>(
        *game,
        SDL_FRect{
            .x = 150.F, .y = START_Y + (ROW_H * 2.F), .w = 200.F, .h = 48.F},
        Button::ButtonKind::PUSH, font, std::string("Back"),
        SDL_Color{.r = 255, .g = 255, .b = 255, .a = 255}, ButtonStyle{},
        InputState::PAUSED);

    back_btn->setOnPress([this]() { game->setState(State::PLAYING); });
    buttons.push_back(std::move(back_btn));
}

void SettingsMenu::bindControls()
{
    // FPS limit — keyboard only, no button needed
    input_manager->bindKeyDown(this, SDLK_UP, InputState::PAUSED, [this]() {
        settings->setFpsLimit(settings->fpsLimit() + 1);
        updateFpsLabel();
    });

    input_manager->bindKeyDown(this, SDLK_DOWN, InputState::PAUSED, [this]() {
        if (settings->fpsLimit() > 0) {
            settings->setFpsLimit(settings->fpsLimit() - 1);
        }
        updateFpsLabel();
    });
}

void SettingsMenu::updateFpsLabel()
{
    fps_label->setText("FPS Limit: " + std::to_string(settings->fpsLimit()));
}
