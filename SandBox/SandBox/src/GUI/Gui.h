#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "imGui/imgui.h"
#include "imGui/imgui_impl_glfw.h"
#include "imGui/imgui_impl_opengl3.h"

class Gui
{
public:
	Gui(GLFWwindow* window);
	~Gui();

	void OnUpdate();
	void OnRender();
private:
};