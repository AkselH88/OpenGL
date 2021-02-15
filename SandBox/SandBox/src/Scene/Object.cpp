#include "Object.h"

#include "Shader/ShaderSource.h"
#include "Math/SceneMath.h"

#include "Loaders/glTF/glTF.h"

namespace Scene
{
    Object::Object(const std::string& filepath)
        : scene(SceneContentConteiner()), shader(Shader()), source(ShaderProgramSource()), scale(0.01f)
    {
        glTF_file_data(filepath, &scene);

        source.VertexSource = ShaderSource::Vertex();
        source.FragmentSource = ShaderSource::Fragment();

        shader.LoadFromMemory(source);
    }

    Object::~Object()
    {

    }

    void Object::Init()
    {
        scene.meshes.Load();

        shader.Bind();
        shader.SetUniform1i("material.diffuse", 0);
        shader.SetUniform1i("material.specular", 1);
        shader.Unbind();
    }

    void Object::OnUpdate(float deltaTime, Camera* camera)
    {
        shader.Bind();

        /* Vertex Shader */
        shader.SetUniformMat4f("u_modelViewMatrix", camera->GetWorldViewMatrix() * scale_Matrix(scale));
        shader.SetUniformMat4f("u_projectionMatrix", camera->GetProjectionMatrix());
        shader.SetUniform3f("viewPos", camera->EyeWorld);

        shader.SetUniform3f("light.position", camera->getWorldLightPosition());


        // Fragment Shader
        shader.SetUniform3f("light.ambient", ambient);
        shader.SetUniform3f("light.diffuse", diffuse);
        shader.SetUniform3f("light.specular", specular);

        // material properties
        //shader.SetUniform3f("material.specular", materialspecular);
        shader.SetUniform1f("material.shininess", 64.0f);

        shader.Unbind();
    }

    void Object::OnDraw()
    {
        shader.Bind();

        scene.meshes.Draw();

        shader.Unbind();
    }

    void Object::Unbind()
    {
        scene.meshes.Unbind();
        shader.Unbind();
    }
}