#ifndef MODEL_H_INCLUDED
#define MODEL_H_INCLUDED

// Onut
#include <onut/Axis.h>
#include <onut/Matrix.h>
#include <onut/Resource.h>
#include <onut/Vector3.h>

// STL
#include <vector>
#include <string>
#include <unordered_map>

// Forward
#include <onut/ForwardDeclaration.h>
OForwardDeclare(ContentManager);
OForwardDeclare(IndexBuffer);
OForwardDeclare(Model);
OForwardDeclare(Texture);
OForwardDeclare(VertexBuffer);
OForwardDeclare(Shader);

#define ONUT_MODEL_VERTEX_HAS_POSITION  0x01
#define ONUT_MODEL_VERTEX_HAS_NORMAL    0x02
#define ONUT_MODEL_VERTEX_HAS_COLOR     0x04
#define ONUT_MODEL_VERTEX_HAS_UV        0x08
#define ONUT_MODEL_VERTEX_HAS_WEIGHTS   0x10

namespace onut
{
    class Model final : public Resource, public std::enable_shared_from_this<Model>
    {
    public:
        struct AnimFrame
        {
            std::vector<Matrix> bones;
        };

        struct AnimMesh
        {
            std::vector<AnimFrame> frames;
        };

        struct Anim
        {
            double duration;
            int frameCount;
            std::vector<AnimMesh> animMeshes;
            std::string name;
        };

        struct Mesh
        {
            OIndexBufferRef pIndexBuffer;
            OVertexBufferRef pVertexBuffer;
            OShaderRef pVS;
            OShaderRef pPS;
            OTextureRef pTexture;
            uint32_t elementCount;
            std::vector<Matrix> bones;
            std::unordered_map<std::string, int> boneMapping;
            int vertexFlags = 0;
            int vertexSize = 6;
            std::vector<float> vertices;
            std::vector<uint16_t> indices16;
            std::vector<uint32_t> indices32;
        };

        struct Batch
        {
            OModelRef pModel;
            Matrix transform;
        };

        struct Node
        {
            Matrix transform;
            std::vector<Node> chidren;
            std::string name;
        };

        static OModelRef createFromFile(const std::string& filename, const OContentManagerRef& pContentManager = nullptr);
        static OModelRef createFromBatch(const std::vector<Batch>& batch);
        //static OModelRef createFromData(uint16_t vertexCount, const float* vertices, uint32_t indiceCount, const uint16_t* indices, const OTextureRef& texture, int vertexFlags = ONUT_MODEL_VERTEX_HAS_POSITION | ONUT_MODEL_VERTEX_HAS_NORMAL | ONUT_MODEL_VERTEX_HAS_COLOR | ONUT_MODEL_VERTEX_HAS_UV);

        ~Model();
        int getMeshCount() const;
        Mesh* getMesh(int index);
        const Vector3* getBoundingBox() const;
        const Node& getRoot() const;
        int getAnimId(const std::string& name) const;
        const Anim* getAnim(int index) const;
        const Anim* getAnim(const std::string& name) const;

        void render(const Matrix& transform, const OShaderRef& customVS = nullptr, const OShaderRef& customPS = nullptr);
        void render(const Matrix& transform, const Anim* pAnim, double time, const OShaderRef& customVS = nullptr, const OShaderRef& customPS = nullptr);

    private:
        std::vector<Mesh> m_meshes;
        std::vector<Anim> m_anims;
        Node m_root;
        Vector3 m_boundingBox[2];
    };
}

OModelRef OGetModel(const std::string& name);
void ORenderModel(const std::string& name, const Matrix& transform);

#endif
