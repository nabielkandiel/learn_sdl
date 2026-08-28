#pragma once

#include <cstdint>
#include <sstream>
#include "object_base.hpp"

class Timer : public ObjectBase
{
  private:
    uint64_t timer{0};
    std::stringstream time_text;

  public:
    Timer();

    void handleInput(const SDL_Event &input) override;
    void update(float delat_t) override;
};
