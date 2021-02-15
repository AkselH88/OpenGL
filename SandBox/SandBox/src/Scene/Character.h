#pragma once

#include "Scene.h"

namespace Scene
{
	class Character : Scene
	{
	public:
		Character(const std::string& filepath);
		~Character() override;

		void Init() override;
		void OnUpdate(float deltaTime, Camera* camera) override;
		void OnDraw() override;

		void Unbind() override;
	private:
		SceneContentConteiner scene;

		/*########################## Shader ##########################*/
		Shader shader;
		ShaderProgramSource source;

		glm::vec3 translation;
		glm::vec3 scale;
		glm::mat4 model;

		glm::vec3 ambient = { 0.2f, 0.2f, 0.2f };
		glm::vec3 diffuse = { 0.5f, 0.5f, 0.5f };
		glm::vec3 specular = { 1.0f, 1.0f, 1.0f };
	};
}