#pragma once
#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"

class Quaternion 
{
private:
	float x, y, z, w;
	glm::vec4 Quat;

	/**
	 * Creates a quaternion and normalizes it.
	 *
	 * @param x
	 * @param y
	 * @param z
	 * @param w
	 */
public:
	Quaternion()
		: x(float()), y(float()), z(float()), w(float()), Quat(glm::vec4())
	{}
	Quaternion(float x, float y, float z, float w) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
		this->Quat = { x, y, z, w };
		normalize();
	}

	void init(float x, float y, float z, float w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
		this->Quat = { x, y, z, w };
		normalize();
	}

	void updateQuat(glm::vec4 quat)
	{
		this->x = quat.x;
		this->y = quat.y;
		this->z = quat.z;
		this->w = quat.w;
		this->Quat = quat;
		normalize();
	}

	/**
	 * Normalizes the quaternion.
	 */
	void normalize() {
		float mag = (float)sqrt(w * w + x * x + y * y + z * z);
		w /= mag;
		x /= mag;
		y /= mag;
		z /= mag;
		Quat /= mag;
	}

	void normalize(glm::vec4 quat) {
		float mag = (float)sqrt(quat.w * quat.w + quat.x * quat.x + quat.y * quat.y + quat.z * quat.z);
		w /= mag;
		x /= mag;
		y /= mag;
		z /= mag;
		Quat /= mag;
	}

	/**
	 * Converts the quaternion to a 4x4 matrix representing the exact same
	 * rotation as this quaternion. (The rotation is only contained in the
	 * top-left 3x3 part, but a 4x4 matrix is returned here for convenience
	 * seeing as it will be multiplied with other 4x4 matrices).
	 *
	 * More detailed explanation here:
	 * http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToMatrix/
	 *
	 * @return The rotation matrix which represents the exact same rotation as
	 *         this quaternion.
	 */
	glm::mat4 toRotationMatrix() {
		glm::mat4 matrix = glm::mat4();
		const float xy = x * y;
		const float xz = x * z;
		const float xw = x * w;
		const float yz = y * z;
		const float yw = y * w;
		const float zw = z * w;
		const float xSquared = x * x;
		const float ySquared = y * y;
		const float zSquared = z * z;
		
		matrix = {	1 - 2 * (ySquared + zSquared),					2 * (xy + zw),					2 * (xz - yw), 0,
									2 * (xy - zw),	1 - 2 * (xSquared + zSquared),					2 * (yz + xw), 0,
									2 * (xz + yw),					2 * (yz - xw),	1 - 2 * (xSquared + ySquared), 0,
												0,								0,								0, 1 };

		return matrix;
	}

	/**
	 * Extracts the rotation part of a transformation matrix and converts it to
	 * a quaternion using the magic of maths.
	 *
	 * More detailed explanation here:
	 * http://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/index.htm
	 *
	 * @param matrix
	 *            - the transformation matrix containing the rotation which this
	 *            quaternion shall represent.
	 */
	static Quaternion fromMatrix(glm::mat4 matrix) {
		float w, x, y, z;
		float diagonal = matrix[0][0] + matrix[1][1] + matrix[2][2];
		if (diagonal > 0) {
			float w4 = (float)(sqrt(diagonal + 1.f) * 2.f);
			w = w4 / 4.f;
			x = (matrix[2][1] - matrix[1][2]) / w4;
			y = (matrix[0][2] - matrix[2][0]) / w4;
			z = (matrix[1][0] - matrix[0][1]) / w4;
		}
		else if ((matrix[0][0] > matrix[1][1]) && (matrix[0][0] > matrix[2][2])) {
			float x4 = (float)(sqrt(1.f + matrix[0][0] - matrix[1][1] - matrix[2][2]) * 2.f);
			w = (matrix[2][1] - matrix[1][2]) / x4;
			x = x4 / 4.f;
			y = (matrix[0][1] + matrix[1][0]) / x4;
			z = (matrix[0][2] + matrix[2][0]) / x4;
		}
		else if (matrix[1][1] > matrix[2][2]) {
			float y4 = (float)(sqrt(1.f + matrix[1][1] - matrix[0][0] - matrix[2][2]) * 2.f);
			w = (matrix[0][2] - matrix[2][0]) / y4;
			x = (matrix[0][1] + matrix[1][0]) / y4;
			y = y4 / 4.f;
			z = (matrix[1][2] + matrix[2][1]) / y4;
		}
		else {
			float z4 = (float)(sqrt(1.f + matrix[2][2] - matrix[0][0] - matrix[1][1]) * 2.f);
			w = (matrix[1][0] - matrix[0][1]) / z4;
			x = (matrix[0][2] + matrix[2][0]) / z4;
			y = (matrix[1][2] + matrix[2][1]) / z4;
			z = z4 / 4.f;
		}
		return Quaternion(x, y, z, w);
	}

	/**
	 * Interpolates between two quaternion rotations and returns the resulting
	 * quaternion rotation. The interpolation method here is "nlerp", or
	 * "normalized-lerp". Another mnethod that could be used is "slerp", and you
	 * can see a comparison of the methods here:
	 * https://keithmaggio.wordpress.com/2011/02/15/math-magician-lerp-slerp-and-nlerp/
	 *
	 * and here:
	 * http://number-none.com/product/Understanding%20Slerp,%20Then%20Not%20Using%20It/
	 *
	 * @param a
	 * @param b
	 * @param blend
	 *            - a value between 0 and 1 indicating how far to interpolate
	 *            between the two quaternions.
	 * @return The resulting interpolated rotation in quaternion format.
	 */
	static Quaternion interpolate(Quaternion a, Quaternion b, float blend) {
		Quaternion result = Quaternion(0, 0, 0, 1);
		float dot = a.w * b.w + a.x * b.x + a.y * b.y + a.z * b.z;
		float blendI = 1.f - blend;
		if (dot < 0) {
			result.w = blendI * a.w + blend * -b.w;
			result.x = blendI * a.x + blend * -b.x;
			result.y = blendI * a.y + blend * -b.y;
			result.z = blendI * a.z + blend * -b.z;
		}
		else {
			result.w = blendI * a.w + blend * b.w;
			result.x = blendI * a.x + blend * b.x;
			result.y = blendI * a.y + blend * b.y;
			result.z = blendI * a.z + blend * b.z;
		}
		result.normalize();
		return result;
	}

	float dot(Quaternion a, Quaternion b)
	{
		return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
	}

	Quaternion slerp(Quaternion previousQuat, Quaternion nextQuat, float interpolationValue)
	{
		float dotProduct = dot(previousQuat, nextQuat);

			//make sure we take the shortest path in case dot Product is negative
		if (dotProduct < 0.0)
		{
			nextQuat.Quat = -nextQuat.Quat;
			dotProduct = -dotProduct;
		}
			
		//if the two quaternions are too close to each other, just linear interpolate between the 4D vector
		if (dotProduct > 0.9995)
		{
			Quat = previousQuat.Quat + interpolationValue * (nextQuat.Quat - previousQuat.Quat);
			return Quaternion(Quat.x, Quat.y, Quat.z, Quat.w);
		}
					
		//perform the spherical linear interpolation
		float theta_0 = acos(dotProduct);
		float theta = interpolationValue * theta_0;
		float sin_theta = sin(theta);
		float sin_theta_0 = sin(theta_0);

		float scalePreviousQuat = cos(theta) - dotProduct * sin_theta / sin_theta_0;
		float scaleNextQuat = sin_theta / sin_theta_0;
		Quat = scalePreviousQuat * previousQuat.Quat + scaleNextQuat * nextQuat.Quat;
		return Quaternion(Quat.x, Quat.y, Quat.z, Quat.w);
	}

};