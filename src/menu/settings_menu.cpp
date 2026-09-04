#include "settings_menu.hpp"

#include "common/CommonEnums.hpp"

SettingsMenu::SettingsMenu(GameContext &context)
    : game(std::addressof(context)),
      input_manager(std::addressof(context.getInputManager())),
      settings(std::addressof(context.getSettings())),
      font(context.getResourceManager().getFontPath(Fonts::MONTSERRAT), 40)
{
    refreshLabels();
    bindControls();
}

SettingsMenu::~SettingsMenu()
{
    input_manager->unbindAll(this);
}

void SettingsMenu::render(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 180); // dark tint
    SDL_FRect backdrop{.x = 0,
                       .y = 0,
                       .w = static_cast<float>(game->getScreenWidth()),
                       .h = static_cast<float>(game->getScreenHeight())};
    SDL_RenderFillRect(renderer, &backdrop);

    float start_y = 150.F;
    int y_idx = 0;
    for (auto &option : options) {
        option.text->setText(option.label);
        option.text->renderText(
            {.x = 150.F, .y = start_y + (static_cast<float>(y_idx++) * 50.F)},
            renderer);
    }
}

void SettingsMenu::refreshLabels()
{
    options.clear();

    options.push_back(
        {.label =
             "VSync: " + std::string(settings->vsyncEnabled() ? "ON" : "OFF"),
         .text = std::make_unique<Text>(font),
         .onActivate = [this]() {
             settings->toggleVsync();
             refreshLabels();
         }});

    options.push_back(
        {.label = "FrameLimit: " + std::to_string(settings->fpsLimit()),
         .text = std::make_unique<Text>(font),
         .onActivate = [this]() { refreshLabels(); }});

    options.push_back(
        {.label = "Back To Game",
         .text = std::make_unique<Text>(font),
         .onActivate = [this]() { game->setState(State::PLAYING); }});
}

void SettingsMenu::bindControls()
{
    input_manager->bindKeyDown(this, SDLK_UP, InputState::PAUSED, [this]() {
        settings->setFpsLimit(settings->fpsLimit() + 1);
        refreshLabels();
    });

    input_manager->bindKeyDown(this, SDLK_DOWN, InputState::PAUSED, [this]() {
        if (settings->fpsLimit() > 0) {
            settings->setFpsLimit(settings->fpsLimit() - 1);
        }
        refreshLabels();
    });

    input_manager->bindKeyDown(this, SDLK_V, InputState::PAUSED, [this]() {
        settings->toggleVsync();
        refreshLabels();
    });
}