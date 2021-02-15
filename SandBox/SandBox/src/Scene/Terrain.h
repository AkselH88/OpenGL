#pragma once
#include <unordered_map>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class Terrain
{
public:
	Terrain();
	~Terrain();
	void Init(std::vector<float> pos);

	bool FindStartPosition(glm::vec3 pos);

	float OnUpdate(glm::vec3& pos);
private:
	float getCurrentPosition(const glm::vec3* pos);

	void PrintPlane();

	const glm::vec3& getTopLeft() { return terrain[current_row][current_colum]; };
	const glm::vec3& getBottomLeft() { return terrain[current_row + 1][current_colum]; };
	const glm::vec3& getTopRight() { return terrain[current_row][current_colum + 1]; };
	const glm::vec3& getBottomRight() { return terrain[current_row + 1][current_colum + 1]; };

	int current_row, current_colum;
	int rows, columns;

	std::vector<std::vector<glm::vec3>> terrain;
};