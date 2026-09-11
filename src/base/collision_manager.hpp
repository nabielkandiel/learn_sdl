#pragma once

#include <vector>
#include "base/collidable_base.hpp"

class CollisionManager
{

  private:
    std::vector<CollidableBase *> entities;

    struct Manifold
    {
        bool hit = false;
        // pointing from a to b
        float nnx = 0.F;
        float nny = 0.F;
        float penetration = 0.F;
    };

    static Manifold circleCircle(const Collider &a_collider,
                                 const Collider &b_collider);
    static Manifold circleAABB(const Collider &circle, const Collider &rect);
    static Manifold aabbAABB(const Collider &rect_a, const Collider &rect_b);

    static Manifold collide(const Collider &a_collider,
                            const Collider &b_collider);

    static void resolve(CollidableBase &col_a, CollidableBase &col_b,
                        const Manifold &mani);

  public:
    void add(CollidableBase *entity);
    void remove(CollidableBase *entity);
    void step(int iterations = 4);
};