#include "ShaderSource.h"

namespace ShaderSource
{
    std::string Vertex(int joints/* = 0*/)
    {
        std::string source = std::string();

        source = "#version 430 core\n";

        source +=
            "\n"
            "layout(location = 0) in vec3 a_Position;\n"
            "layout(location = 1) in vec3 a_Normal;\n"
            "layout(location = 2) in vec2 a_TexCoords;\n"
            "\n"
            "layout(location = 3) in vec4 a_Joint;\n"
            "layout(location = 4) in vec4 a_Weight;\n"
            "\n"
            "out vec4 Position;\n"
            "out vec3 Normal;\n"
            "out vec2 TexCoords;\n";

        if (joints)
        {
            source +=
                "uniform mat4 u_jointMatrix[";
            source += std::to_string(joints).c_str();
            source += "];\n";

            source +=
                "uniform mat4 u_modelViewMatrix;\n"
                "uniform mat4 u_projectionMatrix;\n"
                "\n"
                "void main(void)\n"
                "{\n"
                "   mat4 skinMat =\n"
                "       a_Weight.x * u_jointMatrix[int(a_Joint.x)] +\n"
                "       a_Weight.y * u_jointMatrix[int(a_Joint.y)] +\n"
                "       a_Weight.z * u_jointMatrix[int(a_Joint.z)] +\n"
                "       a_Weight.w * u_jointMatrix[int(a_Joint.w)];\n"
                "\n"
                "   Position = u_modelViewMatrix * skinMat * vec4(a_Position, 1.0);\n"
                "   Normal = (skinMat * vec4(a_Normal, 0)).xyz;\n"
                "   TexCoords = a_TexCoords;\n"
                "\n"
                "   gl_Position = u_projectionMatrix * Position;\n"
                "}\n";
        }
        else
        {
            source +=
                "uniform mat4 u_modelViewMatrix;\n"
                "uniform mat4 u_projectionMatrix;\n"
                "\n"
                "void main(void)\n"
                "{\n"
                "   Position = u_modelViewMatrix * vec4(a_Position, 1.0);\n"
                "   Normal = a_Normal;\n"
                "   TexCoords = a_TexCoords;\n"
                "\n"
                "   gl_Position = u_projectionMatrix * Position;\n"
                "}\n";
        }

        return source;
    }


    std::string Fragment()
    {
        std::string source = std::string();

        source = "#version 430 core\n";

        source +=
            "\n"
            "out vec4 FragColor;\n"
            "\n"
            "struct Material {\n"
            "    sampler2D diffuse;\n"
            "    sampler2D specular;\n"
            "    float shininess;\n"
            "};\n"
            "\n"
            "struct Light {\n"
            "    vec3 position;\n"
            "    vec3 ambient;\n"
            "    vec3 diffuse;\n"
            "    vec3 specular;\n"
            "};\n"
            "\n"
            "in vec4 Position;\n"
            "in vec3 Normal;\n"
            "in vec2 TexCoords;\n"
            "\n"
            "uniform vec3 viewPos;\n"
            "uniform Material material;\n"
            "uniform Light light;\n"
            "\n"
            "void main()\n"
            "{\n"
            "    // ambient\n"
            "    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;\n"
            "\n"
            "    // diffuse\n"
            "    vec3 norm = normalize(Normal);\n"
            "    vec3 lightDir = normalize(light.position - vec3(Position));\n"
            "    float diff = max(dot(norm, lightDir), 0.0);\n"
            "    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;\n"
            "\n"
            "    // specular\n"
            "    vec3 viewDir = normalize(viewPos - vec3(Position));\n"
            "    vec3 reflectDir = reflect(-lightDir, norm);\n"
            "    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);\n"
            "    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;\n"
            "\n"
            "    vec3 result = ambient + diffuse + specular;\n"
            "    FragColor = vec4(result, 1.0);\n"
            "}\n";

        return source;
    }

    std::string SkeletenVs()
    {
        std::string vs =
            "#version 430 core\n"
            "\n"
            "layout(location = 0) in vec4 a_position;\n"
            "\n"
            "out vec4 fragColor;\n"
            "\n"
            "uniform vec4 u_color; \n"
            "\n"
            "uniform mat4 u_projection;\n"
            "uniform mat4 u_view;\n"
            "uniform mat4 u_model;\n"
            "\n"
            "void main() {\n"
            "\n"
            "   fragColor = u_color;\n"
            "   gl_Position =  u_view * u_projection * u_model * a_position;\n"
            "}\n";

        return vs;
    }


    std::string SkeletenFs()
    {
        std::string fs =
            "#version 430 core\n"
            "\n"
            "in vec4 fragColor;\n"
            "\n"
            "void main() {\n"
            "\n"
            "gl_FragColor = fragColor; \n"
            "}\n";

        return fs;
    }

    std::string BasicVs()
    {
        std::string vs =
            "#version 330 core\n"
            "\n"
            "layout(location = 0) in vec4 a_Position;\n"
            "\n"
            "out vec4 Position;\n"
            "out vec4 fcolor;\n"
            "\n"
            "uniform mat4 u_model;\n"
            "uniform mat4 u_view;\n"
            "uniform mat4 u_projection;\n"
            "\n"
            "uniform vec4 u_color;"
            "\n"
            "void main(void)\n"
            "{\n"
            "   Position = u_view * u_model * a_Position;\n"
            "   fcolor = u_color;"
            "\n"
            "   gl_Position = u_projection * Position;\n"
            "}\n";
        return vs;
    }

    std::string BasicFs()
    {
        std::string fs =
            "#version 330 core\n"
            "\n"
            "\n"
            "in vec4 fcolor;"
            "in vec4 Position;\n"
            "\n"
            "out vec4 FragColor;\n"
            "\n"
            "void main()\n"
            "{\n"
            "    FragColor = fcolor;\n"
            "}\n";
        return fs;
    }
}