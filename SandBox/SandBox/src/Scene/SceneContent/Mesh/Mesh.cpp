#include "Mesh.h"
#include <iostream>

#include <GL/glew.h>
#include "Shader/Shader.h"

namespace SceneContent
{
	//////////////////	Mesh	///////////////////////
	Mesh::Mesh()
		: meshID(unsigned int()), renderID(unsigned int()), primetives(std::unordered_map<std::string, Primetive>()),
		indices(Primetive()), layout(unsigned int()), count(unsigned int()), m_VertexArrayBuffer{ unsigned int() }
	{
		layoutLocations["POSITION"]		= 0;
		layoutLocations["NORMAL"]		= 1;
		
		layoutLocations["TEXCOORD_0"]	= 2;
		layoutLocations["JOINTS_0"]		= 3;
		layoutLocations["WEIGHTS_0"]	= 4;
		layoutLocations["TANGENT"]		= 5;

		layoutLocations["TEXCOORD_1"]	= 6;
		layoutLocations["JOINTS_1"]		= 7;
		layoutLocations["WEIGHTS_1"]	= 8;
		layoutLocations["COLOR_0"]		= 9;
		layoutLocations["COLOR_1"]		= 10;
	}

	Mesh::~Mesh()
	{
		glDeleteVertexArrays(1, &renderID);
		glDeleteBuffers(count, m_VertexArrayBuffer);
		//delete m_VertexArrayBuffer;
		//m_VertexArrayBuffer = nullptr;
	}

	void Mesh::AddPrimetive(std::string name, unsigned int primetiveType, unsigned int vector, std::vector<unsigned char>& primetive)
	{

		if (primetives.find(name) != primetives.end())
		{
			return;
		}
		if(layoutLocations.find(name) != layoutLocations.end())
			primetives[name].SetLayoutLocation(layoutLocations[name]);

		primetives[name].AddPrimetiveType(primetiveType);
		primetives[name].AddPrimetive(primetive);
		primetives[name].SetVector(vector);

		layout++;
		count++;
	}

	void Mesh::AddPrimetive(std::string name, unsigned int primetiveType, unsigned int vector, std::vector<unsigned short>& primetive)
	{

		if (primetives.find(name) != primetives.end())
		{
			return;
		}
		if (layoutLocations.find(name) != layoutLocations.end())
			primetives[name].SetLayoutLocation(layoutLocations[name]);

		primetives[name].AddPrimetiveType(primetiveType);
		primetives[name].AddPrimetive(primetive);
		primetives[name].SetVector(vector);

		layout++;
		count++;
	}

	void Mesh::AddPrimetive(std::string name, unsigned int primetiveType, unsigned int vector, std::vector<int>& primetive)
	{

		if (primetives.find(name) != primetives.end())
		{
			return;
		}
		if (layoutLocations.find(name) != layoutLocations.end())
			primetives[name].SetLayoutLocation(layoutLocations[name]);

		primetives[name].AddPrimetiveType(primetiveType);
		primetives[name].AddPrimetive(primetive);
		primetives[name].SetVector(vector);

		layout++;
		count++;
	}

	void Mesh::AddPrimetive(std::string name, unsigned int primetiveType, unsigned int vector, std::vector<unsigned int>& primetive)
	{

		if (primetives.find(name) != primetives.end())
		{
			return;
		}
		if (layoutLocations.find(name) != layoutLocations.end())
			primetives[name].SetLayoutLocation(layoutLocations[name]);

		primetives[name].AddPrimetiveType(primetiveType);
		primetives[name].AddPrimetive(primetive);
		primetives[name].SetVector(vector);

		layout++;
		count++;
	}

	void Mesh::AddPrimetive(std::string name, unsigned int primetiveType, unsigned int vector, std::vector<float>& primetive)
	{

		if (primetives.find(name) != primetives.end())
		{
			return;
		}
		if (layoutLocations.find(name) != layoutLocations.end())
			primetives[name].SetLayoutLocation(layoutLocations[name]);

		primetives[name].AddPrimetiveType(primetiveType);
		primetives[name].AddPrimetive(primetive);
		primetives[name].SetVector(vector);

		layout++;
		count++;
	}

	const std::vector<float> Mesh::GetPrimetive(std::string name)
	{
		if (primetives.find(name) != primetives.end())
		{
			return primetives[name].GetPrimetive_float();
		}
		return std::vector<float>();
	}

	void Mesh::Load()
	{
		for (size_t i = 0; i < count; i++) { m_VertexArrayBuffer[i] = unsigned int(); }
		//m_VertexArrayBuffer = new unsigned int[count];
		glGenVertexArrays(1, &renderID);
		glBindVertexArray(renderID);
		glGenBuffers(count, m_VertexArrayBuffer);

		for (auto& primetive : primetives)
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_VertexArrayBuffer[primetive.second.GetLayoutLocation()]);
			glBufferData(GL_ARRAY_BUFFER, primetive.second.size_in_bytes(), primetive.second.GetPrimetiveAdress(), GL_STATIC_DRAW);
			glEnableVertexAttribArray(primetive.second.GetLayoutLocation());
			glVertexAttribPointer(primetive.second.GetLayoutLocation(), primetive.second.GetVector(), primetive.second.GetPrimetiveType(), GL_FALSE, 0, 0);
		}

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VertexArrayBuffer[layout]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size_in_bytes(), indices.GetPrimetiveAdress(), GL_STATIC_DRAW);

		glBindVertexArray(0);
	}

	void Mesh::Bind() const
	{
		glBindVertexArray(renderID);
	}

	void Mesh::Unbind() const
	{
		glBindVertexArray(0);
	}

	void Mesh::Draw() const
	{
		glBindVertexArray(renderID);
		glDrawElementsBaseVertex(GL_TRIANGLES, indices.size(), indices.GetPrimetiveType(), 0, 0);

		glBindVertexArray(0);
	}
}