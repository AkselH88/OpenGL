#pragma once

#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Quaternion.h"

void translation_Matrix(glm::mat4& T, const glm::vec3& t);
glm::mat4 translation_Matrix(const glm::vec3& t);

void scale_Matrix(glm::mat4& S, const glm::vec3& s);
glm::mat4 scale_Matrix(const glm::vec3& s);

void TRS_Matrix(glm::mat4& M, const glm::mat4& T, const glm::mat4& R, const glm::mat4& S);
glm::mat4 TRS_Matrix(const glm::mat4& T, const glm::mat4& R, const glm::mat4& S);

void invMat4(glm::mat4& inv, const glm::mat4& mat);
glm::mat4 invMat4(const glm::mat4& mat);

void printVec3(const glm::vec3& v);
void printVec4(const glm::vec4& v);
void printMat3(const glm::mat3& m);
void printMat4(const glm::mat4& m);
