#pragma once

#include "Animation.h"
#include "AnimationOverHead.h"

namespace SceneContent
{
    class Animator
    {
    public:
        Animator();
        ~Animator();

        void addJoint(Joint& joint) { joints.AddJoint(joint); }

        void GlobalTransform() { for (const auto& parent : joints.GetPerents()) joints.CalcGlobalTransform(parent); }
        std::vector<glm::mat4> GetGlobalBindJoints() { return joints.getGlobalBindJoitsMatrix(); }

        std::string getCurrentAnimation() { return currentAnimation; }

        std::vector<glm::mat4> playAnimation(float deltaTime, AnimationSetter a = AnimationSetter());

        void addAnimation(Animation animation);

        std::unordered_map<std::string, float> getAnimations();

        int size_of_joints() { return joints.size(); }

    private:
        bool initFirst;
        std::string priviousAnimation;
        std::string firstAnimation;
        std::string currentAnimation;
        Joints joints;
        std::unordered_map<std::string, Animation> AnimationMap;
    };
}