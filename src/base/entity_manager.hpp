#pragma once

#include <iostream>
#include <unordered_set>
#include "base/object_base.hpp"

class EntityManager
{

  public:
    void registerEntity(ObjectBase &entity)
    {
        std::cout << "REGISTERING ENTITY" << '\n';
        auto *addr = std::addressof(entity);
        if (entities.contains(addr)) {
            return;
        }
        entities.insert(addr);
    }

    void unregisterEntity(ObjectBase &entity)
    {
        entities.erase(std::addressof(entity));
    }

    void updateEntites(float delta_t)
    {
        for (auto *entity : entities) {
            entity->update(delta_t);
        }
    }

    void renderEntities(SDL_Renderer *renderer)
    {
        for (auto *entity : entities) {
            entity->render(renderer);
        }
    }

  private:
    std::unordered_set<ObjectBase *> entities;
};