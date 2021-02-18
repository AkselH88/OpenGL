#pragma once

#include <vector>
#include <unordered_map>

/*
#define GL_BYTE 0x1400				// 5120
#define GL_UNSIGNED_BYTE 0x1401		// 5121
#define GL_SHORT 0x1402				// 5122
#define GL_UNSIGNED_SHORT 0x1403	// 5123
#define GL_INT 0x1404				// 5124
#define GL_UNSIGNED_INT 0x1405		// 5125
#define GL_FLOAT 0x1406				// 5126
*/

namespace SceneContent
{
	struct Primetive
	{
	public:
		/* vec2 = 2, vec3 = 3 .....*/
		void SetVector(unsigned int vector) { this->vector = vector; }
		/* 2 = vec2, 3 = vec3 .....*/
		unsigned int GetVector() { return vector; }

		void SetLayoutLocation(unsigned int location) { this->layoutLocation = location; }
		unsigned int GetLayoutLocation() { return layoutLocation; }

		void AddPrimetiveType(unsigned int primetiveType) { this->primetiveType = primetiveType; }

		void AddPrimetive(std::vector<unsigned char>& primetive)	{ primetive_u_char	= primetive;	}
		void AddPrimetive(std::vector<unsigned short>& primetive)	{ primetive_u_short = primetive;	}
		void AddPrimetive(std::vector<int>& primetive)				{ primetive_int		= primetive;	}
		void AddPrimetive(std::vector<unsigned int>& primetive)		{ primetive_u_int	= primetive;	}
		void AddPrimetive(std::vector<float>& primetive)			{ primetive_float	= primetive;	}

		unsigned int size_in_bytes()
		{
			switch (primetiveType)
			{
			case 0x1401: // GL_UNSIGNED_BYTE
				return primetive_u_char.size();
			case 0x1403: // GL_UNSIGNED_SHORT
				return 2 * primetive_u_short.size();
			case 0x1404: // GL_INT
				return 4 * primetive_int.size();
			case 0x1405: // GL_UNSIGNED_INT
				return 4 * primetive_u_int.size();
			case 0x1406: // GL_FLOAT
				return 4 * primetive_float.size();
			default:
				break;
			}
		}
		const void* GetPrimetiveAdress()
		{
			switch (primetiveType)
			{
			case 5121: // GL_UNSIGNED_BYTE
				return &primetive_u_char[0];
			case 5123: // GL_UNSIGNED_SHORT
				return &primetive_u_short[0];
			case 5124: // GL_INT
				return &primetive_int[0];
			case 5125: // GL_UNSIGNED_INT
				return &primetive_u_int[0];
			case 5126: // GL_FLOAT
				return &primetive_float[0];
			default:
				break;
			}
		}
		unsigned int size() const
		{
			switch (primetiveType)
			{
			case 0x1401: // GL_UNSIGNED_BYTE
				return primetive_u_char.size();
			case 0x1403: // GL_UNSIGNED_SHORT
				return primetive_u_short.size();
			case 0x1404: // GL_INT
				return primetive_int.size();
			case 0x1405: // GL_UNSIGNED_INT
				return primetive_u_int.size();
			case 0x1406: // GL_FLOAT
				return primetive_float.size();
			}
		}
		unsigned int GetPrimetiveType() const { return primetiveType; }
		unsigned int GetTypeSize() { return typeSize; }

		/* "POSITION" can only be float */
		const std::vector<float> GetPrimetive_float() { return primetive_float; }
		/* Indices got unsigned int and short */
		const std::vector<unsigned int>* GetPrimetive_unsigned_int() { return &primetive_u_int; }
		const std::vector<unsigned short>* GetPrimetive_unsigned_short() { return &primetive_u_short; }

	private:
		unsigned int typeSize = NULL;
		unsigned int primetiveType = NULL;
		std::vector<int> primetive_int = std::vector<int>();
		std::vector<float> primetive_float = std::vector<float>();
		std::vector<unsigned int> primetive_u_int = std::vector<unsigned int>();
		std::vector<unsigned char> primetive_u_char = std::vector<unsigned char>();
		std::vector<unsigned short> primetive_u_short = std::vector<unsigned short>();

		unsigned int vector = NULL;
		unsigned int layoutLocation = NULL;
	};

	class Mesh
	{
	public:
		Mesh();
		~Mesh();
		void SetMeshId(unsigned int meshId) { this->meshID = meshId; }
		unsigned int GetMeshId() { return meshID; }

		void AddPrimetive(std::string name, unsigned int primetiveType, unsigned int vector, std::vector<unsigned char> primetive);
		void AddPrimetive(std::string name, unsigned int primetiveType, unsigned int vector, std::vector<unsigned short> primetive);
		void AddPrimetive(std::string name, unsigned int primetiveType, unsigned int vector, std::vector<int> primetive);
		void AddPrimetive(std::string name, unsigned int primetiveType, unsigned int vector, std::vector<unsigned int> primetive);
		void AddPrimetive(std::string name, unsigned int primetiveType, unsigned int vector, std::vector<float> primetive);

		void AddIndices(unsigned int primetiveType, std::vector<unsigned short> primetive)	{ indices.AddPrimetiveType(primetiveType); indices.AddPrimetive(primetive); count++; }
		void AddIndices(unsigned int primetiveType, std::vector<unsigned int> primetive)	{ indices.AddPrimetiveType(primetiveType); indices.AddPrimetive(primetive); count++; }

		const std::vector<float> GetPrimetive(std::string name);

		unsigned int GetIndicesType() { return indices.GetPrimetiveType(); }
		const std::vector<unsigned int>* GetIndices_unsigned_int() { return indices.GetPrimetive_unsigned_int(); }
		const std::vector<unsigned short>* GetIndices_unsigned_short() { return indices.GetPrimetive_unsigned_short(); }

		void Load();
		void Bind() const;
		void Unbind() const;
		void Draw() const;

		unsigned int size() const { return count; }
	private:
		unsigned int meshID;
		unsigned int renderID;
		unsigned int m_VertexArrayBuffer[11];

		unsigned int layout;
		unsigned int count;

		std::unordered_map<std::string, Primetive> primetives;
		std::unordered_map<std::string, unsigned int> layoutLocations;
		Primetive indices;
	};
}