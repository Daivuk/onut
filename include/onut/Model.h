#ifndef MODEL_H_INCLUDED
#define MODEL_H_INCLUDED

// Onut
#include <onut/Axis.h>
#include <onut/Matrix.h>
#include <onut/Resource.h>
#include <onut/Vector3.h>

// STL
#include <vector>

// Forward
#include <onut/ForwardDeclaration.h>
OForwardDeclare(ContentManager);
OForwardDeclare(IndexBuffer);
OForwardDeclare(Model);
OForwardDeclare(Texture);
OForwardDeclare(VertexBuffer);

namespace onut
{
    class Model final : public Resource, public std::enable_shared_from_this<Model>
    {
    public:
        struct MeshVertex
        {
            float position[3];
            float normal[3];
            float color[4];
            float uv[2];
        };

        struct Mesh
        {
            OIndexBufferRef pIndexBuffer;
            OVertexBufferRef pVertexBuffer;
            OTextureRef pTexture;
            uint32_t elementCount;

            // Raw data. We keep that for batching
            std::vector<MeshVertex> vertices;
            std::vector<uint16_t> indices16;
            std::vector<uint32_t> indices32;
        };

        struct Batch
        {
            OModelRef pModel;
            Matrix transform;
        };

        static OModelRef createFromFile(const std::string& filename, const OContentManagerRef& pContentManager = nullptr);
        static OModelRef createFromBatch(const std::vector<Batch>& batch);

        ~Model();
        int getMeshCount() const;
        Mesh* getMesh(int index);
        const Vector3* getBoundingBox() const;

        void render(const Matrix& transform);

    private:
        std::vector<Mesh> m_meshes;
        Vector3 m_boundingBox[2];
    };
}

OModelRef OGetModel(const std::string& name);
void ORenderModel(const std::string& name, const Matrix& transform);

#endif
