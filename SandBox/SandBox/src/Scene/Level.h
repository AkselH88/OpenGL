#pragma once

#include "Scene.h"

#include "Terrain.h"

namespace Scene
{
	class Level : Scene
	{
	public:
		Level(const std::string& filepath);
		~Level() override;

		void Init() override;
		void OnUpdate(float deltaTime, Camera* camera) override;
		void OnDraw() override;

		void Unbind() override;
	private:
		Terrain terrain;
		SceneContentConteiner scene;

		/*########################## Shader ##########################*/
	private:
		Shader shader;
		ShaderProgramSource source;

		glm::vec3 scale;

		glm::vec3 ambient = { 0.2f, 0.2f, 0.2f };
		glm::vec3 diffuse = { 0.5f, 0.5f, 0.5f };
		glm::vec3 specular = { 1.0f, 1.0f, 1.0f };
	};
}