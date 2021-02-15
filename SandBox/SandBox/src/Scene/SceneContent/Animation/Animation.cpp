#include "Animation.h"

namespace SceneContent
{
    Animation::Animation()
        : name(std::string()), inputTime(std::vector<float>()), maxTime(float())
    {}

    Animation::~Animation()
    {}
    
    void Animation::addTranslation(std::string name, std::vector<float>& translation)
    {
        for (size_t i = 0; i < translation.size(); i += 3)
        {
            AnimationDataMap[name].translation.emplace_back(translation[i], translation[i + 1], translation[i + 2]);
        }
    }

    void Animation::addQuaternion(std::string name, std::vector<float>& rotation)
    {
        for (size_t i = 0; i < rotation.size(); i += 4)
        {
            AnimationDataMap[name].rotation.emplace_back(Quaternion(rotation[i], rotation[i + 1], rotation[i + 2], rotation[i + 3]));
        }
    }

    void Animation::addScale(std::string name, std::vector<float>& scale)
    {
        for (size_t i = 0; i < scale.size(); i += 3)
        {
            AnimationDataMap[name].scale.emplace_back(scale[i], scale[i + 1], scale[i + 2]);
        }
    }
    
    void Animation::ResetDataMap()
    {
        for (auto& data : AnimationDataMap)
        {
            data.second.currentTime = float();
            data.second.nextTime = float();
            data.second.iterationCount = size_t();
        } 
    }

    bool Animation::evaluate(float deltaTime, bool loop)
    {
        for (auto& updating : AnimationDataMap)
        {
            if (!loop)
            {
                if (!controlTime(updating.second, deltaTime))
                {
                    if (updating.second.currentTime > updating.second.nextTime)
                        UpdatePreviousAndNextsTime(updating.second);

                    interpolat(updating.second);
                }
                else
                    return true;
            }
            else
                time(updating.second, deltaTime);

            forUpdate[updating.first] = TRS_Matrix(translation_Matrix(updating.second.currentTranslation),
                updating.second.currentQuat.toRotationMatrix(),
                scale_Matrix(updating.second.currentScale));
        }

        return false; 
    }

    bool Animation::controlTime(AnimationData& data, float deltaTime)
    {
        data.currentTime += deltaTime;

        if (data.currentTime >= maxTime)
        {
            return true;
        }

        return false;
    }

    void Animation::time(AnimationData& data, float deltaTime)
    {
        data.currentTime += deltaTime;

        if (data.currentTime >= maxTime)
        {
            data.currentTime = data.currentTime - maxTime;
            data.iterationCount = 0;
            UpdatePreviousAndNextsTime(data);
        }
        else if (data.currentTime > data.nextTime)
            UpdatePreviousAndNextsTime(data);

        interpolat(data);
    }

    void Animation::UpdatePreviousAndNextsTime(AnimationData& data)
    {
        data.previousTime = inputTime[data.iterationCount];
        data.nextTime = inputTime[data.iterationCount + 1];

        data.previousTranslation = data.translation[data.iterationCount];
        data.nextTranslation = data.translation[data.iterationCount + 1];

        data.previousQuat = data.rotation[data.iterationCount];
        data.nextQuat = data.rotation[data.iterationCount + 1];

        data.previousScale = data.scale[data.iterationCount];
        data.nextScale = data.scale[data.iterationCount + 1];
        
        data.iterationCount++;
    }

    void Animation::interpolat(AnimationData& data)
    {
        data.interpolationValue = (data.currentTime - data.previousTime) / (data.nextTime - data.previousTime);

        data.currentTranslation = data.previousTranslation + data.interpolationValue * (data.nextTranslation - data.previousTranslation);
        data.currentQuat = data.currentQuat.slerp(data.previousQuat, data.nextQuat, data.interpolationValue);
        data.currentScale = data.previousScale + data.interpolationValue * (data.nextScale - data.previousScale);
    }

    std::unordered_map<std::string, glm::mat4>& Animation::Update(float deltaTime)
    {
        /*for (auto& updating : AnimationDataMap)
        {
            time(updating.second, deltaTime);

            forUpdate[updating.first] = TRS_Matrix(translation_Matrix(updating.second.currentTranslation),
                updating.second.currentQuat.toRotationMatrix(),
                scale_Matrix(updating.second.currentScale));
        }*/
        return forUpdate;
    }
}