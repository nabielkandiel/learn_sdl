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

enum class ButtonSprite : uint8_t
{
    NORMAL = 0,
    ACTIVE = 1,
    COUNT = 2,
};

enum class ShapeType : uint8_t
{
    CIRCLE = 0,
    AABB = 1,
};