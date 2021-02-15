#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

#include "Window/Input.h"
#include "Scene/SceneContent/Animation/AnimationOverHead.h"

// TODO clean this up!

const float YAW = 90.0f;
const float PITCH = 15.0f;
const float SPEED = 4.0f;
const float SENSITIVITY = 0.2f;
const float ZOOM = 45.0f;

class Camera
{
public:
    glm::vec3 Position;
    glm::vec3 WorldPosition;
    glm::vec3 EyeWorld;
    glm::vec3 Front;
    glm::vec3 Eye;
    glm::vec3 Center;
    glm::vec3 Up;
    glm::vec3 Left;
    glm::vec3 WorldUp;

    // only need terrain hight.
    float terrain_hight = float();

    float Yaw;
    float Pitch;
    float Radius;

    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    bool ProssesJump;

    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
        
    void OnUpdate(float deltaTime, const KeyContent* key);
    float g = 78.68f;
    float v = 24.2f;
    float t = 0.0f;

    float time = 0;
    glm::vec3 worldLightPosition;
    glm::vec3 lightPosition;
    
    AnimationSetter getAnimation(std::string current);

    void setCurrentAnimation(SET_ANIMATION type);
    
    glm::mat4 GetWorldViewMatrix();
    
    glm::mat4 GetViewMatrix();

    glm::mat4 GetProjectionMatrix();

    glm::vec3 getLightPosition();
    glm::vec3 getWorldLightPosition();

    void ProcessLeftMouseMovement(float xoffset, float yoffset);

    void ProcessRightMouseMovement(float xoffset, float yoffset);

    void ProcessMouseScroll(float yoffset);

    void setWidthAndHeight(float width, float height) { Width = width; Height = height; }

private:
    AnimationOverHead animation;

    /* OnUpdate */
    void EventHandler(float deltaTime, const KeyContent* key);
    void prosessAnimation(float deltaTime);
    void CalculateLightPositin(float deltaTime);

    bool falling;

    float Height;
    float Width;

    float RightX;
    float RightZ;

    float RightYaw;
    void updateCameraVectors();
    glm::vec3 temp;
    void updateCameraVectorsOnLeftClick();

    void updateCameraVectorsOnRightClick();
};
