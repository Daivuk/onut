#pragma once
#include "ContentManager.h"

#include <string>
#include <unordered_map>

extern onut::ContentManager* OContentManager;

namespace onut
{
    class Texture;

    class TiledMap
    {
    public:
        struct sLayer
        {
            virtual ~sLayer();
            std::string name;
            bool isVisible = true;
        };

        struct sTileLayer : public sLayer
        {
            virtual ~sTileLayer();
            int width;
            int height;
            uint32_t *tileIds = nullptr;
        };

        struct sObject
        {
            Vector2 position;
            Vector2 size;
            uint32_t id;
            std::string name;
            std::string type;
            std::unordered_map<std::string, std::string> properties;
        };

        struct sObjectLayer : public sLayer
        {
            virtual ~sObjectLayer();
            uint32_t objectCount = 0;
            sObject *pObjects = nullptr;
        };

        TiledMap(const std::string &map, onut::ContentManager *pContentManager = OContentManager);
        virtual ~TiledMap();

        const Matrix &getTransform() const { return m_transform; }
        void setTransform(const Matrix &transform) { m_transform = transform; }

        int getWidth() const { return m_width; }
        int getHeight() const { return m_height; }

        void render();
        void renderLayer(int index);
        void renderLayer(const std::string &name);
        void renderLayer(sLayer *pLayer);
        void render(const RECT &rect);
        void renderLayer(const RECT &rect, int index);
        void renderLayer(const RECT &rect, const std::string &name);
        void renderLayer(const RECT &rect, sLayer *pLayer);

        int getLayerCount() const { return m_layerCount; }
        sLayer *getLayer(int index) const { return m_layers[index]; }
        sLayer *getLayer(const std::string &name) const;

        onut::Texture *getMinimap();

    private:
        struct sTileSet
        {
            int firstId;
            int tileWidth;
            int tileHeight;
            onut::Texture *pTexture;
            std::string name;
        };

        struct sTile
        {
            sTileSet *pTileset = nullptr;
            Rect rect;
            Vector4 UVs;
        };

        struct sTileLayerInternal : public sTileLayer
        {
            virtual ~sTileLayerInternal();
            sTile *tiles = nullptr;
        };

        int m_width = 0;
        int m_height = 0;
        int m_layerCount = 0;
        int m_tilesetCount = 0;
        sLayer **m_layers = nullptr;
        sTileSet *m_tileSets = nullptr;
        Matrix m_transform = Matrix::Identity;
        onut::Texture *pMinimap = nullptr;
    };
};
