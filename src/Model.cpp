// Onut
#include <onut/ContentManager.h>
#include <onut/Files.h>
#include <onut/IndexBuffer.h>
#include <onut/Log.h>
#include <onut/Model.h>
#include <onut/Renderer.h>
#include <onut/Strings.h>
#include <onut/Texture.h>
#include <onut/VertexBuffer.h>

// Third parties
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <json/json.h>
#include <stb/stb_image.h>

// STL
#include <cassert>
#include <fstream>

namespace onut
{
    OModelRef Model::createFromFile(const std::string& filename, const OContentManagerRef& pContentManager)
    {
        unsigned char white[4] = {255, 255, 255, 255};
        static auto WhiteTexture = Texture::createFromData(white, {1, 1}, false);

        float scale = 1.0f;
        bool rh = true;
        Axis upAxis = Axis::Z;

        std::string assetFilename = pContentManager->findResourceFile(filename);
        if (assetFilename.empty())
        {
            assetFilename = filename;
        }

        // Load config json.model (optional)
        if (onut::getExtension(filename) == "MODEL")
        {
            std::ifstream fic(assetFilename);
            Json::Value json;
            fic >> json;
            fic.close();

            if (json["name"].isString())
            {
                assetFilename = pContentManager->findResourceFile(json["name"].asString());
            }
            else
            {
                // Assume fbx
                assetFilename = pContentManager->findResourceFile(onut::getFilenameWithoutExtension(assetFilename) + ".fbx");
            }
            if (json["scale"].isNumeric())
            {
                scale = json["scale"].asFloat();
            }
            if (json["rh"].isBool())
            {
                rh = json["rh"].asBool();
            }
            if (json["up"].isString())
            {
                auto axis = onut::toUpper(json["up"].asString());
                if (axis == "Y") upAxis = Axis::Y;
                // Otherwise keep Z, and ignore X
            }
        }

        auto pRet = std::make_shared<Model>();

        auto options = 
            aiProcess_CalcTangentSpace |
            aiProcess_Triangulate |
            aiProcess_GenNormals |
            aiProcess_PreTransformVertices |
            aiProcess_JoinIdenticalVertices |
            aiProcess_FlipUVs |
            aiProcess_SortByPType |
            aiProcess_SplitLargeMeshes; // onut only draws in 16 bits index arrays

        if (!rh) options |= aiProcess_MakeLeftHanded;

        const aiScene* pScene = aiImportFile(assetFilename.c_str(), options);
        if (!pScene)
        {
            OLogE("Cannt load " + assetFilename);
            return nullptr;
        }
        assert(pScene);

        // Materials
        std::vector<OTextureRef> materials;
        materials.resize((int)pScene->mNumMaterials);
        for (int i = 0; i < (int)materials.size(); ++i)
        {
            auto pAssMat = pScene->mMaterials[i];

            aiString texturePath;
            auto ret = pAssMat->GetTexture(aiTextureType::aiTextureType_DIFFUSE, 0, &texturePath);
            if (ret == aiReturn_SUCCESS)
            {
                if (auto texture = pScene->GetEmbeddedTexture(texturePath.C_Str()))
                {
                    // Embedded
                    if (texture->mHeight == 0)
                    {
                        materials[i] = OTexture::createFromFileData((uint8_t*)texture->pcData, (int)texture->mWidth);
                    }
                    else
                    {
                        uint8_t* image = new uint8_t[texture->mWidth * texture->mHeight];
                        for (int i = 0, len = (int)texture->mWidth * texture->mHeight; i < len; ++i)
                        {
                            image[i * 4 + 0] = texture->pcData[i].r;
                            image[i * 4 + 1] = texture->pcData[i].g;
                            image[i * 4 + 2] = texture->pcData[i].b;
                            image[i * 4 + 3] = texture->pcData[i].a;
                        }
                        materials[i] = OTexture::createFromData(image, { (int)texture->mWidth, (int)texture->mHeight });
                        delete[] image;
                    }
                }
                else
                {
                    // File
                    auto texName = onut::getFilename(texturePath.C_Str());
                    materials[i] = OGetTexture(texName);
                }
            }
            else
            {
                materials[i] = WhiteTexture;
            }
        }

        // Meshes
        float mins[3] = {100000.f, 100000.f, 100000.f};
        float maxs[3] = {-100000.f, -100000.f, -100000.f};
#if defined(_DEBUG)
        int vertCount = 0;
#endif
        pRet->m_meshes.resize((int)pScene->mNumMeshes);
        for (int i = 0; i < (int)pRet->m_meshes.size(); ++i)
        {
            auto pMesh = pRet->m_meshes.data() + i;
            auto pAssMesh = pScene->mMeshes[i];

            pMesh->pTexture = materials[pAssMesh->mMaterialIndex];
            aiColor4D transparentColor;
            if (aiGetMaterialColor(pScene->mMaterials[pAssMesh->mMaterialIndex], AI_MATKEY_COLOR_TRANSPARENT, &transparentColor) != AI_SUCCESS)
            {
                transparentColor.r = 0.0f;
            }
            aiColor4D diffuseColor;
            if (pMesh->pTexture != WhiteTexture ||
                aiGetMaterialColor(pScene->mMaterials[pAssMesh->mMaterialIndex], AI_MATKEY_COLOR_DIFFUSE, &diffuseColor) != AI_SUCCESS)
            {
                diffuseColor.r = 1.0f;
                diffuseColor.g = 1.0f;
                diffuseColor.b = 1.0f;
                diffuseColor.a = 1.0f;
            }
            diffuseColor.a *= 1.0f - transparentColor.r;
            aiColor4D emissiveColor;
            if (aiGetMaterialColor(pScene->mMaterials[pAssMesh->mMaterialIndex], AI_MATKEY_COLOR_EMISSIVE, &emissiveColor) == AI_SUCCESS)
            {
                diffuseColor.r += emissiveColor.r;
                diffuseColor.g += emissiveColor.g;
                diffuseColor.b += emissiveColor.b;
            }

            if (!pAssMesh->mNumVertices)
            {
                OLogE("No vertices in " + assetFilename);
                return nullptr;
            }
            assert(pAssMesh->mNumVertices);

            pMesh->vertices.resize(pAssMesh->mNumVertices);
            MeshVertex* vertices = pMesh->vertices.data();
#if defined(_DEBUG)
            vertCount += (int)pAssMesh->mNumVertices;
#endif
            for (int i = 0; i < (int)pAssMesh->mNumVertices; ++i)
            {
                auto pVertex = vertices + i;
                if (upAxis == Axis::Z)
                {
                    pVertex->position[0] = pAssMesh->mVertices[i].x * scale;
                    pVertex->position[1] = -pAssMesh->mVertices[i].z * scale;
                    pVertex->position[2] = pAssMesh->mVertices[i].y * scale;
                }
                else
                {
                    pVertex->position[0] = pAssMesh->mVertices[i].x * scale;
                    pVertex->position[1] = pAssMesh->mVertices[i].y * scale;
                    pVertex->position[2] = pAssMesh->mVertices[i].z * scale;
                }
                mins[0] = std::min(mins[0], pVertex->position[0]);
                mins[1] = std::min(mins[1], pVertex->position[1]);
                mins[2] = std::min(mins[2], pVertex->position[2]);
                maxs[0] = std::max(maxs[0], pVertex->position[0]);
                maxs[1] = std::max(maxs[1], pVertex->position[1]);
                maxs[2] = std::max(maxs[2], pVertex->position[2]);
            }
            for (int i = 0; i < (int)pAssMesh->mNumVertices; ++i)
            {
                auto pVertex = vertices + i;
                if (upAxis == Axis::Z)
                {
                    pVertex->normal[0] = pAssMesh->mNormals[i].x;
                    pVertex->normal[1] = -pAssMesh->mNormals[i].z;
                    pVertex->normal[2] = pAssMesh->mNormals[i].y;
                }
                else
                {
                    pVertex->normal[0] = pAssMesh->mNormals[i].x;
                    pVertex->normal[1] = pAssMesh->mNormals[i].y;
                    pVertex->normal[2] = pAssMesh->mNormals[i].z;
                }
            }
            if (pAssMesh->HasVertexColors(0))
            {
                auto colorCnt = pAssMesh->GetNumColorChannels();
                for (int i = 0; i < (int)pAssMesh->mNumVertices; ++i)
                {
                    auto pVertex = vertices + i;
                    memcpy(pVertex->color, pAssMesh->mColors[0] + i * colorCnt, sizeof(float) * 4);
                    pVertex->color[0] *= diffuseColor.r;
                    pVertex->color[1] *= diffuseColor.g;
                    pVertex->color[2] *= diffuseColor.b;
                    pVertex->color[3] *= diffuseColor.a;
                }
            }
            else
            {
                for (int i = 0; i < (int)pAssMesh->mNumVertices; ++i)
                {
                    auto pVertex = vertices + i;
                    pVertex->color[0] = diffuseColor.r;
                    pVertex->color[1] = diffuseColor.g;
                    pVertex->color[2] = diffuseColor.b;
                    pVertex->color[3] = diffuseColor.a;
                }
            }
            if (pAssMesh->HasTextureCoords(0))
            {
                auto uvCnt = pAssMesh->GetNumUVChannels();
                for (int i = 0; i < (int)pAssMesh->mNumVertices; ++i)
                {
                    auto pVertex = vertices + i;
                    memcpy(pVertex->uv, pAssMesh->mTextureCoords[0] + i * uvCnt, sizeof(float) * 2);
                }
            }

            pMesh->pVertexBuffer = OVertexBuffer::createStatic(vertices, pAssMesh->mNumVertices * sizeof(MeshVertex));

            // Load faces
            pMesh->elementCount = (uint32_t)pAssMesh->mNumFaces * 3;
            if (pAssMesh->mNumFaces * 3 > std::numeric_limits<uint16_t>::max())
            {
                pMesh->indices32.resize(pMesh->elementCount);
                uint32_t* indices = pMesh->indices32.data();
                for (int i = 0; i < (int)pAssMesh->mNumFaces; ++i)
                {
                    indices[i * 3 + 0] = (uint32_t)pAssMesh->mFaces[i].mIndices[0];
                    indices[i * 3 + 1] = (uint32_t)pAssMesh->mFaces[i].mIndices[1];
                    indices[i * 3 + 2] = (uint32_t)pAssMesh->mFaces[i].mIndices[2];
                }
                pMesh->pIndexBuffer = OIndexBuffer::createStatic(indices, pMesh->elementCount * sizeof(uint32_t), 32);
            }
            else
            {
                pMesh->indices16.resize(pMesh->elementCount);
                uint16_t* indices = pMesh->indices16.data();
                for (int i = 0; i < (int)pAssMesh->mNumFaces; ++i)
                {
                    indices[i * 3 + 0] = (uint16_t)pAssMesh->mFaces[i].mIndices[0];
                    indices[i * 3 + 1] = (uint16_t)pAssMesh->mFaces[i].mIndices[1];
                    indices[i * 3 + 2] = (uint16_t)pAssMesh->mFaces[i].mIndices[2];
                }
                pMesh->pIndexBuffer = OIndexBuffer::createStatic(indices, pMesh->elementCount * sizeof(uint16_t), 16);
            }
        }

#if defined(_DEBUG)
        OLog(filename + " v cnt: " + std::to_string(vertCount) + ", dims: " 
            + std::to_string(maxs[0] - mins[0]) + " x "
            + std::to_string(maxs[1] - mins[1]) + " x "
            + std::to_string(maxs[2] - mins[2]) + ", center: "
            + std::to_string((maxs[0] + mins[0]) / 2) + ", "
            + std::to_string((maxs[1] + mins[1]) / 2) + ", "
            + std::to_string((maxs[2] + mins[2]) / 2));
#endif

        pRet->m_boundingBox[0] = { mins[0], mins[1], mins[2] };
        pRet->m_boundingBox[1] = { maxs[0], maxs[1], maxs[2] };

        aiReleaseImport(pScene);

        return pRet;
    }

