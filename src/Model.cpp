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
#include <onut/Shader.h>

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
    static void loadArmature(Model::Node& node, aiNode* pAiNode)
    {
        node.name = pAiNode->mName.C_Str();
        node.chidren.resize(pAiNode->mNumChildren);
        node.transform = Matrix(&pAiNode->mTransformation.a1).Transpose();
        for (int i = 0; i < (int)pAiNode->mNumChildren; ++i)
        {
            loadArmature(node.chidren[i], pAiNode->mChildren[i]);
        }
    }


    static aiNodeAnim* findNodeAnim(aiAnimation* pAssAnim, const std::string& nodeName)
    {
        for (int i = 0; i < (int)pAssAnim->mNumChannels; i++)
        {
            auto pNodeAnim = pAssAnim->mChannels[i];
            if (nodeName == pNodeAnim->mNodeName.C_Str())
            {
                return pNodeAnim;
            }
        }
        return nullptr;
    }

    static int findRotation(double t, const aiNodeAnim* pNodeAnim)
    {
        assert(pNodeAnim->mNumRotationKeys > 0);

        for (int i = 0; i < (int)pNodeAnim->mNumRotationKeys - 1; i++)
        {
            if (t < pNodeAnim->mRotationKeys[i + 1].mTime)
            {
                return i;
            }
        }

        assert(0);
        return -1;
    }

    static int findScaling(double t, const aiNodeAnim* pNodeAnim)
    {
        assert(pNodeAnim->mNumScalingKeys > 0);

        for (int i = 0; i < (int)pNodeAnim->mNumScalingKeys - 1; i++)
        {
            if (t < pNodeAnim->mScalingKeys[i + 1].mTime)
            {
                return i;
            }
        }

        assert(0);
        return -1;
    }

    static int findPosition(double t, const aiNodeAnim* pNodeAnim)
    {
        assert(pNodeAnim->mNumPositionKeys > 0);

        for (int i = 0; i < (int)pNodeAnim->mNumPositionKeys - 1; i++)
        {
            if (t < pNodeAnim->mPositionKeys[i + 1].mTime)
            {
                return i;
            }
        }

        assert(0);
        return -1;
    }

    static void calcInterpolatedRotation(aiQuaternion& out, double t, const aiNodeAnim* pNodeAnim)
    {
        // we need at least two values to interpolate...
        if (pNodeAnim->mNumRotationKeys == 1)
        {
            out = pNodeAnim->mRotationKeys[0].mValue;
            return;
        }

        int index = findRotation(t, pNodeAnim);
        int nextIndex = (index + 1);
        assert(nextIndex < (int)pNodeAnim->mNumRotationKeys);
        double deltaTime = pNodeAnim->mRotationKeys[nextIndex].mTime - pNodeAnim->mRotationKeys[index].mTime;
        double factor = (t - pNodeAnim->mRotationKeys[index].mTime) / deltaTime;
        assert(factor >= 0.0 && factor <= 1.0);
        const aiQuaternion& StartRotationQ = pNodeAnim->mRotationKeys[index].mValue;
        const aiQuaternion& EndRotationQ = pNodeAnim->mRotationKeys[nextIndex].mValue;
        aiQuaternion::Interpolate(out, StartRotationQ, EndRotationQ, (float)factor);
        out = out.Normalize();
    }

    static void calcInterpolatedScaling(aiVector3D& out, double t, const aiNodeAnim* pNodeAnim)
    {
        // we need at least two values to interpolate...
        if (pNodeAnim->mNumScalingKeys == 1)
        {
            out = pNodeAnim->mScalingKeys[0].mValue;
            return;
        }

        int index = findScaling(t, pNodeAnim);
        int nextIndex = (index + 1);
        assert(nextIndex < (int)pNodeAnim->mNumScalingKeys);
        double deltaTime = pNodeAnim->mScalingKeys[nextIndex].mTime - pNodeAnim->mScalingKeys[index].mTime;
        double factor = (t - pNodeAnim->mScalingKeys[index].mTime) / deltaTime;
        assert(factor >= 0.0 && factor <= 1.0);
        const auto& start = pNodeAnim->mScalingKeys[index].mValue;
        const auto& end = pNodeAnim->mScalingKeys[nextIndex].mValue;
        out = start + (end - start) * (float)factor;
    }

    static void calcInterpolatedPosition(aiVector3D& out, double t, const aiNodeAnim* pNodeAnim)
    {
        // we need at least two values to interpolate...
        if (pNodeAnim->mNumPositionKeys == 1)
        {
            out = pNodeAnim->mPositionKeys[0].mValue;
            return;
        }

        int index = findPosition(t, pNodeAnim);
        int nextIndex = (index + 1);
        assert(nextIndex < (int)pNodeAnim->mNumPositionKeys);
        double deltaTime = pNodeAnim->mPositionKeys[nextIndex].mTime - pNodeAnim->mPositionKeys[index].mTime;
        double factor = (t - pNodeAnim->mPositionKeys[index].mTime) / deltaTime;
        assert(factor >= 0.0 && factor <= 1.0);
        const auto& start = pNodeAnim->mPositionKeys[index].mValue;
        const auto& end = pNodeAnim->mPositionKeys[nextIndex].mValue;
        out = start + (end - start) * (float)factor;
    }

    static const aiNode* findParent(const aiNode* node, const aiString& childName)
    {
        for (unsigned int i = 0; i < node->mNumChildren; ++i)
        {
            const aiNode* child = node->mChildren[i];
            if (child->mName == childName)
            {
                return node;
            }

            const aiNode* foundParent = findParent(child, childName);
            if (foundParent)
            {
                return foundParent;
            }
        }

        return nullptr;
    }

    // Reference: http://ogldev.atspace.co.uk/www/tutorial38/tutorial38.html
    static void bakeAnimBones(double t,
                              aiAnimation* pAssAnim, 
                              const Model::Node& node,
                              const std::unordered_map<std::string, int>& boneMapping, 
                              std::vector<Matrix>& bones,
                              std::vector<Matrix>& boneOffsets,
                              const Matrix& invRootTransform,
                              const Matrix& parentTransform)
    {
        auto pAssNodeAnim = findNodeAnim(pAssAnim, node.name);
        
        auto nodeTransformation = node.transform;
        if (pAssNodeAnim)
        {
            // Interpolate scaling and generate scaling transformation matrix
            aiVector3D scaling;
            calcInterpolatedScaling(scaling, t, pAssNodeAnim);
            auto scalingM = Matrix::CreateScale(scaling.x, scaling.y, scaling.z);

            // Interpolate rotation and generate rotation transformation matrix
            aiQuaternion rotationQ;
            calcInterpolatedRotation(rotationQ, t, pAssNodeAnim);
            auto qMat = rotationQ.GetMatrix();
            auto rotationM = Matrix(qMat.a1, qMat.b1, qMat.c1, 0,
                                    qMat.a2, qMat.b2, qMat.c2, 0,
                                    qMat.a3, qMat.b3, qMat.c3, 0,
                                    0, 0, 0, 1);

            // Interpolate translation and generate translation transformation matrix
            aiVector3D translation;
            calcInterpolatedPosition(translation, t, pAssNodeAnim);
            auto translationM = Matrix::CreateTranslation(translation.x, translation.y, translation.z);

            // Combine the above transformations
            //nodeTransformation = translationM * rotationM * scalingM;
            nodeTransformation = scalingM * rotationM * translationM;
        }

        //auto globalTransformation = parentTransform * nodeTransformation;
        auto globalTransformation = nodeTransformation * parentTransform;

        auto it = boneMapping.find(node.name);
        if (it != boneMapping.end())
        {
            auto boneIndex = it->second;
            //bones[boneIndex] = invRootTransform * globalTransformation * boneOffsets[boneIndex];
            bones[boneIndex] = boneOffsets[boneIndex] * globalTransformation * invRootTransform;
        }

        for (const auto& child : node.chidren)
        {
            bakeAnimBones(t, 
                          pAssAnim,
                          child, 
                          boneMapping,
                          bones,
                          boneOffsets,
                          invRootTransform,
                          globalTransformation);
        }
    }

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
            //aiProcess_PreTransformVertices |
            aiProcess_JoinIdenticalVertices |
            aiProcess_FlipUVs |
            aiProcess_SortByPType |
            aiProcess_SplitLargeMeshes; // onut only draws in 16 bits index arrays (no.. not anymore, but ok)

        if (!rh) options |= aiProcess_MakeLeftHanded;

        const aiScene* pScene = aiImportFile(assetFilename.c_str(), options);
        if (!pScene)
        {
            OLogE("Cannot load " + assetFilename);
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
        float meshScale = scale;
        auto meshUpAxis = upAxis;
        if (pScene->HasAnimations())
        {
            // We will transform the root matrix instead
            meshScale = 1.0f;
            meshUpAxis = onut::Axis::Y;
        }

        float mins[3] = {100000.f, 100000.f, 100000.f};
        float maxs[3] = {-100000.f, -100000.f, -100000.f};
#if defined(_DEBUG)
        int vertCount = 0;
#endif
        pRet->m_meshes.resize((int)pScene->mNumMeshes);
        for (int j = 0; j < (int)pRet->m_meshes.size(); ++j)
        {
            auto pMesh = pRet->m_meshes.data() + j;
            auto pAssMesh = pScene->mMeshes[j];

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

            int vertexSize = 6;
            int posOff = 0;
            int normOff = 3;
            int colOff = 6;
            int uvOff = 6;
            int weightOff = 6;
            pMesh->vertexFlags = ONUT_MODEL_VERTEX_HAS_POSITION | ONUT_MODEL_VERTEX_HAS_NORMAL;
            if (pAssMesh->HasVertexColors(0))
            {
                vertexSize += 4;
                uvOff += 4;
                weightOff += 4;
                pMesh->vertexFlags |= ONUT_MODEL_VERTEX_HAS_COLOR;
            }
            if (pAssMesh->HasTextureCoords(0))
            {
                vertexSize += 2;
                weightOff += 2;
                pMesh->vertexFlags |= ONUT_MODEL_VERTEX_HAS_UV;
            }
            if (pAssMesh->HasBones())
            {
                vertexSize += 8;
                pMesh->vertexFlags |= ONUT_MODEL_VERTEX_HAS_WEIGHTS;
            }
            pMesh->vertexSize = vertexSize;
            pMesh->pVS = oRenderer->get3DVSForInput(pMesh->vertexFlags & ONUT_MODEL_VERTEX_HAS_COLOR,
                                                    pMesh->vertexFlags & ONUT_MODEL_VERTEX_HAS_UV,
                                                    pMesh->vertexFlags & ONUT_MODEL_VERTEX_HAS_WEIGHTS);
            pMesh->pPS = oRenderer->get3DPSForInput(pMesh->vertexFlags & ONUT_MODEL_VERTEX_HAS_UV);

            pMesh->vertices.resize(pAssMesh->mNumVertices * vertexSize);
            float* vertices = pMesh->vertices.data();
#if defined(_DEBUG)
            vertCount += (int)pAssMesh->mNumVertices;
#endif
            for (int i = 0; i < (int)pAssMesh->mNumVertices; ++i)
            {
                auto pVertex = vertices + i * vertexSize;
                if (meshUpAxis == Axis::Z)
                {
                    pVertex[posOff + 0] = pAssMesh->mVertices[i].x * meshScale;
                    pVertex[posOff + 1] = -pAssMesh->mVertices[i].z * meshScale;
                    pVertex[posOff + 2] = pAssMesh->mVertices[i].y * meshScale;
                }
                else
                {
                    pVertex[posOff + 0] = pAssMesh->mVertices[i].x * meshScale;
                    pVertex[posOff + 1] = pAssMesh->mVertices[i].y * meshScale;
                    pVertex[posOff + 2] = pAssMesh->mVertices[i].z * meshScale;
                }
                //auto transformed = Vector3::Transform(Vector3(pVertex->position), rootTransform);
                //pVertex->position[0] = transformed.x;
                //pVertex->position[1] = transformed.y;
                //pVertex->position[2] = transformed.z;
                mins[0] = std::min(mins[0], pVertex[posOff + 0]);
                mins[1] = std::min(mins[1], pVertex[posOff + 1]);
                mins[2] = std::min(mins[2], pVertex[posOff + 2]);
                maxs[0] = std::max(maxs[0], pVertex[posOff + 0]);
                maxs[1] = std::max(maxs[1], pVertex[posOff + 1]);
                maxs[2] = std::max(maxs[2], pVertex[posOff + 2]);
            }
            for (int i = 0; i < (int)pAssMesh->mNumVertices; ++i)
            {
                auto pVertex = vertices + i * vertexSize;
                if (meshUpAxis == Axis::Z)
                {
                    pVertex[normOff + 0] = pAssMesh->mNormals[i].x;
                    pVertex[normOff + 1] = -pAssMesh->mNormals[i].z;
                    pVertex[normOff + 2] = pAssMesh->mNormals[i].y;
                }
                else
                {
                    pVertex[normOff + 0] = pAssMesh->mNormals[i].x;
                    pVertex[normOff + 1] = pAssMesh->mNormals[i].y;
                    pVertex[normOff + 2] = pAssMesh->mNormals[i].z;
                }
            }
            if (pAssMesh->HasVertexColors(0))
            {
                auto colorCnt = pAssMesh->GetNumColorChannels();
                for (int i = 0; i < (int)pAssMesh->mNumVertices; ++i)
                {
                    auto pVertex = vertices + i * vertexSize;
                    memcpy(pVertex + colOff, pAssMesh->mColors[0] + i * colorCnt, sizeof(float) * 4);
                    pVertex[colOff + 0] *= diffuseColor.r;
                    pVertex[colOff + 1] *= diffuseColor.g;
                    pVertex[colOff + 2] *= diffuseColor.b;
                    pVertex[colOff + 3] *= diffuseColor.a;
                }
            }
            //else
            //{
            //    for (int i = 0; i < (int)pAssMesh->mNumVertices; ++i)
            //    {
            //        auto pVertex = vertices + i;
            //        pVertex->color[0] = diffuseColor.r;
            //        pVertex->color[1] = diffuseColor.g;
            //        pVertex->color[2] = diffuseColor.b;
            //        pVertex->color[3] = diffuseColor.a;
            //    }
            //}
            if (pAssMesh->HasTextureCoords(0))
            {
                auto uvCnt = pAssMesh->GetNumUVChannels();
                for (int i = 0; i < (int)pAssMesh->mNumVertices; ++i)
                {
                    auto pVertex = vertices + i * vertexSize;
                    memcpy(pVertex + uvOff, pAssMesh->mTextureCoords[0] + i * uvCnt, sizeof(float) * 2);
                }
            }

            // Bone weights
            if (pAssMesh->HasBones())
            {
                // Initialize all weights to zero
                for (int i = 0; i < (int)pAssMesh->mNumVertices; ++i)
                {
                    auto pVertex = vertices + i * vertexSize;
                    memset(pVertex + weightOff, 0, sizeof(float) * 8);
                }
                pMesh->bones.resize(pAssMesh->mNumBones);
                for (int i = 0; i < (int)pAssMesh->mNumBones; ++i)
                {
                    auto pAssBone = pAssMesh->mBones[i];
                    auto transform = Matrix(&pAssBone->mOffsetMatrix.a1).Transpose();
                    pMesh->bones[i] = transform;
                    pMesh->boneMapping[pAssBone->mName.C_Str()] = i;
                    for (int w = 0; w < (int)pAssBone->mNumWeights; ++w)
                    {
                        auto pWeight = pAssBone->mWeights + w;
                        auto idx = pWeight->mVertexId;
                        auto weight = pWeight->mWeight;
                        auto pVertex = vertices + idx * vertexSize;
                        pVertex[weightOff + 0] = (float)i;
                        pVertex[weightOff + 1] = weight;
                    }
                }

                // Find bone's parents using the root node for the entire scene... not great assimp, not great.
                pMesh->bone_parents.resize(pMesh->bones.size());
                for (int i = 0; i < (int)pAssMesh->mNumBones; ++i)
                {
                    pMesh->bone_parents[i] = -1;
                    auto pAssBone = pAssMesh->mBones[i];
                    const aiNode* pParentNode = findParent(pScene->mRootNode, pAssBone->mName);
                    if (pParentNode)
                    {
                        auto it = pMesh->boneMapping.find(pParentNode->mName.C_Str());
                        if (it != pMesh->boneMapping.end())
                        {
                            pMesh->bone_parents[i] = it->second;
                        }
                    }
                }
            }

            pMesh->pVertexBuffer = OVertexBuffer::createStatic(vertices, pAssMesh->mNumVertices * sizeof(float) * vertexSize);

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

        // Animations
        if (pScene->HasAnimations())
        {
            // Load Armature
            loadArmature(pRet->m_root, pScene->mRootNode);

            pRet->m_root.transform =
                Matrix::CreateScale(scale) *
                ((upAxis == onut::Axis::Z) ? Matrix::CreateRotationX(OPIDIV2) : Matrix::Identity) *
                pRet->m_root.transform;

            auto invRootTransform = Matrix(&pScene->mRootNode->mTransformation.a1).Transpose().Invert();
            pRet->m_anims.resize(pScene->mNumAnimations);
            for (int i = 0, len = (int)pScene->mNumAnimations; i < len; ++i)
            {
                auto pAssAnim = pScene->mAnimations[i];
                auto pAnim = pRet->m_anims.data() + i;

                pAnim->name = pAssAnim->mName.C_Str();
                auto splits = onut::splitString(pAnim->name, '|');
                if (splits.size() == 2) pAnim->name = splits[1];
                double fps = 25.0f;
                if (pAssAnim->mTicksPerSecond != 0.0) fps = (double)pAssAnim->mTicksPerSecond;
                pAnim->frameCount = (int)pAssAnim->mDuration;
                pAnim->duration = (double)(pAssAnim->mDuration / fps);

                // We load the frames for each meshes
                auto meshCount = (int)pRet->m_meshes.size();
                pAnim->animMeshes.resize(meshCount);
                for (int m = 0; m < meshCount; ++m)
                {
                    auto pMesh = pRet->m_meshes.data() + m;
                    pAnim->animMeshes[m].frames.resize(pAnim->frameCount);
                    for (int f = 0; f < pAnim->frameCount; ++f)
                    {
                        //double percent = (double)f / (double)pAnim->frameCount;
                        //double t = percent * pAnim->duration;
                        double t = std::fmod((double)f, pAssAnim->mDuration);
                        auto& animFrame = pAnim->animMeshes[m].frames[f];
                        animFrame.bones.resize((int)pMesh->bones.size());
                        bakeAnimBones(t, 
                                      pAssAnim,
                                      pRet->m_root, 
                                      pMesh->boneMapping,
                                      animFrame.bones, 
                                      pMesh->bones,
                                      invRootTransform,
                                      Matrix::Identity);
                    }
                }
            }
        }

//#if defined(_DEBUG)
//        OLog(filename + " v cnt: " + std::to_string(vertCount) + ", dims: " 
//            + std::to_string(maxs[0] - mins[0]) + " x "
//            + std::to_string(maxs[1] - mins[1]) + " x "
//            + std::to_string(maxs[2] - mins[2]) + ", center: "
//            + std::to_string((maxs[0] + mins[0]) / 2) + ", "
//            + std::to_string((maxs[1] + mins[1]) / 2) + ", "
//            + std::to_string((maxs[2] + mins[2]) / 2));
//#endif

        pRet->m_boundingBox[0] = { mins[0], mins[1], mins[2] };
        pRet->m_boundingBox[1] = { maxs[0], maxs[1], maxs[2] };

        aiReleaseImport(pScene);

        return pRet;
    }

    OModelRef Model::createFromBatch(const std::vector<Batch>& batch)
    {
        auto pRet = std::make_shared<Model>();
        /*
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
                    pRet->m_meshes.push_back({ nullptr, nullptr, srcMesh.pTexture, srcMesh.elementCount, {}, vertsCopy, srcMesh.indices16, srcMesh.indices32 });
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
        }*/

        return pRet;
    }

    Model::~Model()
    {
        
    }

    const Model::Node& Model::getRoot() const
    {
        return m_root;
    }

    int Model::getAnimId(const std::string& name) const
    {
        for (int i = 0, len = (int)m_anims.size(); i < len; ++i)
        {
            if (m_anims[i].name == name) return i;
        }
        return -1;
    }

    const Model::Anim* Model::getAnim(int index) const
    {
        return m_anims.data() + index;
    }

    const Model::Anim* Model::getAnim(const std::string& name) const
    {
        auto id = getAnimId(name);
        if (id == -1) return nullptr;
        return getAnim(id);
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

    void Model::render(const Matrix& transform, const OShaderRef& customVS, const OShaderRef& customPS)
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
            rs.vertexShader = customVS ? customVS : pMeshes[i].pVS;
            rs.pixelShader = customPS ? customPS : pMeshes[i].pPS;
            if (pMeshes[i].vertexFlags & ONUT_MODEL_VERTEX_HAS_WEIGHTS)
            {
                rs.vertexShader.get()->setMatrixArray("bones", pMeshes[i].bones.data(), (int)pMeshes[i].bones.size());
            }
            pRenderer->drawIndexed(pMeshes[i].elementCount, 0);
        }
    }

    void Model::render(const Matrix& transform, const Anim* pAnim, double time, const OShaderRef& customVS, const OShaderRef& customPS)
    {
        if (pAnim == nullptr)
        {
            render(transform, customVS, customPS);
            return;
        }

        int framei = (int)((time / pAnim->duration) * (double)pAnim->frameCount);
        framei %= pAnim->frameCount;

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
            rs.vertexShader = customVS ? customVS : pMeshes[i].pVS;
            rs.pixelShader = customPS ? customPS : pMeshes[i].pPS;
            if (pMeshes[i].vertexFlags & ONUT_MODEL_VERTEX_HAS_WEIGHTS)
            {
                const auto& bones = pAnim->animMeshes[i].frames[framei].bones;
                rs.vertexShader.get()->setMatrixArray("bones", bones.data(), (int)bones.size());
            }
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
