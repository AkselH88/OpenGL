#include "AnimationOverHead.h"

AnimationOverHead::AnimationOverHead()
	:animations(AnimationSetter()), shift(bool()), ProssesJump(bool()), input(bool()), transition(bool()), idle(bool())
{
	animations.current = "IDLE";
	animations.next = "IDLE";
}
AnimationOverHead::~AnimationOverHead()
{}

void AnimationOverHead::setCurrentAnimation(std::string animation)
{
    this->animations.current = animation;
    findNextAnimation();
}

std::string AnimationOverHead::getCurrentAnimation()
{
    return animations.current;
}

void AnimationOverHead::setCurrentAnimation(SET_ANIMATION type)
{

    if (!ProssesJump && !transition)
    {
        input = true;
        if (type == SET_ANIMATION::FORWARD)
        {
            if (shift)
                animations.current = "CROUL";
            else
                animations.current = "RUN";
            findNextAnimation();
        }
        else if (type == SET_ANIMATION::IDLE)
        {
            if (shift)
                animations.current = "CROUTCH_IDLE";
            else
                animations.current = "IDLE";
            findNextAnimation();
        }
        else if (type == SET_ANIMATION::SHIFT)
        {
            shift = !shift;

            if (shift)
                animations.current = "CROUTCH";
            else
                animations.current = "RISE";
            findNextAnimation();
        }
        else if (type == SET_ANIMATION::JUMP)
        {
            if (shift)
                animations.current = "JUMP";
            else
                animations.current = "CROUTCH";
            findNextAnimation();
        }
    }
}
void AnimationOverHead::findNextAnimation()
{
    if (animations.current == "IDLE")
    {
        idle = true;
        animations.loop = true;
    }
    else if (animations.current == "RUN")
    {
        idle = false;
        animations.loop = true;
    }

    else if (animations.current == "JUMP")
    {
        ProssesJump = true;
        idle = false;
        animations.loop = false;
        animations.next = "FALL";
    }
    else if (animations.current == "LAND")
    {
        ProssesJump = false;
        idle = false;
        animations.loop = false;
        if (shift)
            animations.next = "CROUTCH_IDLE";
        else
            animations.next = "RISE";
    }
    else if (animations.current == "RISE")
    {
        idle = false;
        animations.loop = false;
        animations.next = "IDLE";
    }
    else if (animations.current == "FALL")
    {
        ProssesJump = true;
        idle = false;
        animations.loop = true;
        animations.next = "LAND";
    }
    else if (animations.current == "CROUL")
    {
        idle = false;
        animations.loop = true;
    }
    else if (animations.current == "CROUTCH")
    {
        idle = false;
        animations.loop = false;
        if (shift)
            animations.next = "CROUTCH_IDLE";
        else
            animations.next = "JUMP";
    }
    else if (animations.current == "CROUTCH_IDLE")
    {
        idle = true;
        animations.loop = true;
    }
}

AnimationSetter AnimationOverHead::getAnimation(std::string current)
{
    if (animations.current != current && input && !animations.loop || animations.current != current && transition)
    {
        if (!transition)
        {
            transition = true;
        }
        else if (!ProssesJump && current == "FALL");

        else
            animations.current = current;

        findNextAnimation();

        if (idle)
        {
            if (!ProssesJump)
            {
                transition = false;
            }
        }

        input = false;
    }

    return animations;
}