#pragma once

#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <unordered_map>

#include "Math/SceneMath.h"

namespace SceneContent
{
	class Joint
	{
	public:
		Joint()
			: name(std::string()), JointId(unsigned int()), joint_Matrix(glm::mat4()), local_TransformMatrix(glm::mat4()), global_TransformMatrix(glm::mat4()), inverseBindMatrice(glm::mat4()),
			Children(std::vector<std::string>()), translation(glm::vec3()), rotation(Quaternion()), scale(glm::vec3())
		{}
		~Joint() {}

		std::string name;
		std::string parent;
		unsigned int JointId;

		glm::vec3 translation;
		Quaternion rotation;
		glm::vec3 scale;

		glm::mat4 joint_Matrix;
		glm::mat4 local_TransformMatrix;
		glm::mat4 global_TransformMatrix;
		
		glm::mat4 inverseBindMatrice;

		std::vector<std::string> Children;
	};

	class Joints
	{
	public:
		Joints();
		~Joints();

		int const& size() { return count; }

		void AddJoint(Joint& joint);
		
		void SetAnimationMatrixMap(const std::unordered_map<std::string, glm::mat4>& keyFrame) { this->keyFrame = keyFrame; }

		std::vector<glm::mat4> const& GetJointMatrix() { return u_jointMatrix; }
		std::vector<std::string> const& GetPerents() { return parents; }
		void CalcGlobalTransform(std::string name, glm::mat4 parentGlobal = glm::mat4(1));
		void UpdateJointMatrix(std::string name, glm::mat4 trensformMatrix = glm::mat4(1));

		std::vector<glm::mat4> GetStaticPose();
		std::vector<glm::mat4> getGlobalBindJoitsMatrix();
	private:
		int count;
		std::string GetPerent(std::string name);
		glm::mat4 const& GetKeyFrame(std::string name) {return keyFrame[name];}

		std::vector<std::string> parents;
		
		std::vector<glm::mat4> u_jointMatrix;
		std::unordered_map<std::string, glm::mat4> keyFrame;
		std::unordered_map<std::string, Joint> JointMap;
	};
}
