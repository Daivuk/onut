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
        struct Mesh
        {
            OIndexBufferRef pIndexBuffer;
            OVertexBufferRef pVertexBuffer;
            OTextureRef pTexture;
            uint32_t elementCount;
        };

        static OModelRef createFromFile(const std::string& filename, const OContentManagerRef& pContentManager = nullptr);

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
