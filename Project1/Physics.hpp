#pragma once
#include "Entity.hpp"
#include "vec2.hpp"

namespace Physics
{
    Vec2f GetOverLap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b)
    {
        // Ensure both entities have a CBoundingBox component
        if (!a->has<CBoundingBox>() || !b->has<CBoundingBox>())
        {
            return { 0.0f, 0.0f }; // No overlap if either entity lacks a bounding box
        }

        // Get the bounding box components
        auto& boxA = a->get<CBoundingBox>();
        auto& boxB = b->get<CBoundingBox>();

        // Get the positions of the entities
        auto& transformA = a->get<CTransform>();
        auto& transformB = b->get<CTransform>();

        Vec2f posA = transformA.pos;
        Vec2f posB = transformB.pos;

        // Calculate the overlap on the X and Y axes
        float overlapX = boxA.halfSize.x + boxB.halfSize.x - abs(posA.x - posB.x);
        float overlapY = boxA.halfSize.y + boxB.halfSize.y - abs(posA.y - posB.y);

        return { overlapX, overlapY };
    }


    Vec2f GetPreviousOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b)
    {
        // Ensure both entities have a CBoundingBox component
        if (!a->has<CBoundingBox>() || !b->has<CBoundingBox>())
        {
            return { 0.0f, 0.0f }; // No overlap if either entity lacks a bounding box
        }

        // Get the bounding box components
        auto& boxA = a->get<CBoundingBox>();
        auto& boxB = b->get<CBoundingBox>();

        // Get the previous positions of the entities
        auto& transformA = a->get<CTransform>();
        auto& transformB = b->get<CTransform>();

        Vec2f posA = transformA.prevPos;
        Vec2f posB = transformB.prevPos;

        float overlapX = boxA.halfSize.x + boxB.halfSize.x - abs(posA.x - posB.x);
        float overlapY = boxA.halfSize.y + boxB.halfSize.y - abs(posA.y - posB.y);

        return { overlapX, overlapY };
    }
};