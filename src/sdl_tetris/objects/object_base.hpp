#pragma once

#include <SDL3/SDL.h>
#include "../texture.hpp"

class ObjectBase
{

  public:
    ObjectBase() = default;
    ObjectBase(const ObjectBase &) = default;
    ObjectBase(ObjectBase &&) = delete;
    ObjectBase &operator=(const ObjectBase &) = default;
    ObjectBase &operator=(ObjectBase &&) = delete;
    virtual ~ObjectBase() = default;

    // apply changes to object to objects position
    virtual void update(float delat_t) = 0;
    // render
    virtual void render(SDL_Renderer *renderer) = 0;
    virtual void handleUpdate(const SDL_Event &event) = 0;
    virtual Texture &getTexture() = 0;
};