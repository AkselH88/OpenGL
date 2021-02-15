#include "Level.h"

#include "stb_image/stb_image.h"

#include "Renderer.h"
#include "Shader/ShaderSource.h"
#include "Math/SceneMath.h"

#include "Loaders/glTF/glTF.h"

namespace Scene
{
    Level::Level(const std::string& filepath)
        : scene(SceneContentConteiner()), shader(Shader()), source(ShaderProgramSource()), scale(1.0f)
    {
        glTF_file_data(filepath, &scene);

        source.VertexSource = ShaderSource::Vertex();
        source.FragmentSource = ShaderSource::Fragment();

        shader.LoadFromMemory(source);
    }

    Level::~Level()
    {

    }

    void Level::Init()
    {
        scene.meshes.Load();

        terrain.Init(scene.meshes.GetPrimetivePosition("Plane"));
        terrain.FindStartPosition(glm::vec3(0));

        shader.Bind();
        shader.SetUniform1i("material.diffuse", 0);
        shader.Unbind();
    }

    void Level::OnUpdate(float deltaTime, Camera* camera)
    {
        camera->terrain_hight = terrain.OnUpdate(camera->WorldPosition);

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

    void Level::OnDraw()
    {
        shader.Bind();

        scene.meshes.Draw();

        shader.Unbind();
    }

    void Level::Unbind()
    {
        scene.meshes.Unbind();
        shader.Unbind();
    }
}