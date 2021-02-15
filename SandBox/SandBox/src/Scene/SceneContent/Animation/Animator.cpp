#include "Animator.h"

namespace SceneContent
{
    Animator::Animator()
        : initFirst(bool()), firstAnimation("first animation not inited"), joints(Joints()), AnimationMap(std::unordered_map<std::string, Animation>()),
        priviousAnimation(std::string()), currentAnimation(std::string())
    {
    }
    Animator::~Animator()
    {
    }

    std::vector<glm::mat4> Animator::playAnimation(float deltaTime, AnimationSetter a/* = AnimationSetter()*/)
    {
        if (AnimationMap.find(a.current) != AnimationMap.end())
        {
            if (priviousAnimation != a.current)
            {
                AnimationMap[a.current].ResetDataMap();
                priviousAnimation = a.current;
            }
            if (AnimationMap[a.current].evaluate(deltaTime, a.loop))
                return playAnimation(deltaTime, { a.next , "", true });

            currentAnimation = a.current;

            joints.SetAnimationMatrixMap(AnimationMap[a.current].Update(deltaTime));
            for (const auto& parent : joints.GetPerents())
                joints.UpdateJointMatrix(parent);

            return joints.GetJointMatrix();
        }
        else if (a.current == firstAnimation)
        {
            return joints.GetStaticPose();
        }
        else
            return playAnimation(deltaTime, { firstAnimation , "", true });
    }

    void Animator::addAnimation(Animation animation)
    {
        if (!initFirst)
        {
            priviousAnimation = animation.name;
            firstAnimation = animation.name;
            currentAnimation = animation.name;
            initFirst = true;
        }
        std::string name_upper = std::string();
        for (const auto& c : animation.name)
            name_upper += std::toupper(c);

        if (name_upper == "IDLE")
        {
            priviousAnimation = animation.name;
            firstAnimation = animation.name;
            currentAnimation = animation.name;
        }
        if (AnimationMap.find(animation.name) != AnimationMap.end())
        {
            return;
        }
        AnimationMap[animation.name] = animation;
    }

    std::unordered_map<std::string, float> Animator::getAnimations()
    {
        std::unordered_map<std::string, float> a;
        for (const auto& animation : AnimationMap)
            a[animation.first] = animation.second.getMaxTime();

        return a;
    }
}