#include "collidable_base.hpp"

#include "collision_manager.hpp"

CollidableBase::CollidableBase(CollisionManager &manager)
    : collision_manager(std::addressof(manager))
{
    collision_manager->add(this);
}

CollidableBase::~CollidableBase()
{
    collision_manager->remove(this);
}

[[nodiscard]] float CollidableBase::getRestitution() const
{
    return 1.F;
}

[[nodiscard]] SDL_FPoint CollidableBase::getVelocity() const
{
    return {.x = 0.F, .y = 0.F};
}
[[nodiscard]] uint32_t CollidableBase::getLayerMask() const
{
    return 0xFFFFFFFFU;
}
