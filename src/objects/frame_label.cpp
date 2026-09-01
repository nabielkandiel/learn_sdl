#include "frame_label.hpp"

FrameLabel::FrameLabel(GameContext &context, Font &font, SDL_FPoint pos)
    : ObjectBase(context), text(font), position(pos), settings(std::addressof(context.getSettings()))
{
    timer.start();
    auto &input_manager = context.getInputManager();
    input_manager.bindKeyDown(
        this, SDLK_V, [this]() { settings->vsyncEnabled() ? settings->disableVsync() : settings->enableVsync(); });
};

FrameLabel::~FrameLabel()
{
    getInputManager().unbindAll(this);
}

void FrameLabel::update(float delta_t)
{
    timeSinceUpdate += delta_t;
    frameCount++;
    if (timeSinceUpdate > 1) {
        fps = static_cast<double>(frameCount) / static_cast<double>(timeSinceUpdate);
        timeSinceUpdate = 0;
        frameCount = 0;
        std::string msg = "MS since start: " + std::to_string(timer.getTicksNS() / 1'000'000) + "\n" +
                          "Vsync: " + (settings->vsyncEnabled() ? "Yes" : "No") + "\n" + "FPS: " + std::to_string(fps);
        text.setText(msg);
    }
}

void FrameLabel::render(SDL_Renderer *renderer)
{
    text.renderText(position, renderer);
}
