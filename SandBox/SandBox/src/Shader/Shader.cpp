#include "Shader.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"


Shader::Shader()
    : m_FilePath(std::string()), m_RendererID(0)
{
}

Shader::Shader(const std::string& filepath)
	: m_FilePath(filepath), m_RendererID(0)
{
    std::ifstream stream(m_FilePath);
    ShaderProgramSource source = ParseShader(stream);
    m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_RendererID));
}

void Shader::LoadFromMemory(const ShaderProgramSource& source)
{
    m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

template<typename T>
ShaderProgramSource Shader::ParseShader(T& stream)
{
    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1, COUNT
    };

    std::string line;
    std::stringstream ss[(int)ShaderType::COUNT];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }
        else if (type != ShaderType::NONE && type != ShaderType::COUNT)
        {
            ss[(int)type] << line << '\n';
        }
    }

    return { ss[(int)ShaderType::VERTEX].str(), ss[(int)ShaderType::FRAGMENT].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    GLCall(glShaderSource(id, 1, &src, nullptr));
    GLCall(glCompileShader(id));

    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE)
    {
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));

        // TODO: finne ut hvorfor alloca ikke er likt.
        char* message = (char*)malloc(length * sizeof(char));
        GLCall(glGetShaderInfoLog(id, length, &length, message));
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " Shader!" << std::endl;
        std::cout << message << std::endl;
        GLCall(glDeleteShader(id));

        std::cout << '\n' << source << '\n' << std::endl;

        return 0;
    }

    return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

    return program;
}

void Shader::Bind() const
{
    GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
    GLCall(glUseProgram(0));
}

// this can geturn GLint insted of int, bot its the same.
int Shader::GetUniformLocation(const std::string& name) const
{
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache[name];

    GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1)
        std::cout << "Warning: '" << name << "' doesn't exist!" << std::endl;
    
    m_UniformLocationCache[name] = location;

    return location;
}

/*
    All the SetUniform are the same in prinsipal.
    1.  will bother the gpu more then nesesery.
    void Shader::SetUniform1f(const std::string& name, float value)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        GLCall(glUniform1f(location, value));
    }
    2.
    void Shader::SetUniform1i(const std::string& name, int value)
    {
        GLint location = GetUniformLocation(name);
        GLCall(glUniform1i(location, value));
    }
    3. in use.  les code the nr 2.
    void Shader::SetUniform2f(const std::string& name, const glm::vec2& value)
    {
        GLCall(glUniform2f(GetUniformLocation(name), value.x, value.y));
    }
*/

void Shader::SetUniform1i(const std::string& name, int value)
{
    GLCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniform1f(const std::string& name, float value)
{
    GLCall(glUniform1f(GetUniformLocation(name), value));
}

void Shader::SetUniform2f(const std::string& name, const glm::vec2& value)
{
    GLCall(glUniform2f(GetUniformLocation(name), value.x, value.y));
}

void Shader::SetUniform3f(const std::string& name, const glm::vec3& value)
{
    GLCall(glUniform3f(GetUniformLocation(name), value.x, value.y, value.z));
}
void Shader::SetUniform4f(const std::string& name, const glm::vec4& value)
{
    GLCall(glUniform4f(GetUniformLocation(name), value.x, value.y, value.z, value.w));
}

void Shader::SetUniformMat3f(const std::string& name, const glm::mat3& matrix)
{
    GLCall(glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}
void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
    GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

void Shader::SetUniformMat4f(const std::string& name, const std::vector<glm::mat4>& matrix)
{
    GLCall(glUniformMatrix4fv(GetUniformLocation(name), matrix.size(), GL_FALSE, &matrix[0][0][0]));
}
