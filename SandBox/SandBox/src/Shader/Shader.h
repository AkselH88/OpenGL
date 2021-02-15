#pragma once

#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

struct ShaderProgramSource
{
	std::string VertexSource = std::string();
	std::string FragmentSource = std::string();
};

class Shader
{
private:
	std::string m_FilePath;
	unsigned int m_RendererID;
	mutable std::unordered_map<std::string, int> m_UniformLocationCache;
public:
	Shader();
	Shader(const std::string& filepath);
	
	~Shader();

	// use this if you created the shader class using Shader();
	void LoadFromMemory(const ShaderProgramSource& Source);

	void Bind() const;
	void Unbind() const;

	void SetUniform1i(const std::string& name, int value);
	void SetUniform1f(const std::string& name, float value);
	void SetUniform2f(const std::string& name, const glm::vec2& value);
	void SetUniform3f(const std::string& name, const glm::vec3& value);
	void SetUniform4f(const std::string& name, const glm::vec4& value);

	void SetUniformMat3f(const std::string& name, const glm::mat3& matrix);
	void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);
	void SetUniformMat4f(const std::string& name, const std::vector<glm::mat4>& matrix);
private:
	template<typename T>
	ShaderProgramSource ParseShader(T& stream);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);

	int GetUniformLocation(const std::string& name) const;
};