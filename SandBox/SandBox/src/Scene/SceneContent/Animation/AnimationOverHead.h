#pragma once
#include <iostream>
#include <string>

enum class SET_ANIMATION
{
    FORWARD,
    IDLE,
    JUMP,
    SHIFT,
};

struct AnimationSetter
{
    std::string current = std::string();
    std::string next = std::string();
    bool loop = true;
};

class AnimationOverHead
{
public:
	AnimationOverHead();
	~AnimationOverHead();

    bool input;
    bool idle;
    bool shift;
    bool transition;

    void setCurrentAnimation(std::string animation);
    void setCurrentAnimation(SET_ANIMATION type);

    void findNextAnimation();

    std::string getCurrentAnimation();

    AnimationSetter getAnimation(std::string current);


private:
    bool ProssesJump;

    AnimationSetter animations;
};

