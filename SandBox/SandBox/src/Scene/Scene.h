#pragma once

#include "SceneContent/Animation/Animator.h"
#include "SceneContent/Mesh/Meshes.h"
#include "SceneContent/Mesh/Texture.h"

#include "Shader/Shader.h"

#include "Camera.h"
#include "Terrain.h"

enum class ModelType
{
	OBJECT,
	TERRAIN,
	CHARACTER
};

namespace Scene
{
	struct SceneContentConteiner
	{
		SceneContent::Animator animator = SceneContent::Animator();
		SceneContent::Meshes meshes = SceneContent::Meshes();
	};

	class Scene
	{
	public:
		Scene();
		Scene(const std::string& filepath, bool playable, ModelType type);
		virtual ~Scene();

		virtual void Init();
		virtual void OnUpdate(float deltaTime, Camera* camera);
		virtual void OnDraw();

		virtual void Unbind();
	private:
		ModelType type;

		bool joints;
		bool playable;
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