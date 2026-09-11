#pragma once

#include <SDL3/SDL_rect.h>

#include "common/CommonEnums.hpp"

class CollisionManager;

struct CircleShape
{
    SDL_FPoint center_pos;
    float radius;
};

struct AABBShape
{
    SDL_FPoint center_pos;
    SDL_FPoint half;
};

struct Collider
{
    ShapeType type;
    union {
        CircleShape circle;
        AABBShape aabb;
    };
};

class CollidableBase
{
  public:
    CollidableBase(CollisionManager &manager);
    virtual ~CollidableBase();

    CollidableBase(const CollidableBase &) = delete;
    CollidableBase(CollidableBase &&) = delete;
    CollidableBase &operator=(const CollidableBase &) = delete;
    CollidableBase &operator=(CollidableBase &&) = delete;

    [[nodiscard]] virtual Collider getCollider() const = 0;
    virtual void translate(SDL_FPoint delta) = 0;

    // physics stuff
    // 0 means not able to be moved
    [[nodiscard]] virtual float getInverseMass() const = 0;
    [[nodiscard]] virtual float getRestitution() const;
    [[nodiscard]] virtual SDL_FPoint getVelocity() const;
    virtual void setVelocity(SDL_FPoint /*unused*/) {}

    [[nodiscard]] virtual uint32_t getLayerMask() const;

  private:
    friend class CollisionManager;
    CollisionManager *collision_manager;
    size_t slot = 0; // updated by collision manager
};