#include "frame_label.hpp"

FrameLabel::FrameLabel(GameContext &context, Font &font, SDL_FPoint pos)
    : ObjectBase(context), text(font), position(pos)
{
    auto &input_manager = context.getInputManager();
    input_manager.bindKeyDown(this, SDLK_RETURN, [this]() {
        if (timer.isStarted()) {
            timer.stop();
        } else {
            timer.start();
        }
    });

    input_manager.bindKeyDown(this, SDLK_SPACE, [this]() {
        if (timer.isPaused()) {
            timer.unpause();
        } else {
            timer.pause();
        }
    });
};

FrameLabel::~FrameLabel()
{
    getInputManager().unbindAll(this);
}

void FrameLabel::update(float /*delat_t*/)
{
    text.setText("MS since start: " + std::to_string(timer.getTicksNS() / 1'000'000));
}

void FrameLabel::render(SDL_Renderer *renderer)
{
    text.renderText(position, renderer);
}
