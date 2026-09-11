#include "collision_manager.hpp"

#include <algorithm>
#include <cmath>
#include <ranges>

void CollisionManager::add(CollidableBase *entity)
{
    entity->slot = entities.size();
    entities.push_back(entity);
}

void CollisionManager::remove(CollidableBase *entity)
{
    // pop and swap
    const size_t idx = entity->slot;
    CollidableBase *moved = entities.back();
    entities[idx] = moved;
    moved->slot = idx;
    entities.pop_back();
}

void CollisionManager::step(int iterations)
{
    for (int itter = 0; itter < iterations; ++itter) {
        size_t index = 0;
        for (CollidableBase *col_a : entities) {
            for (CollidableBase *col_b :
                 entities | std::ranges::views::drop(index + 1)) {
                if ((col_a->getLayerMask() & col_b->getLayerMask()) == 0U) {
                    continue;
                }
                Manifold mani =
                    collide(col_a->getCollider(), col_b->getCollider());
                if (mani.hit) {
                    resolve(*col_a, *col_b, mani);
                }
            }
            ++index;
        }
    }
}

void CollisionManager::resolve(CollidableBase &col_a, CollidableBase &col_b,
                               const Manifold &mani)
{
    const float inv_a = col_a.getInverseMass();
    const float inv_b = col_b.getInverseMass();
    const float inv_sum = inv_a + inv_b;
    if (inv_sum == 0.F) {
        return;
    }

    const float corr = mani.penetration / inv_sum;
    col_a.translate(
        {.x = (-mani.nnx * corr * inv_a), .y = (-mani.nny * corr * inv_a)});
    col_b.translate(
        {.x = (mani.nnx * corr * inv_b), .y = (mani.nny * corr * inv_b)});

    const SDL_FPoint vel_a = col_a.getVelocity();
    const SDL_FPoint vel_b = col_b.getVelocity();
    const float rel_n =
        ((vel_b.x - vel_a.x) * mani.nnx) + ((vel_b.y - vel_a.y) * mani.nny);
    if (rel_n > 0.F) {
        return;
    }

    const float energy =
        std::min(col_a.getRestitution(), col_b.getRestitution());
    const float joule = -(1.F + energy) * rel_n / inv_sum;

    col_a.setVelocity({.x = vel_a.x - (joule * inv_a * mani.nnx),
                       .y = vel_a.y - (joule * inv_a * mani.nny)});
    col_b.setVelocity({.x = vel_b.x + (joule * inv_b * mani.nnx),
                       .y = vel_b.y + (joule * inv_b * mani.nny)});
}

CollisionManager::Manifold CollisionManager::collide(const Collider &a_collider,
                                                     const Collider &b_collider)
{
    if (a_collider.type == ShapeType::CIRCLE &&
        b_collider.type == ShapeType::CIRCLE) {
        return circleCircle(a_collider, b_collider);
    }
    if (a_collider.type == ShapeType::CIRCLE &&
        b_collider.type == ShapeType::AABB) {
        return circleAABB(a_collider, b_collider);
    }
    if (a_collider.type == ShapeType::AABB &&
        b_collider.type == ShapeType::CIRCLE) {
        // NOLINTNEXTLINE readability-suspicious-call-argument
        auto mani = circleAABB(b_collider, a_collider);
        mani.nnx = -mani.nnx;
        mani.nny = -mani.nny;
        return mani;
    }
    return aabbAABB(a_collider, b_collider);
}

