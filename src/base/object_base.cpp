#include "object_base.hpp"
#include "base/entity_manager.hpp"
#include "base/game_context.hpp"

#include <memory>

ObjectBase::ObjectBase(GameContext &context)
    : entity_manager(std::addressof(context.getEntityManager())),
      input_manager(std::addressof(context.getInputManager()))
{
    entity_manager->registerEntity(*this);
}

ObjectBase::~ObjectBase()
{
    if (entity_manager != nullptr) {
        entity_manager->unregisterEntity(*this);
    }
}