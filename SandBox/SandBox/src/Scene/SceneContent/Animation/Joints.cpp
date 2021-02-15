#include "Joints.h"

constexpr auto FirstParent = "God";

namespace SceneContent
{
	Joints::Joints()
		: count(int()), parents(std::vector<std::string>()), JointMap(std::unordered_map<std::string, Joint>()),
		u_jointMatrix(std::vector<glm::mat4>()), keyFrame(std::unordered_map<std::string, glm::mat4>())
	{}
	Joints::~Joints()
	{}

	std::string Joints::GetPerent(std::string name)
	{
		for (const auto& perent : JointMap)
		{
			for (const auto& child : perent.second.Children)
			{
				if (child == name)
					return perent.first;
			}
		}
		return FirstParent;
	}

	void Joints::AddJoint(Joint& joint)
	{
		if (JointMap.find(joint.name) != JointMap.end())
		{
			return;
		}

		joint.JointId = count;
		joint.local_TransformMatrix = TRS_Matrix(	translation_Matrix(joint.translation),
													joint.rotation.toRotationMatrix(),
													scale_Matrix(joint.scale));

		u_jointMatrix.emplace_back(joint.local_TransformMatrix);
		JointMap[joint.name] = joint;

		JointMap[joint.name].parent = GetPerent(joint.name);
		if (JointMap[joint.name].parent == FirstParent)
			parents.emplace_back(joint.name);
			
		count++;
	}

	void Joints::CalcGlobalTransform(std::string name, glm::mat4 parentGlobal)
	{
		JointMap[name].global_TransformMatrix = parentGlobal * JointMap[name].local_TransformMatrix;

		for (const auto& child : JointMap[name].Children)
			CalcGlobalTransform(child, JointMap[name].global_TransformMatrix);
		/*testing*/
		u_jointMatrix[JointMap[name].JointId] = JointMap[name].global_TransformMatrix * JointMap[name].inverseBindMatrice;
	}

	std::vector<glm::mat4> Joints::GetStaticPose()
	{
		return u_jointMatrix;
	}

	std::vector<glm::mat4> Joints::getGlobalBindJoitsMatrix()
	{
		std::vector<glm::mat4> p = std::vector<glm::mat4>();
		p.reserve(JointMap.size());
		for (const auto& pose : JointMap)
			p.emplace_back(pose.second.global_TransformMatrix);
		return p;
	}

	void Joints::UpdateJointMatrix(std::string name, glm::mat4 trensformMatrix/* = glm::mat4(1)*/)
	{
		glm::mat4 pose = trensformMatrix * GetKeyFrame(name);
		for (const auto& child : JointMap[name].Children)
			UpdateJointMatrix(child, pose);

		u_jointMatrix[JointMap[name].JointId] =
			pose * JointMap[name].inverseBindMatrice;
	}
}