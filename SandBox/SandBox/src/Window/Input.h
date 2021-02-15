#pragma once

struct MouseContent
{
	float lastX = float();
	float lastY = float();

	bool firstClick = bool();
	bool Left = bool();
	bool Right = bool();
};

struct KeyContent
{
	bool A = bool();
	bool D = bool();
	bool S = bool();
	bool W = bool();

	bool SPACE = bool();
	bool SHIFT = bool();
	bool shiftPressed = bool();
	bool Pressed = bool();
};