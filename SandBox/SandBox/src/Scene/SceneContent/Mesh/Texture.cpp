#include "Texture.h"

#include <iostream>

#include <GL/glew.h>
#include "stb_image/stb_image.h"


namespace SceneContent
{
	Texture::Texture()
		:images(std::vector<Image>())
	{

	}
	Texture::~Texture()
	{
        Terminat();
	}

    void Texture::Terminat()
    {
        for (auto& image : images)
        {
            glDeleteTextures(1, &image.renderID);
        }
        images.erase(images.begin(), images.end());
    }

    void Texture::TerminatImage(Image& image)
    {
        glDeleteTextures(1, &image.renderID);
        glBindTexture(GL_TEXTURE_2D, 0);
        images.erase(images.begin() + image.imageID);

        for (size_t i = 0; i < images.size(); i++)
        {
            images[i].imageID = i;
        }
    }

	void Texture::AddTexture(std::string path)
	{
        Image image = Image();

        image.imageID = images.size();
        image.path = path;;

        image.TextureBuffer = stbi_load(image.path.c_str(), &image.Width, &image.Height, &image.BPP, 4);
        images.push_back(image);
	}

    void Texture::AddTexture(std::string path, std::vector<unsigned char> data)
    {
        Image image = Image();

        image.imageID = images.size();
        image.path = path;

        image.TextureBuffer = stbi_load_from_memory(&data[0], data.size(), &image.Width, &image.Height, &image.BPP, 4);
        images.push_back(image);
    }

    void Texture::AddTextureOnLoad(std::string path)
    {
        Image image = Image();

        image.imageID = images.size();
        image.retrivRenderID = true;
        image.path = path;;

        images.push_back(image);
    }

    std::string Texture::GetPath(unsigned int imageID)
    {
        if (images.size() > imageID)
        {
            return images[imageID].path;
        }

        return std::string();
    }

    void Texture::SetImage(unsigned int imageID, Image image)
    {
        if (images.size() > imageID)
        {
            images[imageID] = image;
            images[imageID].imageID = imageID;
        }
    }

    Image Texture::GetImage(std::string path)
    {
        for (const auto& image : images)
        {
            if (image.path == path)
                return image;
        }

        //std::cout << "did not find image conteining path: " << path << std::endl;
        return Image();
    }

    bool Texture::PushGPU(unsigned char* buffer, int width, int height)
    {
        if (buffer == nullptr)
            return false;
        
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
        return true;
    }

	bool Texture::Load(unsigned int imageID)
	{
        if (images.size() > imageID)
        {
            if (images[imageID].retrivRenderID)
            {
                //std::cout << "retrivning render id:" << std::endl;
                return false;
            }

            glGenTextures(1, &images[imageID].renderID);
            glBindTexture(GL_TEXTURE_2D, images[imageID].renderID);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            std::cout << "Loading texture to GPU: " << images[imageID].path << std::endl;
            if (!PushGPU(images[imageID].TextureBuffer, images[imageID].Width, images[imageID].Height))
            {
                images[imageID].TextureBuffer = stbi_load(images[imageID].path.c_str(), &images[imageID].Width, &images[imageID].Height, &images[imageID].BPP, 4);
                if (!PushGPU(images[imageID].TextureBuffer, images[imageID].Width, images[imageID].Height))
                {
                    std::cout << "cant load (" << images[imageID].path << ") to GPU" << std::endl;
                    TerminatImage(images[imageID]);
                    /* returning true so the imege thets not loaded gets ignored after terminate image.*/
                    return true;
                }
            }
            stbi_image_free(images[imageID].TextureBuffer);

            glBindTexture(GL_TEXTURE_2D, 0);
        }
        return true;
	}

	void Texture::Bind() const
	{
        for (auto& image : images)
        {
            glActiveTexture(GL_TEXTURE0 + image.imageID);
            glBindTexture(GL_TEXTURE_2D, image.renderID);
        }
	}

	void Texture::Unbind() const
	{
        glBindTexture(GL_TEXTURE_2D, 0);
	}
}