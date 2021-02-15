#include "glTF.h"

#include <GLTFSDK/GLTF.h>
#include <GLTFSDK/GLTFResourceReader.h>
#include <GLTFSDK/GLBResourceReader.h>
#include <GLTFSDK/Deserialize.h>
#include <GLTFSDK/MeshPrimitiveUtils.h>
#include <GLTFSDK/IndexedContainer.h>


// <filesystem> needs C++17
#include <filesystem>

#include <fstream>
#include <sstream>

#include <cassert>
#include <cstdlib>

    // The glTF SDK is decoupled from all file I/O by the IStreamReader (and IStreamWriter)
    // interface(s) and the C++ stream-based I/O library. This allows the glTF SDK to be used in
    // sandboxed environments, such as WebAssembly modules and UWP apps, where any file I/O code
    // must be platform or use-case specific.
class StreamReader : public Microsoft::glTF::IStreamReader
{
public:
    StreamReader(std::filesystem::path pathBase) : m_pathBase(std::move(pathBase))
    {
        assert(m_pathBase.has_root_path());
    }

    // Resolves the relative URIs of any external resources declared in the glTF manifest
    std::shared_ptr<std::istream> GetInputStream(const std::string& filename) const override
    {
        // In order to construct a valid stream:
        // 1. The filename argument will be encoded as UTF-8 so use filesystem::u8path to
        //    correctly construct a path instance.
        // 2. Generate an absolute path by concatenating m_pathBase with the specified filename
        //    path. The filesystem::operator/ uses the platform's preferred directory separator
        //    if appropriate.
        // 3. Always open the file stream in binary mode. The glTF SDK will handle any text
        //    encoding issues for us.
        auto streamPath = m_pathBase / std::filesystem::u8path(filename);
        auto stream = std::make_shared<std::ifstream>(streamPath, std::ios_base::binary);

        // Check if the stream has no errors and is ready for I/O operations
        if (!stream || !(*stream))
        {
            throw std::runtime_error("Unable to create a valid input stream for uri: " + filename);
        }

        return stream;
    }

private:
    std::filesystem::path m_pathBase;
};


template<typename T>
void parseVec2(std::vector<glm::vec2>* pos, const std::vector<T>& positions)
{
    for (size_t i = 0; i < positions.size(); i += 2)
    {
        pos->emplace_back(positions[i], positions[i + 1]);
    }
}

template<typename T>
void parseVec3(std::vector<glm::vec3>* pos, const std::vector<T>& positions)
{
    for (size_t i = 0; i < positions.size(); i += 3)
    {
        pos->emplace_back(positions[i], positions[i + 1], positions[i + 2]);
    }
}

template<typename T>
void parseVec4(std::vector<glm::vec4>* pos, const std::vector<T>& positions)
{
    for (size_t i = 0; i < positions.size(); i += 4)
    {
        pos->emplace_back(positions[i], positions[i + 1], positions[i + 2], positions[i + 3]);
    }
}

template<typename T>
void parseMat3(std::vector<glm::mat3>* pos, const std::vector<T>& positions)
{
    for (size_t i = 0; i < positions.size(); i += 9)
    {
        pos->emplace_back(  positions[i],       positions[i + 1], positions[i + 2],
                            positions[i + 3],   positions[i + 4], positions[i + 5],
                            positions[i + 6],   positions[i + 7], positions[i + 8]);
    }
}

template<typename T>
void parseMat4(std::vector<glm::mat4>* pos, const std::vector<T>& positions)
{
    for (size_t i = 0; i < positions.size(); i += 16)
    {
        pos->emplace_back(  positions[i],       positions[i + 1],   positions[i + 2],   positions[i + 3],
                            positions[i + 4],   positions[i + 5],   positions[i + 6],   positions[i + 7],
                            positions[i + 8],   positions[i + 9],   positions[i + 10],  positions[i + 11],
                            positions[i + 12],  positions[i + 13],  positions[i + 14],  positions[i + 15]);
    }
}

