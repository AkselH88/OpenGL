#include "Meshes.h"
#include <iostream>

namespace SceneContent
{
	Meshes::Meshes()
		: MeshMap(std::unordered_map<std::string, Mesh>()), TextureMap(std::unordered_map<std::string, Texture>()), TextureLocationMap(std::unordered_map<std::string, std::string>())
	{

	}

	Meshes::~Meshes()
	{

	}

	void Meshes::AddMesh(std::string name, Mesh& mesh)
	{
		if (MeshMap.find(name) != MeshMap.end())
		{
			return;
		}
		MeshMap[name] = mesh;
	}

	void Meshes::AddTexture(std::string name, std::string path)
	{
		if (TextureLocationMap.find(path) != TextureLocationMap.end())
		{
			AddTextureOnLoad(name, path);
		}
		else
		{
			if (TextureMap.find(name) != TextureMap.end())
			{
				std::cout << name << std::endl;
				TextureMap[name].AddTexture(path);
			}
			else
			{
				TextureMap[name] = Texture();
				std::cout << name << ": new!" << std::endl;
				TextureMap[name].AddTexture(path);
			}

			TextureLocationMap[path] = name;
		}
	}

	void Meshes::AddTexture(std::string name, std::string path, std::vector<unsigned char> data)
	{
		if (TextureLocationMap.find(path) != TextureLocationMap.end())
		{
			AddTextureOnLoad(name, path);
		}
		else
		{
			if (TextureMap.find(name) != TextureMap.end())
			{
				std::cout << name << std::endl;
				TextureMap[name].AddTexture(path, data);
			}
			else
			{
				TextureMap[name] = Texture();
				std::cout << name << ": new!" << std::endl;
				TextureMap[name].AddTexture(path, data);
			}

			TextureLocationMap[path] = name;
		}
	}

	void Meshes::AddTextureOnLoad(std::string name, std::string path)
	{
		if (TextureMap.find(name) != TextureMap.end())
		{
			//std::cout << name << ": add on LOAD" << std::endl;
			TextureMap[name].AddTextureOnLoad(path);
		}
		else
		{
			TextureMap[name] = Texture();
			//std::cout << name << ": new! add on LOAD" << std::endl;
			TextureMap[name].AddTextureOnLoad(path);
		}
	}

	void Meshes::Load()
	{
		for (auto& mesh : MeshMap)
		{
			for (size_t i = 0; i < TextureMap[mesh.first].size(); i++)
			{
				if (!TextureMap[mesh.first].Load(i))
				{
					if (TextureLocationMap.find(TextureMap[mesh.first].GetPath(i)) != TextureLocationMap.end())
					{
						TextureMap[mesh.first].SetImage(i, TextureMap[TextureLocationMap[TextureMap[mesh.first].GetPath(i)]].GetImage(TextureMap[mesh.first].GetPath(i)));
					}
				}
			}
			
			mesh.second.Load();
		}
	}

	void Meshes::Bind()
	{
		for (const auto& mesh : MeshMap)
		{
			TextureMap[mesh.first].Bind();
			mesh.second.Bind();
		}
	}

	void Meshes::Unbind()
	{
		for (const auto& mesh : MeshMap)
		{
			TextureMap[mesh.first].Unbind();
			mesh.second.Unbind();
		}
	}

	void Meshes::Draw()
	{
		for (const auto& mesh : MeshMap)
		{
			TextureMap[mesh.first].Bind();
			mesh.second.Draw();
		}
	}

	const std::vector<float> Meshes::GetPrimetivePosition(std::string name)
	{
		if (MeshMap.find(name) != MeshMap.end())
		{
			return MeshMap[name].GetPrimetive("POSITION");
		}
		else
			return std::vector<float>();
	}
}