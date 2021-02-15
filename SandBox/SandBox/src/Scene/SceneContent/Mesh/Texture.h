#pragma once
#include <string>
#include <vector>

namespace SceneContent
{
	/* data conteiner for Texture class */
	struct Sampler
	{
		int magFilter = int();
		int minFilter = int();
		int wrapS = int();
		int wrapT = int();
	};

	/* data conteiner for Texture class */
	struct Image
	{
		std::string path = std::string();

		std::string mimeType = std::string();

		Sampler sampler = Sampler();

		int Width = int();
		int Height = int();
		int BPP = int();

		bool retrivRenderID = bool();
		unsigned int imageID = int();
		unsigned int renderID = NULL;

		unsigned char* TextureBuffer = nullptr;
	};

	class Texture
	{
	public:
		Texture();
		~Texture();

		void AddTexture(std::string path);
		void AddTexture(std::string path, std::vector<unsigned char> data);
		void AddTextureOnLoad(std::string path);

		size_t size() { return images.size(); }

		std::string GetPath(unsigned int imageID);
		void SetImage(unsigned int imageID, Image image);
		Image GetImage(std::string path);

		bool Load(unsigned int imageID);
		void Bind() const;
		void Unbind() const;
	private:
		void Terminat();
		void TerminatImage(Image& image);
		bool PushGPU(unsigned char* buffer, int width, int height);

		std::vector<Image> images;
	};
}