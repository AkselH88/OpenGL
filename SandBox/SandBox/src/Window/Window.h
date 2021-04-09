#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "GUI/Gui.h"

#include "Input.h"
#include "Camera.h"
#include "Renderer.h"

constexpr auto WIDTH = 800;
constexpr auto HEIGHT = 600;

class Window
{
public:
	Window();
	~Window();

	void OnTerminate();
	bool InitOK() { return data.Initialized; }

	bool IsOpen();

	void SetVSync(bool vsync);
	bool IsVSync() { return vSync; }

	float FrameTime() { return data.deltaTime; }

	void OnUpdate();
	void OnRender();

	Camera* GetCamera() { return &data.camera; }

private:
	void OnEventInit();
	void setWindowHint();
	void updateTime();

	struct WindowContentConteiner
	{
		std::string Title = std::string();

		unsigned int Width = NULL;
		unsigned int Height = NULL;

		bool Initialized = bool();
		bool IsOpen = bool();

		float deltaTime = float();
		float lastFrame = float();

		MouseContent mouse = MouseContent();
		void processMouseOnMove(float xpos, float ypos);

		KeyContent buttons = KeyContent();
		void processKeyOnPress(int key);
		void processKeyOnRelease(int key);

		Camera camera;
	};

	bool vSync;
	WindowContentConteiner data;

	GLFWwindow* m_Window;
	Gui* m_Gui;

	Renderer renderer;
};