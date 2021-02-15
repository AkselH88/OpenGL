#pragma once

#include "Scene.h"

namespace Scene
{
	class Object : Scene
	{
	public:
		Object(const std::string& filepath);
		~Object() override;

		void Init() override;
		void OnUpdate(float deltaTime, Camera* camera) override;
		void OnDraw() override;

		void Unbind() override;
	private:
		SceneContentConteiner scene;

		/*########################## Shader ##########################*/
		Shader shader;
		ShaderProgramSource source;

		glm::vec3 scale;

		glm::vec3 ambient = { 0.2f, 0.2f, 0.2f };
		glm::vec3 diffuse = { 0.5f, 0.5f, 0.5f };
		glm::vec3 specular = { 1.0f, 1.0f, 1.0f };
	};
}