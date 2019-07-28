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

// STL
#include <cassert>
#include <fstream>

struct MeshVertex
{
    float position[3];
    float normal[3];
    float color[4];
    float uv[2];
};

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
                auto texName = onut::getFilename(texturePath.C_Str());
                materials[i] = OGetTexture(texName);
            }
            else
            {
                materials[i] = WhiteTexture;
            }
        }

        // Meshes
#if defined(_DEBUG)
        float mins[3] = {10000.f, 10000.f, 10000.f};
        float maxs[3] = {-10000.f, -10000.f, -10000.f};
        int vertCount = 0;
#endif
        pRet->m_meshes.resize((int)pScene->mNumMeshes);
        for (int i = 0; i < (int)pRet->m_meshes.size(); ++i)
        {
            auto pMesh = pRet->m_meshes.data() + i;
            auto pAssMesh = pScene->mMeshes[i];

            pMesh->pTexture = materials[pAssMesh->mMaterialIndex];

            assert(pAssMesh->mNumVertices);

            MeshVertex* vertices = new MeshVertex[pAssMesh->mNumVertices];
#if defined(_DEBUG)
            vertCount += (int)pAssMesh->mNumVertices;
#endif
            for (int i = 0; i < (int)pAssMesh->mNumVertices; ++i)
            {
                auto pVertex = vertices + i;
                pVertex->position[0] = pAssMesh->mVertices[i].x * scale;
                pVertex->position[1] = -pAssMesh->mVertices[i].z * scale;
                pVertex->position[2] = pAssMesh->mVertices[i].y * scale;
#if defined(_DEBUG)
                mins[0] = std::min(mins[0], pVertex->position[0]);
                mins[1] = std::min(mins[1], pVertex->position[1]);
                mins[2] = std::min(mins[2], pVertex->position[2]);
                maxs[0] = std::max(maxs[0], pVertex->position[0]);
                maxs[1] = std::max(maxs[1], pVertex->position[1]);
                maxs[2] = std::max(maxs[2], pVertex->position[2]);
#endif
            }
            for (int i = 0; i < (int)pAssMesh->mNumVertices; ++i)
            {
                auto pVertex = vertices + i;
                pVertex->normal[0] = pAssMesh->mNormals[i].x;
                pVertex->normal[1] = -pAssMesh->mNormals[i].z;
                pVertex->normal[2] = pAssMesh->mNormals[i].y;
            }
            if (pAssMesh->HasVertexColors(0))
            {
                auto colorCnt = pAssMesh->GetNumColorChannels();
                for (int i = 0; i < (int)pAssMesh->mNumVertices; ++i)
                {
                    auto pVertex = vertices + i;
                    memcpy(pVertex->color, pAssMesh->mColors[0] + i * colorCnt, sizeof(float) * 4);
                }
            }
            else
            {
                for (int i = 0; i < (int)pAssMesh->mNumVertices; ++i)
                {
                    auto pVertex = vertices + i;
                    pVertex->color[0] = 1; pVertex->color[1] = 1; pVertex->color[2] = 1; pVertex->color[3] = 1;
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
            delete[] vertices;

            // Load faces
            pMesh->elementCount = (uint32_t)pAssMesh->mNumFaces * 3;
            if (pAssMesh->mNumFaces * 3 > std::numeric_limits<uint16_t>::max())
            {
                assert(false); // We only support 16 bits index arrays
            }
            else
            {
                uint16_t* indices = new uint16_t[pMesh->elementCount];
                for (int i = 0; i < (int)pAssMesh->mNumFaces; ++i)
                {
                    indices[i * 3 + 0] = (uint16_t)pAssMesh->mFaces[i].mIndices[0];
                    indices[i * 3 + 1] = (uint16_t)pAssMesh->mFaces[i].mIndices[1];
                    indices[i * 3 + 2] = (uint16_t)pAssMesh->mFaces[i].mIndices[2];
                }
                pMesh->pIndexBuffer = OIndexBuffer::createStatic(indices, pMesh->elementCount * sizeof(uint16_t));
                delete[] indices;
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

        aiReleaseImport(pScene);

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
