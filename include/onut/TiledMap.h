#ifndef TILEDMAP_H_INCLUDED
#define TILEDMAP_H_INCLUDED

// Onut
#include <onut/Point.h>
#include <onut/Maths.h>
#include <onut/Resource.h>
#include <onut/SampleMode.h>

// Thirs party
#include <micropather/micropather.h>

// STL
#include <unordered_map>
#include <vector>

// Forward
#include <onut/ForwardDeclaration.h>
OForwardDeclare(ContentManager);
OForwardDeclare(IndexBuffer);
OForwardDeclare(Texture);
OForwardDeclare(TiledMap);
OForwardDeclare(VertexBuffer);

namespace onut
{
    class TiledMap final : public Resource, public micropather::Graph
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
            float opacity;
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
            int padding;
            OTextureRef pTexture;
            std::string name;
        };

        struct ObjectLayer : public Layer
        {
            virtual ~ObjectLayer();
            uint32_t objectCount = 0;
            Object *pObjects = nullptr;
        };

        static OTiledMapRef create(int width, int height, int tileSize);
        static OTiledMapRef createFromFile(const std::string &filename, const OContentManagerRef& pContentManager = nullptr);

        ~TiledMap();

        const Matrix &getTransform() const { return m_transform; }
        void setTransform(const Matrix &transform) { m_transform = transform; }

        int getWidth() const { return m_width; }
        int getHeight() const { return m_height; }
        int getTileSize() const { return m_tileSize; }

        void render();
        void renderLayer(int index);
        void renderLayer(const std::string &name);
        void renderLayer(Layer *pLayer);
        void render(const iRect &rect);
        void renderLayer(const iRect &rect, int index);
        void renderLayer(const iRect &rect, const std::string &name);
        void renderLayer(const iRect &rect, Layer *pLayer);

        TileSet* getTileSet(int index) const;
        TileSet* getTileSet(const std::string& name) const;
        TileSet* addTileSet(const OTextureRef& pTexture, const std::string& name, int padding = 0);

        int getLayerCount() const { return m_layerCount; }
        Layer* getLayer(int index) const { return m_layers[index]; }
        Layer* getLayer(const std::string &name) const;
        int getLayerIndex(const std::string &name) const;
        float* generateCollisions(const std::string &collisionLayerName);
        float* getCollisionTiles() const { return m_pCollisionTileCost; }
        Layer* addLayer(const std::string &name);
        void resetPath();
        void dirtyChunks(Layer* pLayer);

        using Path = std::vector<Point>;
        static const int PATH_ALLOW_DIAGONAL = 0x1;
        static const int PATH_CROSS_CORNERS = 0x2;

        struct PathWithCost
        {
            float cost;
            Path path;
        };
        Path getPath(const Point& from, const Point& to, int type = PATH_ALLOW_DIAGONAL | PATH_CROSS_CORNERS);
        void getPath(const Point& from, const Point& to, Path& path, int type = PATH_ALLOW_DIAGONAL | PATH_CROSS_CORNERS);
        PathWithCost getPathWithCost(const Point& from, const Point& to, int type = PATH_ALLOW_DIAGONAL | PATH_CROSS_CORNERS);
        void getPathWithCost(const Point& from, const Point& to, PathWithCost& path, int type = PATH_ALLOW_DIAGONAL | PATH_CROSS_CORNERS);

        const OTextureRef& getMinimap();

        void setFiltering(onut::sample::Filtering filtering);

        uint32_t getTileAt(TileLayer *pLayer, int x, int y) const;
        void setTileAt(TileLayer *pLayer, int x, int y, uint32_t tileId);

    private:
        struct Tile
        {
            TileSet *pTileset = nullptr;
            Rect rect;
            Vector4 UVs;
        };

        struct Chunk
        {
            OVertexBufferRef pVertexBuffer;
            OIndexBufferRef pIndexBuffer;
            TileSet *pTileset = nullptr;
            int tileCount = 0;
            bool isDirty = true;
            bool isSizeDirty = true;
            int x, y;
        };

        struct TileLayerInternal : public TileLayer
        {
            virtual ~TileLayerInternal();
            Tile *tiles = nullptr;
            Chunk *chunks = nullptr;
            int chunkPitch = 0;
            int chunkRows = 0;
        };

        void refreshChunk(Chunk* pChunk, TileLayerInternal* pLayer);
        float LeastCostEstimate(void* stateStart, void* stateEnd) override;
        void AdjacentCost(void* state, MP_VECTOR< micropather::StateCost > *adjacent) override;
        void PrintStateInfo(void* state) override;

        int m_width = 0;
        int m_height = 0;
        int m_tileSize = 1;
        int m_layerCount = 0;
        int m_tilesetCount = 0;
        Layer **m_layers = nullptr;
        TileSet *m_tileSets = nullptr;
        Matrix m_transform = Matrix::Identity;
        onut::sample::Filtering m_filtering = OFilterNearest;
        OTextureRef m_pMinimap;
        micropather::MicroPather *m_pMicroPather = nullptr;
        float* m_pCollisionTileCost = nullptr;
        int m_pathType = PATH_ALLOW_DIAGONAL | PATH_CROSS_CORNERS;
        MP_VECTOR<void*> m_cachedPath;
    };
};

OTiledMapRef OGetTiledMap(const std::string& name);

#endif