template<typename T>
void parseQuat(std::vector<Quaternion>* quat, const std::vector<T>& positions)
{
    for (size_t i = 0; i < positions.size(); i += 4)
    {
        quat->emplace_back(positions[i], positions[i + 1], positions[i + 2], positions[i + 3]);
    }
}

void glTF_file_data(const std::string& filepath, Scene::SceneContentConteiner* Scene)
{
    std::filesystem::path path = filepath;

    if (path.is_relative())
    {
        auto pathCurrent = std::filesystem::current_path();

        // Convert the relative path into an absolute path by appending the command line argument to the current path
        pathCurrent /= path;
        pathCurrent.swap(path);
    }

    if (!path.has_filename())
    {
        throw std::runtime_error("Command line argument path has no filename");
    }

    if (!path.has_extension())
    {
        throw std::runtime_error("Command line argument path has no filename extension");
    }

    // Pass the absolute path, without the filename, to the stream reader
    auto streamReader = std::make_unique<StreamReader>(path.parent_path());

    std::filesystem::path pathFile = path.filename();
    std::filesystem::path pathFileExt = pathFile.extension();

    std::string manifest;

    auto MakePathExt = [](const std::string& ext)
    {
        return "." + ext;
    };

    std::unique_ptr<Microsoft::glTF::GLTFResourceReader> resourceReader;

    // If the file has a '.gltf' extension then create a GLTFResourceReader
    if (pathFileExt == MakePathExt(Microsoft::glTF::GLTF_EXTENSION))
    {
        auto gltfStream = streamReader->GetInputStream(pathFile.u8string()); // Pass a UTF-8 encoded filename to GetInputString
        auto gltfResourceReader = std::make_unique<Microsoft::glTF::GLTFResourceReader>(std::move(streamReader));

        std::stringstream manifestStream;

        // Read the contents of the glTF file into a string using a std::stringstream
        manifestStream << gltfStream->rdbuf();
        manifest = manifestStream.str();

        resourceReader = std::move(gltfResourceReader);
    }

    // If the file has a '.glb' extension then create a GLBResourceReader. This class derives
    // from GLTFResourceReader and adds support for reading manifests from a GLB container's
    // JSON chunk and resource data from the binary chunk.
    if (pathFileExt == MakePathExt(Microsoft::glTF::GLB_EXTENSION))
    {
        auto glbStream = streamReader->GetInputStream(pathFile.u8string()); // Pass a UTF-8 encoded filename to GetInputString
        auto glbResourceReader = std::make_unique<Microsoft::glTF::GLBResourceReader>(std::move(streamReader), std::move(glbStream));

        manifest = glbResourceReader->GetJson(); // Get the manifest from the JSON chunk

        resourceReader = std::move(glbResourceReader);
    }

    if (!resourceReader)
    {
        throw std::runtime_error("Command line argument path filename extension must be .gltf or .glb");
    }

    Microsoft::glTF::Document document;

    try
    {
        document = Microsoft::glTF::Deserialize(manifest);
    }
    catch (const Microsoft::glTF::GLTFException& ex)
    {
        std::stringstream ss;

        ss << "Microsoft::glTF::Deserialize failed: ";
        ss << ex.what();

        throw std::runtime_error(ss.str());
    }

    std::cout << "### glTF Info - " << pathFile << " ###\n\n";
    /*  SCENES  
    for (const auto& scene : document.scenes.Elements())
    {
        for (const auto& node : scene.nodes)
        {
            if(document.nodes.Has(node));
            {
                std::cout << "name: " << document.nodes.Elements()[std::stoi(node)].name << std::endl;
                for (const auto& child : document.nodes.Elements()[std::stoi(node)].children)
                {
                    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%   Mesh   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
                    if (document.nodes.Elements()[std::stoi(child)].meshId != std::string())
                    {
                        std::cout << "name: " << document.nodes.Elements()[std::stoi(node)].name << std::endl;
                        if (document.meshes.Has(document.nodes.Elements()[std::stoi(child)].meshId))
                        {
                            const auto& mesh = document.meshes.Elements()[std::stoi(document.nodes.Elements()[std::stoi(child)].meshId)];

                            SceneContent::Mesh m = SceneContent::Mesh();
                            m.SetMeshId(std::stoi(mesh.id));
                            for (const auto& primetives : mesh.primitives)
                            {
                                for (const auto& attributs : primetives.attributes)
                                {
                                    const Microsoft::glTF::Accessor& accessor = document.accessors.Get(attributs.second);

                                    if (accessor.componentType == 5121)
                                        m.AddPrimetive(attributs.first, accessor.componentType, accessor.type, resourceReader->ReadBinaryData<unsigned char>(document, accessor));
                                    else if (accessor.componentType == 5123)
                                        m.AddPrimetive(attributs.first, accessor.componentType, accessor.type, resourceReader->ReadBinaryData<unsigned short>(document, accessor));
                                    else if (accessor.componentType == 5125)
                                        m.AddPrimetive(attributs.first, accessor.componentType, accessor.type, resourceReader->ReadBinaryData<unsigned int>(document, accessor));
                                    else if (accessor.componentType == 5126)
                                        m.AddPrimetive(attributs.first, accessor.componentType, accessor.type, resourceReader->ReadBinaryData<float>(document, accessor));
                                }
                                const Microsoft::glTF::Accessor& indicesAccessor = document.accessors.Get(primetives.indicesAccessorId);

                                if (indicesAccessor.componentType == 5123)
                                    m.AddIndices(indicesAccessor.componentType, resourceReader->ReadBinaryData<unsigned short>(document, indicesAccessor));
                                else if (indicesAccessor.componentType == 5125)
                                    m.AddIndices(indicesAccessor.componentType, resourceReader->ReadBinaryData<unsigned int>(document, indicesAccessor));
                            }
                            Scene->meshes.AddMesh(mesh.name, m);
                        }
                    }

                    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%   Skin   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
                    if (document.nodes.Elements()[std::stoi(child)].skinId != std::string())
                    {
                        if (document.skins.Has(document.nodes.Elements()[std::stoi(child)].skinId))
                        {
                            const auto& skin = document.skins.Elements()[std::stoi(document.nodes.Elements()[std::stoi(child)].skinId)];

                            const Microsoft::glTF::Accessor& inverseBindMatriceAccessor = document.accessors.Get(skin.inverseBindMatricesAccessorId);
                            std::cout << "first time inverseBindMatrice.bufferViewId: " << inverseBindMatriceAccessor.bufferViewId << std::endl;
                            std::cout << "first time inverseBindMatriceAccessor.componentType: " << inverseBindMatriceAccessor.componentType << std::endl;

                            std::vector<glm::mat4> inverseBindMatrix = std::vector<glm::mat4>();
                            parseMat4(&inverseBindMatrix, resourceReader->ReadBinaryData<float>(document, inverseBindMatriceAccessor));

                            int i = 0;
                            for (const auto& joint : skin.jointIds)
                            {
                                SceneContent::Joint j = SceneContent::Joint();

                                j.name = document.nodes.Elements()[std::stoi(joint)].name;

                                j.translation.x = document.nodes.Elements()[std::stoi(joint)].translation.x;
                                j.translation.y = document.nodes.Elements()[std::stoi(joint)].translation.y;
                                j.translation.z = document.nodes.Elements()[std::stoi(joint)].translation.z;

                                const auto& quat = document.nodes.Elements()[std::stoi(joint)].rotation;
                                j.rotation.init(quat.x, quat.y, quat.z, quat.w);

                                j.scale.x = document.nodes.Elements()[std::stoi(joint)].scale.x;
                                j.scale.y = document.nodes.Elements()[std::stoi(joint)].scale.y;
                                j.scale.z = document.nodes.Elements()[std::stoi(joint)].scale.z;

                                j.inverseBindMatrice = inverseBindMatrix[i];

                                for (const auto& child : document.nodes.Elements()[std::stoi(joint)].children)
                                    j.Children.emplace_back(document.nodes.Elements()[std::stoi(child)].name);

                                Scene->animator.addJoint(j);

                                i++;
                            }
                        }
                    }
                }
               
            }
        }
    }*/
    /*       SKINS       */
    for (const auto& skin : document.skins.Elements())
    {
        const Microsoft::glTF::Accessor& inverseBindMatriceAccessor = document.accessors.Get(skin.inverseBindMatricesAccessorId);
        //std::cout << "inverseBindMatrice.bufferViewId: " << inverseBindMatriceAccessor.bufferViewId << std::endl;
        //std::cout << "inverseBindMatriceAccessor.componentType: " << inverseBindMatriceAccessor.componentType << std::endl;

        std::vector<glm::mat4> inverseBindMatrix = std::vector<glm::mat4>();
        parseMat4(&inverseBindMatrix, resourceReader->ReadBinaryData<float>(document, inverseBindMatriceAccessor));
        
        //std::cout << "inverseBindMatrix: " << inverseBindMatrix.size() << std::endl;

        int i = 0;
        for (const auto& joint : skin.jointIds)
        {
            SceneContent::Joint j = SceneContent::Joint();

            j.name = document.nodes.Elements()[std::stoi(joint)].name;

            j.translation.x = document.nodes.Elements()[std::stoi(joint)].translation.x;
            j.translation.y = document.nodes.Elements()[std::stoi(joint)].translation.y;
            j.translation.z = document.nodes.Elements()[std::stoi(joint)].translation.z;

            const auto& quat = document.nodes.Elements()[std::stoi(joint)].rotation;
            j.rotation.init(quat.x, quat.y, quat.z, quat.w);

            j.scale.x = document.nodes.Elements()[std::stoi(joint)].scale.x;
            j.scale.y = document.nodes.Elements()[std::stoi(joint)].scale.y;
            j.scale.z = document.nodes.Elements()[std::stoi(joint)].scale.z;

            j.inverseBindMatrice = inverseBindMatrix[i];

            for (const auto& child : document.nodes.Elements()[std::stoi(joint)].children)
                j.Children.emplace_back(document.nodes.Elements()[std::stoi(child)].name);

            Scene->animator.addJoint(j);

            i++;
        }
    }
    
    /*       ANIMATIONS       */
    for (const auto& animation : document.animations.Elements())
    {
        //SceneContent::animations Animation = SceneContent::animations();
        SceneContent::Animation a = SceneContent::Animation();
        //std::cout << "######################" << std::endl;
        //std::cout << "animation.name: " << animation.name << std::endl;
        //std::cout << "######################\n" << std::endl;
        a.name = animation.name;
        for (const auto& channel : animation.channels.Elements())
        {
            
            const auto& node = document.nodes.Elements()[std::stoi(channel.target.nodeId)];
            //std::cout << "node Name: " << node.name << "\n" << std::endl;
            if (node.name != "Armature")
            {
                const Microsoft::glTF::Accessor& inputAccessor = document.accessors.Get(animation.samplers.Elements()[std::stoi(channel.samplerId)].inputAccessorId);
                const Microsoft::glTF::Accessor& outputAccessor = document.accessors.Get(animation.samplers.Elements()[std::stoi(channel.samplerId)].outputAccessorId);

                const auto& time = resourceReader->ReadBinaryData<float>(document, inputAccessor);
                const auto& maxtime = inputAccessor.max;

                a.addMaxTime(inputAccessor.max[0]);
                //std::cout << "chennel target path: " << channel.target.path << std::endl;
                a.addInputTime(resourceReader->ReadBinaryData<float>(document, inputAccessor));
                switch (channel.target.path)
                {
                case (size_t)AnimationTarget::MATRIX:
                    std::cout << "need to add matrix handeler" << std::endl;
                    throw("need to add matrix handeler");
                    break;
                case (size_t)AnimationTarget::TRANSLATION:
                    a.addTranslation(node.name, *&resourceReader->ReadBinaryData<float>(document, outputAccessor));
                    break;
                case (size_t)AnimationTarget::ROTATION:
                    a.addQuaternion(node.name, *&resourceReader->ReadBinaryData<float>(document, outputAccessor));
                    break;
                case (size_t)AnimationTarget::SCALE:
                    a.addScale(node.name, *&resourceReader->ReadBinaryData<float>(document, outputAccessor));
                    break;
                case (size_t)AnimationTarget::MORPH:
                    std::cout << "morph" << std::endl;
                    throw("need to add morph handeler");
                    break;
                default:
                    break;
                }
            }
        }
        Scene->animator.addAnimation(a);
    }

    /*       MESHES       */
    for (const auto& mesh : document.meshes.Elements())
    {
        SceneContent::Mesh m = SceneContent::Mesh();
        m.SetMeshId(std::stoi(mesh.id));
        for (const auto& primetives : mesh.primitives)
        {
            for (const auto& attributs : primetives.attributes)
            {
                const Microsoft::glTF::Accessor& accessor = document.accessors.Get(attributs.second);

                if (accessor.componentType == 5121)
                    m.AddPrimetive(attributs.first, accessor.componentType, accessor.type, *&resourceReader->ReadBinaryData<unsigned char>(document, accessor));
                else if (accessor.componentType == 5123)
                    m.AddPrimetive(attributs.first, accessor.componentType, accessor.type, *&resourceReader->ReadBinaryData<unsigned short>(document, accessor));
                else if (accessor.componentType == 5124)
                    m.AddPrimetive(attributs.first, accessor.componentType, accessor.type, *&resourceReader->ReadBinaryData<int>(document, accessor));
                else if (accessor.componentType == 5125)
                    m.AddPrimetive(attributs.first, accessor.componentType, accessor.type, *&resourceReader->ReadBinaryData<unsigned int>(document, accessor));
                else if (accessor.componentType == 5126)
                    m.AddPrimetive(attributs.first, accessor.componentType, accessor.type, *&resourceReader->ReadBinaryData<float>(document, accessor));
            }
            const Microsoft::glTF::Accessor& indicesAccessor = document.accessors.Get(primetives.indicesAccessorId);

            if (indicesAccessor.componentType == 5123)
                m.AddIndices(indicesAccessor.componentType, *&resourceReader->ReadBinaryData<unsigned short>(document, indicesAccessor));
            else if (indicesAccessor.componentType == 5125)
                m.AddIndices(indicesAccessor.componentType, *&resourceReader->ReadBinaryData<unsigned int>(document, indicesAccessor));


            const auto textures = document.materials.Elements()[std::stoi(primetives.materialId)].GetTextures();

            for (const auto& texture : textures)
            {
                if (!texture.first.empty())
                {
                    if (document.images.Elements().size() > (unsigned int)std::stoi(texture.first))
                    {
                        const auto& image = document.images.Elements()[std::stoi(texture.first)];

                        if (image.name.empty())
                        {
                            std::filesystem::path image_path = path;

                            image_path.remove_filename();
                            image_path += image.uri;

                            Scene->meshes.AddTexture(mesh.name, image_path.string());
                        }
                        else
                        {
                            Scene->meshes.AddTexture(mesh.name, image.name, *&resourceReader->ReadBinaryData<unsigned char>(document, document.bufferViews.Elements()[std::stoi(image.bufferViewId)]));
                        }
                    }
                    else
                    {
                        std::cout << "texture " << texture.first << " did not load...." << std::endl;
                    }
                }
            }
        }
        Scene->meshes.AddMesh(mesh.name, m);
    }
}


