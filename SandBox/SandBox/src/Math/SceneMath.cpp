#include "SceneMath.h"

void translation_Matrix(glm::mat4& T, const glm::vec3& t)
{
	T[0][0] = 1;
	T[1][1] = 1;
	T[2][2] = 1;
	T[3][0] = t.x;
	T[3][1] = t.y;
	T[3][2] = t.z;
	T[3][3] = 1;
}
glm::mat4 translation_Matrix(const glm::vec3& t)
{
	glm::mat4 T = glm::mat4();
	T[0][0] = 1;
	T[1][1] = 1;
	T[2][2] = 1;
	T[3][0] = t.x;
	T[3][1] = t.y;
	T[3][2] = t.z;
	T[3][3] = 1;

	return T;
}

void scale_Matrix(glm::mat4& S, const glm::vec3& s)
{
	S[0][0] = s.x;
	S[1][1] = s.y;
	S[2][2] = s.z;
	S[3][3] = 1;
}

glm::mat4 scale_Matrix(const glm::vec3& s)
{
	glm::mat4 S = glm::mat4();
	S[0][0] = s.x;
	S[1][1] = s.y;
	S[2][2] = s.z;
	S[3][3] = 1;

	return S;
}

/* translation rotation scale Matrix*/
void TRS_Matrix(glm::mat4& M, const glm::mat4& T, const glm::mat4& R, const glm::mat4& S)
{
	M = T * R * S;
}
/* translation rotation scale Matrix*/
glm::mat4 TRS_Matrix(const glm::mat4& T, const glm::mat4& R, const glm::mat4& S)
{
	return T * R * S;
}

/* inverse Matrix */
void invMat4(glm::mat4& inv, const glm::mat4& mat)
{
	inv = mat / (mat * mat);
}
/* inverse Matrix */
glm::mat4 invMat4(const glm::mat4& mat)
{
	return mat / (mat * mat);
}

void printVec3(const glm::vec3& v)
{
	std::cout << v.x << ", " << v.y << ", " << v.z << std::endl;
}

void printVec4(const glm::vec4& v)
{
	std::cout << v.x << ", " << v.y << ", " << v.z << ", " << v.w << std::endl;
}

void printMat3(const glm::mat3& m)
{
	std::cout << m[0][0] << ", " << m[1][0] << ", " << m[2][0] << ", " <<  std::endl;
	std::cout << m[0][1] << ", " << m[1][1] << ", " << m[2][1] << ", " << std::endl;
	std::cout << m[0][2] << ", " << m[1][2] << ", " << m[2][2] << std::endl;
}

void printMat4(const glm::mat4& m)
{
	std::cout << m[0][0] << ", " << m[1][0] << ", " << m[2][0] << ", " << m[3][0] << ", " << std::endl;
	std::cout << m[0][1] << ", " << m[1][1] << ", " << m[2][1] << ", " << m[3][1] << ", " << std::endl;
	std::cout << m[0][2] << ", " << m[1][2] << ", " << m[2][2] << ", " << m[3][2] << ", " << std::endl;
	std::cout << m[0][3] << ", " << m[1][3] << ", " << m[2][3] << ", " << m[3][3] << std::endl;
}

