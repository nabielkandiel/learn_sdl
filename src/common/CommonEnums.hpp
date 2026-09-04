#pragma once

#include <cstdint>

enum class Textures : uint8_t
{
    BALL = 0,
    ARROW = 1,
    RESET_BUTTON = 2,
};

enum class Fonts : uint8_t
{
    MONTSERRAT = 0,
};

enum class State : uint8_t
{
    PLAYING = 0,
    PAUSED = 1,
};

enum class InputState : uint8_t
{
    GLOBAL = 0,
    PLAYING = 1,
    PAUSED = 2,
};