    OModelRef Model::createFromBatch(const std::vector<Batch>& batch)
    {
        auto pRet = std::make_shared<Model>();

        for (const auto& batch : batch)
        {
            for (auto& srcMesh : batch.pModel->m_meshes)
            {
                bool foundMesh = false;
                auto vertsCopy = srcMesh.vertices;
                for (auto& vert : vertsCopy)
                {
                    Vector4 pos(vert.position[0], vert.position[1], vert.position[2], 1.0f);
                    pos = Vector4::Transform(pos, batch.transform);
                    vert.position[0] = pos.x;
                    vert.position[1] = pos.y;
                    vert.position[2] = pos.z;

                    Vector4 norm(vert.normal[0], vert.normal[1], vert.normal[2], 0.0f);
                    norm = Vector4::Transform(norm, batch.transform);
                    vert.normal[0] = norm.x;
                    vert.normal[1] = norm.y;
                    vert.normal[2] = norm.z;
                }
                for (auto& mesh : pRet->m_meshes)
                {
                    if (mesh.pTexture == srcMesh.pTexture) // Batch by material
                    {
                        foundMesh = true;
                        auto start = mesh.vertices.size();
                        mesh.vertices.insert(mesh.vertices.end(), vertsCopy.begin(), vertsCopy.end());
                        if ((!mesh.indices16.empty() && mesh.vertices.size() > std::numeric_limits<uint16_t>::max()) || !srcMesh.indices32.empty())
                        {
                            for (auto i : mesh.indices16)
                            {
                                mesh.indices32.push_back((uint32_t)i);
                            }
                            mesh.indices16.clear();
                        }
                        if (!mesh.indices16.empty())
                        {
                            mesh.elementCount += (uint32_t)srcMesh.indices16.size();
                            for (auto i : srcMesh.indices16)
                            {
                                mesh.indices16.push_back(i + (uint16_t)start);
                            }
                        }
                        else if (!mesh.indices32.empty())
                        {
                            if (!srcMesh.indices16.empty())
                            {
                                mesh.elementCount += (uint32_t)srcMesh.indices16.size();
                                for (auto i : srcMesh.indices16)
                                {
                                    mesh.indices32.push_back((uint32_t)i + (uint32_t)start);
                                }
                            }
                            else if (!srcMesh.indices32.empty())
                            {
                                mesh.elementCount += (uint32_t)srcMesh.indices32.size();
                                for (auto i : srcMesh.indices32)
                                {
                                    mesh.indices32.push_back(i + (uint32_t)start);
                                }
                            }
                        }
                    }
                }
                if (!foundMesh)
                {
                    pRet->m_meshes.push_back({ nullptr, nullptr, srcMesh.pTexture, srcMesh.elementCount, vertsCopy, srcMesh.indices16, srcMesh.indices32 });
                }
            }
        }

        // Generate draw buffers
        for (auto& mesh : pRet->m_meshes)
        {
            mesh.pVertexBuffer = OVertexBuffer::createStatic(mesh.vertices.data(), mesh.vertices.size() * sizeof(MeshVertex));
            if (!mesh.indices16.empty())
                mesh.pIndexBuffer = OIndexBuffer::createStatic(mesh.indices16.data(), mesh.indices16.size() * sizeof(uint16_t), 16);
            if (!mesh.indices32.empty())
                mesh.pIndexBuffer = OIndexBuffer::createStatic(mesh.indices32.data(), mesh.indices32.size() * sizeof(uint32_t), 32);
        }

        return pRet;
    }

    Model::~Model()
    {
        
    }
    
    int Model::getMeshCount() const
    {
        return (int)m_meshes.size();
    }

    Model::Mesh* Model::getMesh(int index)
    {
        return m_meshes.data() + index;
    }

    const Vector3* Model::getBoundingBox() const
    {
        return m_boundingBox;
    }

    void Model::render(const Matrix& transform)
    {
        auto pRenderer = oRenderer.get();
        auto& rs = pRenderer->renderStates;

        rs.world = transform;

        int len = (int)m_meshes.size();
        auto pMeshes = m_meshes.data();
        for (int i = 0; i < len; ++i)
        {
            rs.indexBuffer = pMeshes[i].pIndexBuffer;
            rs.vertexBuffer = pMeshes[i].pVertexBuffer;
            rs.textures[0] = pMeshes[i].pTexture;
            pRenderer->drawIndexed(pMeshes[i].elementCount, 0);
        }
    }
};

OModelRef OGetModel(const std::string& name)
{
    return oContentManager->getResourceAs<OModel>(name);
}

void ORenderModel(const std::string& name, const Matrix& transform)
{
    auto pModel = OGetModel(name);
    if (pModel)
    {
        pModel->render(transform);
    }
}
