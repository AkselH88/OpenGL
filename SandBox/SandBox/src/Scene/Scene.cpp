#include "Scene.h"

#include "Shader/ShaderSource.h"
#include "Math/SceneMath.h"

#include "Loaders/glTF/glTF.h"

namespace Scene
{
    Scene::Scene()
        : scene(SceneContentConteiner()), shader(Shader()), source(ShaderProgramSource()), scale(1.0f),
        joints(false), playable(false), type(ModelType::OBJECT)
    {

    }
    Scene::Scene(const std::string& filepath, bool playable, ModelType type)
        : scene(SceneContentConteiner()), shader(Shader()), source(ShaderProgramSource()), scale(1.0f),
        joints(false), playable(playable), type(type)
    {
        glTF_file_data(filepath, &scene);

        std::cout << "Size of joints: " << scene.animator.size_of_joints() << std::endl;
        if (scene.animator.size_of_joints())
            joints = true;

        source.VertexSource = ShaderSource::Vertex(scene.animator.size_of_joints());
        source.FragmentSource = ShaderSource::Fragment();

        //std::cout << source.VertexSource << std::endl;

        shader.LoadFromMemory(source);
    }

    Scene::~Scene()
    {

    }

    void Scene::Init()
    {
        scene.meshes.Load();
        scene.animator.GlobalTransform();
            
        shader.Bind();
        shader.SetUniform1i("material.diffuse", 0);
        shader.Unbind();
    }

    void Scene::OnUpdate(float deltaTime, Camera* camera)
    {
        shader.Bind();

        /* Vertex Shader */
        if (joints)
        {
            if (playable)
            {
                //camera->prosessAnimation(deltaTime);
                shader.SetUniformMat4f("u_jointMatrix", scene.animator.playAnimation(deltaTime, camera->getAnimation(scene.animator.getCurrentAnimation())));
            }
            else
                shader.SetUniformMat4f("u_jointMatrix", scene.animator.playAnimation(deltaTime));
        }

        if (playable)
        {
            shader.SetUniformMat4f("u_modelViewMatrix", camera->GetViewMatrix() * scale_Matrix(scale));
            shader.SetUniformMat4f("u_projectionMatrix", camera->GetProjectionMatrix());
            shader.SetUniform3f("viewPos", camera->Eye);

            shader.SetUniform3f("light.position", camera->getLightPosition());
        }
        else
        {
            shader.SetUniformMat4f("u_modelViewMatrix", camera->GetWorldViewMatrix() * scale_Matrix(scale));
            shader.SetUniformMat4f("u_projectionMatrix", camera->GetProjectionMatrix());
            shader.SetUniform3f("viewPos", camera->EyeWorld);

            shader.SetUniform3f("light.position", camera->getWorldLightPosition());
        }

        // Fragment Shader

        // light properties
        shader.SetUniform3f("light.ambient", ambient);
        shader.SetUniform3f("light.diffuse", diffuse);
        shader.SetUniform3f("light.specular", specular);

        // material properties
        //shader.SetUniform3f("material.specular", materialspecular);
        shader.SetUniform1f("material.shininess", 64.0f);

        shader.Unbind();
    }

    void Scene::OnDraw()
    {
        shader.Bind();

        scene.meshes.Draw();

        shader.Unbind();
    }

    void Scene::Unbind()
    {
        scene.meshes.Unbind();
        shader.Unbind();
    }
}