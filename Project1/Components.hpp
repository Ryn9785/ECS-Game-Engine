#pragma once

#include "vec2.hpp"
#include "Animation.hpp"
#include "Assets.hpp"
#include <SFML/Graphics.hpp>
//done!
class Component
{
public:
    bool exists = false;
};

class CTransform : public Component
{
public:
    Vec2f pos = { 0.0f, 0.0f };   // Fixed: explicit 'f' for floating-point initialization
    Vec2f prevPos = { 0.0f, 0.0f };   // Fixed: explicit 'f' for floating-point initialization
    Vec2f velocity = { 0.0f, 0.0f };   // Fixed: explicit 'f' for floating-point initialization
    float angle = 0;
    Vec2f scale = { 1.0f,1.0f };

    CTransform() = default;
    CTransform(const Vec2f& p)
        :pos(p),prevPos(p) {}
    CTransform(const Vec2f& p, const Vec2f& sp, float a, const Vec2f& sc)
        : pos(p),prevPos(p), velocity(sp), angle(a), scale(sc) {
    }
};

class CLifespan : public Component
{
public:
    int lifespan = 0;
    int frameCreated = 0;

    CLifespan() = default;

    // Added flexibility to set different remaining lifespan, if needed
    CLifespan(int duration, int frame)
        : lifespan(duration), frameCreated(frame) {
    }
};

class CInput : public Component
{
public:
    bool up = false;
    bool left = false;
    bool right = false;
    bool down = false;
    bool shoot = false;
    bool canShoot = true;
    bool canJump = true;

    CInput() = default;

};

class CBoundingBox : public Component
{
public:
    Vec2f size = { 64.0f, 64.0f };         // Define size with x and y components
    Vec2f halfSize = { 32.0f, 32.0f };     // Define halfSize with x and y components
    CBoundingBox() = default;
    CBoundingBox(const Vec2f& Size)
        :size(Size), halfSize(Size.x / 2,Size.y/2) {
    }
};

//CAnimation,
//CGravity,
//CState
class CAnimation : public Component
{
public:
    Animation animation;
    bool repeat = false;
    CAnimation() = default;
    CAnimation(const Animation& animation, bool r)
        : animation(animation), repeat(r) {
    }
};

class CGravity : public Component
{
public:
    float gravity = 0;
    CGravity() = default;
    CGravity(float g) :gravity(g) {}
};
class CState : public Component
{
public:
    std::string state = "Stand";
    CState() = default;
    CState(std::string s)
        :state(s) {
    }
};