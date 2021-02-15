#include "Character.h"

#include "Shader/ShaderSource.h"
#include "Math/SceneMath.h"

#include "Loaders/glTF/glTF.h"

namespace Scene
{
    Character::Character(const std::string& filepath)
        : scene(SceneContentConteiner()), shader(Shader()), source(ShaderProgramSource()), scale(1.0f)
    {
        glTF_file_data(filepath, &scene);

        std::cout << "Size of joints: " << scene.animator.size_of_joints() << std::endl;
        if (!scene.animator.size_of_joints())
        {
            std::cout << filepath << ": dont got any joints, and cant be animated." << std::endl;
            throw(filepath, ": dont got any joints, and cant be animated.");
        }

        source.VertexSource = ShaderSource::Vertex(scene.animator.size_of_joints());
        source.FragmentSource = ShaderSource::Fragment();

        //std::cout << source.VertexSource << std::endl;

        shader.LoadFromMemory(source);
    }

    Character::~Character()
    {
    }

    void Character::Init()
    {
        scene.meshes.Load();
        scene.animator.GlobalTransform();

        shader.Bind();
        shader.SetUniform1i("material.diffuse", 0);
        shader.Unbind();
    }

    void Character::OnUpdate(float deltaTime, Camera* camera)
    {
        shader.Bind();

        /* Vertex Shader */
        shader.SetUniformMat4f("u_jointMatrix", scene.animator.playAnimation(deltaTime, camera->getAnimation(scene.animator.getCurrentAnimation())));

        shader.SetUniformMat4f("u_modelViewMatrix", camera->GetViewMatrix() * scale_Matrix(scale));
        shader.SetUniformMat4f("u_projectionMatrix", camera->GetProjectionMatrix());
        shader.SetUniform3f("viewPos", camera->Eye);

        shader.SetUniform3f("light.position", camera->getLightPosition());

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

    void Character::OnDraw()
    {
        shader.Bind();

        scene.meshes.Draw();

        shader.Unbind();
    }

    void Character::Unbind()
    {
        scene.meshes.Unbind();
        shader.Unbind();
    }
}