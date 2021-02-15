#pragma once

#include "Mesh.h"
#include "Texture.h"

namespace SceneContent
{
	class Meshes
	{
	public:
		Meshes();
		~Meshes();

		void AddMesh(std::string name, Mesh& mesh);
		void AddTexture(std::string name, std::string path);
		void AddTexture(std::string name, std::string path, std::vector<unsigned char>& data);

		void Load();

		void Bind();
		void Unbind();

		void Draw();

		const std::vector<float> GetPrimetivePosition(std::string name);
	private:
		void AddTextureOnLoad(std::string name, std::string path);
		std::unordered_map<std::string, Mesh> MeshMap;
		std::unordered_map<std::string, Texture> TextureMap;

		/* std::unordered_map<path, mesh> TextureLocationMap */
		std::unordered_map<std::string, std::string> TextureLocationMap;
	};
}