#include "Camera.h"

#include "imGui/imgui.h"

#include "Window/Window.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : Front(glm::vec3(0.0f, 0.0f, -1.0f)), EyeWorld(glm::vec3(0.0f)),
RightX(cos(glm::radians(YAW))), RightZ(sin(glm::radians(YAW))), RightYaw(0.0f), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM),
falling(false), ProssesJump(false), Center(glm::vec3(0.0f, 1.7f, 0.0f)), animation(AnimationOverHead()), Width(WIDTH), Height(HEIGHT)
{
    Eye = Front;
    Position = position;
    WorldPosition = position;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
    Radius = 6.0f;
    updateCameraVectors();
}

void Camera::OnUpdate(float deltaTime, const KeyContent* key)
{
    EventHandler(deltaTime, key);

    CalculateLightPositin(deltaTime);
    prosessAnimation(deltaTime);
}
void Camera::EventHandler(float deltaTime, const KeyContent* key)
{
    float velocity = MovementSpeed * deltaTime;

    if (key->W)
    {
        animation.setCurrentAnimation(SET_ANIMATION::FORWARD);
        WorldPosition -= Front * velocity;
    }
    if (key->S)
    {
        WorldPosition += Front * velocity;
    }
    if (key->A)
    {
        WorldPosition += Left * velocity;
    }
    if (key->D)
    {
        WorldPosition -= Left * velocity;
    }
    if (key->SPACE)
    {
        animation.setCurrentAnimation(SET_ANIMATION::JUMP);
        if (!ProssesJump)
            ProssesJump = true;
    }
    if (key->SHIFT)
    {
        if (!animation.transition)
            animation.setCurrentAnimation(SET_ANIMATION::SHIFT);
    }
    
}
int test = 0;
void Camera::prosessAnimation(float deltaTime)
{
    ImGui::Text("World cordinets: (x: %.03f, y: %.03f, z: %.03f)", WorldPosition.x, WorldPosition.y, WorldPosition.z);
    /* All this needs to get re writhen when collision is added */

    ImGui::Text("model in world y: %.03f, world y: %.03f)", WorldPosition.y, terrain_hight);

    ImGui::Text("falling: %i)", falling);
    ImGui::Text("ProssesJump: %i)", ProssesJump);
    ImGui::Text("!(falling || ProssesJump): %i)", !(falling || ProssesJump));
    ImGui::Text("test: %i)", test);

    if (ProssesJump)
    {
        if (WorldPosition.y <= terrain_hight && animation.getCurrentAnimation() == "FALL")
        {
            test = 3;
            t = 0;
            WorldPosition.y = terrain_hight;
            animation.setCurrentAnimation("LAND");

            falling = false;
            ProssesJump = false;

            Front = glm::normalize(glm::vec3(temp.x, 0, temp.z) / (Radius * cos(glm::radians(Pitch))));
        }
        else if ((animation.getCurrentAnimation() == "JUMP" || animation.getCurrentAnimation() == "FALL"))
        {
            test = 4;
            t += deltaTime;
            WorldPosition.y += (v * t - 0.5f * g * t * t) * deltaTime;
        }
        else
        {
            test = 2;
            t = 0;
            WorldPosition.y = terrain_hight;
        }
    }
    else if (WorldPosition.y > (terrain_hight + 0.03f))
    {
        test = 1;
        t += deltaTime;
        WorldPosition.y += (-0.5f * g * t * t) * deltaTime;

        //test = 2;
        if (!falling)
        {
            animation.setCurrentAnimation("FALL");
            falling = true;
        }
    }
    else
    {
        test = 0;
        if (falling)
        {
            animation.setCurrentAnimation("LAND");
            falling = false;
            t = 0;
        }
        WorldPosition.y = terrain_hight;
    }
    /*
    if ((WorldPosition.y <= terrain.y+0.01f) && !(falling || ProssesJump))
    {
        test = 0;
        WorldPosition.y = terrain.y;
    }
    else if (falling && !ProssesJump && !(WorldPosition.y <= terrain.y))
    {
        test = 1;
        t += deltaTime;
        WorldPosition.y += (-0.5f * g * t * t) * deltaTime;
    }
    else
    {
        if (WorldPosition.y > terrain.y && !(falling || ProssesJump))
        {
            test = 2;
            animation.setCurrentAnimation("FALL");
            falling = true;
        }
        else if (WorldPosition.y <= terrain.y && animation.getCurrentAnimation() == "FALL")
        {
            test = 3;
            t = 0;
            WorldPosition.y = terrain.y;
            animation.setCurrentAnimation("LAND");

            falling = false;
            ProssesJump = false;

            Front = glm::normalize(glm::vec3(temp.x, 0, temp.z) / (Radius * cos(glm::radians(Pitch))));
        }
        else if ((animation.getCurrentAnimation() == "JUMP" || animation.getCurrentAnimation() == "FALL") && ProssesJump)
        {
            test = 4;
            t += deltaTime;
            WorldPosition.y += (v * t - 0.5f * g * t * t) * deltaTime;
        }
        else if (WorldPosition.y <= terrain.y && (falling || ProssesJump) && animation.idle)
        {
            test = 5;
            animation.setCurrentAnimation("LAND");
            falling = false;
            ProssesJump = false;
        }
        else
        {
            falling = false;
            ProssesJump = false;
            test = 6;
        }
    }*/
}

