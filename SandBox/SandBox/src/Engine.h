#pragma once
#include "Window/Window.h"
#include "Scene/Scene.h"
#include "Scene/Level.h"
#include "Scene/Object.h"
#include "Scene/Character.h"

class Engine
{
public:
	Engine();
	~Engine();

	void Run();
	int Turmenate();
private:
	
	int err;

	Window window;
	Scene::Level* world;
	Scene::Character* player;
};