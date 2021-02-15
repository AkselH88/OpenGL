#pragma once

#include "Joints.h"

enum class AnimationTarget
{
    MATRIX,
    TRANSLATION,
    ROTATION,
    SCALE,
    MORPH,
    COUNT
};

struct AnimationData
{
    std::vector<glm::vec3> translation = std::vector<glm::vec3>();
    std::vector<Quaternion> rotation = std::vector<Quaternion>();
    std::vector<glm::vec3> scale = std::vector<glm::vec3>();

    Quaternion previousQuat = Quaternion();
    Quaternion currentQuat = Quaternion();
    Quaternion nextQuat = Quaternion();

    glm::vec3 previousScale = glm::vec3();
    glm::vec3 currentScale = glm::vec3();
    glm::vec3 nextScale = glm::vec3();

    glm::vec3 previousTranslation = glm::vec3();
    glm::vec3 currentTranslation = glm::vec3();
    glm::vec3 nextTranslation = glm::vec3();

    float previousTime = float();
    float currentTime = float();
    float nextTime = float();
    float interpolationValue = float();

    size_t iterationCount = size_t();
};

namespace SceneContent
{	
    class Animation
	{
    public:
        Animation();
        ~Animation();

        std::string name;

        void addInputTime(std::vector<float> time) { inputTime = time; }
        void addMaxTime(float time) { maxTime = time; }
        float getMaxTime() const { return maxTime; }

        void addTranslation(std::string name, std::vector<float>& translation);
        void addQuaternion(std::string name, std::vector<float>& rotation);
        void addScale(std::string name, std::vector<float>& scale);

        bool evaluate(float deltaTime, bool loop);
        bool controlTime(AnimationData& data, float deltaTime);

        std::unordered_map<std::string, glm::mat4>& Update(float deltaTime);
        void ResetDataMap();

    private:
        void time(AnimationData& data, float deltaTime);
        void UpdatePreviousAndNextsTime(AnimationData& data);
        void interpolat(AnimationData& data);

        std::unordered_map<std::string, AnimationData> AnimationDataMap;
        std::vector<float> inputTime;

        float maxTime;
        
        std::unordered_map<std::string, glm::mat4> forUpdate;
        std::unordered_map<std::string, Joint> animatedJoint;
	};
}