AnimationSetter Camera::getAnimation(std::string current)
{
    return animation.getAnimation(current);
}

void Camera::setCurrentAnimation(SET_ANIMATION type)
{
    animation.setCurrentAnimation(type);
}


glm::mat4 Camera::GetWorldViewMatrix()
{
    return glm::lookAt(WorldPosition + EyeWorld, WorldPosition + Center, Up);
}

glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(Eye, Center, glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::mat4 Camera::GetProjectionMatrix()
{
    return glm::perspective(glm::radians(Zoom), Width / Height, .1f, 100.0f);
}

glm::vec3 Camera::getLightPosition()
{ 
    return lightPosition;
}

glm::vec3 Camera::getWorldLightPosition()
{
    return worldLightPosition;
}

void Camera::ProcessLeftMouseMovement(float xoffset, float yoffset)
{
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch -= yoffset;

    if (Pitch > 89.0f)
        Pitch = 89.0f;
    else if (Pitch < -89.0f)
        Pitch = -89.0f;

    updateCameraVectorsOnLeftClick();
}

void Camera::ProcessRightMouseMovement(float xoffset, float yoffset)
{
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch -= yoffset;

    if (Pitch > 89.0f)
        Pitch = 89.0f;
    else if (Pitch < -89.0f)
        Pitch = -89.0f;

    updateCameraVectorsOnRightClick();
}

void Camera::ProcessMouseScroll(float yoffset)
{
    if (Radius >= 4.0f && Radius <= 20.0f)
        Radius -= yoffset;
    if (Radius <= 4.0f)
        Radius = 4.0f;
    if (Radius >= 20.0f)
        Radius = 20.0f;

    updateCameraVectorsOnLeftClick();
}

void Camera::updateCameraVectors()
{
    glm::vec3 front;

    front.x = cos(glm::radians(Yaw));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw));

    Front = glm::normalize(glm::vec3(front.x, 0.0, front.z));
    front.x = front.x * cos(glm::radians(Pitch));
    front.z = front.z * cos(glm::radians(Pitch));

    Eye = Radius * glm::vec3(   cos(glm::radians(Yaw)) * cos(glm::radians(Pitch)),
                                sin(glm::radians(Pitch)),
                                sin(glm::radians(Yaw)) * cos(glm::radians(Pitch)));

    EyeWorld = Radius * front;
    temp = EyeWorld;

    Left = glm::normalize(glm::cross(glm::normalize(front), WorldUp));
    Up = glm::normalize(glm::cross(Left, glm::normalize(front)));
}

void Camera::updateCameraVectorsOnLeftClick()
{
    glm::vec3 front;

    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));

    Eye = Radius * glm::vec3(   cos(glm::radians(Yaw - RightYaw)) * cos(glm::radians(Pitch)),
                                sin(glm::radians(Pitch)),
                                sin(glm::radians(Yaw - RightYaw)) * cos(glm::radians(Pitch)));

    EyeWorld = Radius * front;

    Up = glm::normalize(glm::cross(glm::normalize(glm::cross(glm::normalize(front), WorldUp)), glm::normalize(front)));
}

void Camera::updateCameraVectorsOnRightClick()
{
    // Calculate the new Front vector
    glm::vec3 front;

    front.x = cos(glm::radians(Yaw));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw));

    if (!(falling || ProssesJump))
        Front = glm::normalize(glm::vec3(front.x, 0.0, front.z));

    RightYaw = Yaw - YAW;

    front.x = front.x * cos(glm::radians(Pitch));
    front.z = front.z * cos(glm::radians(Pitch));

    Eye = Radius * glm::vec3(   RightX * cos(glm::radians(Pitch)),
                                sin(glm::radians(Pitch)),
                                RightZ * cos(glm::radians(Pitch)));

    EyeWorld = Radius * front;
    temp = EyeWorld;

    Left = glm::normalize(glm::cross(glm::normalize(front), WorldUp));
    Up = glm::normalize(glm::cross(Left, glm::normalize(front)));
}

void Camera::CalculateLightPositin(float deltaTime)
{
    if (time >= 360)
        time = 0;
    time += deltaTime;

    glm::vec3 light;
    float r = 100;
    light.x = cos(glm::radians(time));
    light.y = sin(glm::radians(time));
    light.z = 0;

    worldLightPosition = r * (light);

    light.x = cos(glm::radians(-RightYaw)) * cos(glm::radians(time));
    //light.y = sin(glm::radians(time));
    light.z = sin(glm::radians(-RightYaw)) * cos(glm::radians(time));

    lightPosition = r * (light);
    ImGui::Text("worldLightPosition x: %.03f", worldLightPosition.x);
    ImGui::Text("lightPosition y: %.03f", lightPosition.y);
}