CollisionManager::Manifold
CollisionManager::circleCircle(const Collider &a_collider,
                               const Collider &b_collider)
{
    const SDL_FPoint circle_a = a_collider.circle.center_pos;
    const SDL_FPoint circle_b = b_collider.circle.center_pos;
    const float rad_a = a_collider.circle.radius;
    const float rad_b = b_collider.circle.radius;

    const float delta_x = circle_b.x - circle_a.x;
    const float delta_y = circle_b.y - circle_a.y;
    const float dist2 = (delta_x * delta_x) + (delta_y * delta_y);
    const float min_dist = rad_a + rad_b;

    Manifold mani; // m.hit defaults to false
    if (dist2 >= min_dist * min_dist) {
        return mani; // not touching
    }

    const float dist = std::sqrt(dist2);
    if (dist > 1e-8F) {
        mani.nnx = delta_x / dist;
        mani.nny = delta_y / dist;
    } else {
        // centres coincide exactly: no defined direction, pick an arbitrary
        // axis
        mani.nnx = 1.0F;
        mani.nny = 0.0F;
    }

    mani.hit = true;
    mani.penetration = min_dist - dist;
    return mani;
}

CollisionManager::Manifold CollisionManager::circleAABB(const Collider &circle,
                                                        const Collider &rect)
{
    const SDL_FPoint center = circle.circle.center_pos;
    const float radius = circle.circle.radius;
    const SDL_FPoint box_min{.x = rect.aabb.center_pos.x - rect.aabb.half.x,
                             .y = rect.aabb.center_pos.y - rect.aabb.half.y};
    const SDL_FPoint box_max{.x = rect.aabb.center_pos.x + rect.aabb.half.x,
                             .y = rect.aabb.center_pos.y + rect.aabb.half.y};

    // closest point on the box to the circle centre
    const float point_x = std::clamp(center.x, box_min.x, box_max.x);
    const float point_y = std::clamp(center.y, box_min.y, box_max.y);

    // circle centre -> closest point: this is the circle -> box direction
    const float delta_x = point_x - center.x;
    const float delta_y = point_y - center.y;
    const float dist2 = (delta_x * delta_x) + (delta_y * delta_y);

    Manifold mani;
    if (dist2 > radius * radius) {
        return mani; // no overlap
    }

    if (dist2 > 1e-8F) {
        const float dist = std::sqrt(dist2);
        mani.nnx = delta_x / dist;
        mani.nny = delta_y / dist;
        mani.penetration = radius - dist;
    } else {
        const float left = center.x - box_min.x;
        const float right = box_max.x - center.x;
        const float top = center.y - box_min.y;
        const float bottom = box_max.y - center.y;

        float min_pen = left;
        float outx = -1.0F;
        float outy = 0.0F;
        if (right < min_pen) {
            min_pen = right;
            outx = 1.F;
            outy = 0.F;
        }
        if (top < min_pen) {
            min_pen = top;
            outx = 0.F;
            outy = -1.F;
        }
        if (bottom < min_pen) {
            min_pen = bottom;
            outx = 0.F;
            outy = 1.F;
        }

        mani.nnx = -outx;
        mani.nny = -outy;
        mani.penetration = radius + min_pen;
    }

    mani.hit = true;
    return mani;
}
CollisionManager::Manifold CollisionManager::aabbAABB(const Collider &rect_a,
                                                      const Collider &rect_b)
{
    const float delta_x = rect_b.aabb.center_pos.x - rect_a.aabb.center_pos.x;
    const float delta_y = rect_b.aabb.center_pos.y - rect_a.aabb.center_pos.y;

    const float overlap_x =
        (rect_a.aabb.half.x + rect_b.aabb.half.x) - std::abs(delta_x);
    const float overlap_y =
        (rect_a.aabb.half.y + rect_b.aabb.half.y) - std::abs(delta_y);

    Manifold mani;
    if (overlap_x <= 0.0F || overlap_y <= 0.0F) {
        return mani; // separated on at least one axis
    }

    // separate along whichever axis has the SMALLER overlap — that's the
    // shortest way out
    if (overlap_x < overlap_y) {
        mani.nnx = (delta_x < 0.0F) ? -1.0F : 1.0F;
        mani.nny = 0.0F;
        mani.penetration = overlap_x;
    } else {
        mani.nnx = 0.0F;
        mani.nny = (delta_y < 0.0F) ? -1.0F : 1.0F;
        mani.penetration = overlap_y;
    }

    mani.hit = true;
    return mani;
}
