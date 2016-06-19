#pragma once
// Onut
#include <onut/Point.h>
#include <onut/Maths.h>
#include <onut/Resource.h>

// STL
#include <unordered_map>

// Forward
#include <onut/ForwardDeclaration.h>
OForwardDeclare(ContentManager);
OForwardDeclare(Texture);
OForwardDeclare(TiledMap);

namespace onut
{
    class TiledMap final : public Resource
    {
    public:
        struct Layer
        {
            virtual ~Layer();
            std::string name;
            bool isVisible = true;
        };

        struct TileLayer : public Layer
        {
            virtual ~TileLayer();
            int width;
            int height;
            uint32_t *tileIds = nullptr;
        };

        struct Object
        {
            Vector2 position;
            Vector2 size;
            uint32_t id;
            std::string name;
            std::string type;
            std::unordered_map<std::string, std::string> properties;
        };

        struct TileSet
        {
            int firstId;
            int tileWidth;
            int tileHeight;
            OTextureRef pTexture;
            std::string name;
        };

        struct ObjectLayer : public Layer
        {
            virtual ~ObjectLayer();
            uint32_t objectCount = 0;
            Object *pObjects = nullptr;
        };

        static OTiledMapRef createFromFile(const std::string &filename, const OContentManagerRef& pContentManager = nullptr);

        ~TiledMap();

        const Matrix &getTransform() const { return m_transform; }
        void setTransform(const Matrix &transform) { m_transform = transform; }

        int getWidth() const { return m_width; }
        int getHeight() const { return m_height; }

        void render();
        void renderLayer(int index);
        void renderLayer(const std::string &name);
        void renderLayer(Layer *pLayer);
        void render(const iRect &rect);
        void renderLayer(const iRect &rect, int index);
        void renderLayer(const iRect &rect, const std::string &name);
        void renderLayer(const iRect &rect, Layer *pLayer);

        TileSet* getTileSet(int index) const;

        int getLayerCount() const { return m_layerCount; }
        Layer *getLayer(int index) const { return m_layers[index]; }
        Layer *getLayer(const std::string &name) const;

        const OTextureRef& getMinimap();

    private:
        struct Tile
        {
            TileSet *pTileset = nullptr;
            Rect rect;
            Vector4 UVs;
        };

        struct TileLayerInternal : public TileLayer
        {
            virtual ~TileLayerInternal();
            Tile *tiles = nullptr;
        };

        int m_width = 0;
        int m_height = 0;
        int m_layerCount = 0;
        int m_tilesetCount = 0;
        Layer **m_layers = nullptr;
        TileSet *m_tileSets = nullptr;
        Matrix m_transform = Matrix::Identity;
        OTextureRef m_pMinimap;
    };
};

OTiledMapRef OGetTiledMap(const std::string& name);
