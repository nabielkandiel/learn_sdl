#pragma once

#include <SDL3/SDL.h>

class EntityManager;
class InputManager;
struct GameContext;

class ObjectBase
{

  public:
    ObjectBase(GameContext &context);
    ObjectBase(const ObjectBase &) = default;
    ObjectBase(ObjectBase &&) = delete;
    ObjectBase &operator=(const ObjectBase &) = default;
    ObjectBase &operator=(ObjectBase &&) = delete;
    virtual ~ObjectBase();

    // apply changes to object to objects position
    virtual void update(float delat_t) = 0;
    virtual void render(SDL_Renderer *renderer) = 0;

  protected:
    [[nodiscard]] InputManager &getInputManager() const
    {
        return *input_manager;
    }

  private:
    EntityManager *entity_manager{nullptr};
    InputManager *input_manager{nullptr};
};