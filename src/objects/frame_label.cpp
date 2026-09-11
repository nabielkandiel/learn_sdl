#include "frame_label.hpp"

FrameLabel::FrameLabel(GameContext &context, Font &font, SDL_FPoint pos)
    : ObjectBase(context), text(font), position(pos),
      settings(std::addressof(context.getSettings()))
{
    timer.start();
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
        fps = static_cast<double>(frameCount) /
              static_cast<double>(timeSinceUpdate);
        double avg_frame_ms = 1'000.F / fps;
        timeSinceUpdate = 0;
        frameCount = 0;
        std::string msg =
            "frame time (ms): " + std::to_string(avg_frame_ms) + "\n" +
            "Vsync: " + (settings->vsyncEnabled() ? "Yes" : "No") + "\n" +
            "FPS: " + std::to_string(fps);
        text.setText(msg);
    }
}

void FrameLabel::render(SDL_Renderer *renderer)
{
    text.renderText(position, renderer);
}
