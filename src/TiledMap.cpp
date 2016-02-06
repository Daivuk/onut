#include <cassert>
#include "tinyxml2.h"
#include "onut.h"
#include "crypto.h"
#include "zlib/zlib.h"

namespace onut
{
    TiledMap::sLayer::~sLayer()
    {
    }

    TiledMap::sTileLayer::~sTileLayer()
    {
        if (tileIds) delete[] tileIds;
    }

    TiledMap::sObjectLayer::~sObjectLayer()
    {
        if (pObjects) delete[] pObjects;
    }

    TiledMap::sTileLayerInternal::~sTileLayerInternal()
    {
        if (tiles) delete[] tiles;
    }

    TiledMap::TiledMap(const std::string &map, onut::ContentManager *pContentManager)
    {
        tinyxml2::XMLDocument doc;
        doc.LoadFile(map.c_str());
        assert(!doc.Error());
        auto pXMLMap = doc.FirstChildElement("map");
        assert(pXMLMap);

        // Tilesets
        for (auto pXMLTileset = pXMLMap->FirstChildElement("tileset"); pXMLTileset; pXMLTileset = pXMLTileset->NextSiblingElement("tileset"))
        {
            ++m_tilesetCount;
        }
        assert(m_tilesetCount);
        m_tileSets = new sTileSet[m_tilesetCount];
        m_tilesetCount = 0;
        for (auto pXMLTileset = pXMLMap->FirstChildElement("tileset"); pXMLTileset; pXMLTileset = pXMLTileset->NextSiblingElement("tileset"))
        {
            auto &pTileSet = m_tileSets[m_tilesetCount];

            pTileSet.name = pXMLTileset->Attribute("name");
            pTileSet.firstId = pXMLTileset->IntAttribute("firstgid");
            assert(pTileSet.firstId);
            pTileSet.tileWidth = pXMLTileset->IntAttribute("tilewidth");
            assert(pTileSet.tileWidth);
            pTileSet.tileHeight = pXMLTileset->IntAttribute("tileheight");
            assert(pTileSet.tileHeight);

            auto pXMLImage = pXMLTileset->FirstChildElement("image");
            assert(pXMLImage);
            auto szImageFilename = pXMLImage->Attribute("source");
            assert(szImageFilename);
            auto filename = getPath(map) + "/" + szImageFilename;
            pTileSet.pTexture = pContentManager->getResource<Texture>(filename);
            if (!pTileSet.pTexture)
            {
                filename = getFilename(szImageFilename);
                pTileSet.pTexture = pContentManager->getResource<Texture>(filename);
            }

            ++m_tilesetCount;
        }

        // Layers
        for (auto pXMLLayer = pXMLMap->FirstChildElement(); pXMLLayer; pXMLLayer = pXMLLayer->NextSiblingElement())
        {
            if (!strcmp(pXMLLayer->Name(), "layer") ||
                !strcmp(pXMLLayer->Name(), "objectgroup"))
            {
                ++m_layerCount;
            }
        }
        assert(m_layerCount);
        m_layers = new sLayer*[m_layerCount];
        m_layerCount = 0;
        for (auto pXMLLayer = pXMLMap->FirstChildElement(); pXMLLayer; pXMLLayer = pXMLLayer->NextSiblingElement())
        {
            if (!strcmp(pXMLLayer->Name(), "layer"))
            {
                m_layers[m_layerCount] = new sTileLayerInternal();
                auto &pLayer = *(sTileLayerInternal*)m_layers[m_layerCount];

                pLayer.name = pXMLLayer->Attribute("name");
                if (pXMLLayer->Attribute("visible"))
                {
                    pLayer.isVisible = pXMLLayer->BoolAttribute("visible");
                }

                pLayer.width = pXMLLayer->IntAttribute("width");
                assert(pLayer.width);
                pLayer.height = pXMLLayer->IntAttribute("height");
                assert(pLayer.height);
                m_width = std::max<>(m_width, pLayer.width);
                m_height = std::max<>(m_height, pLayer.height);
                auto len = pLayer.width * pLayer.height;
                pLayer.tileIds = new uint32_t[len];

                auto pXMLData = pXMLLayer->FirstChildElement("data");
                assert(pXMLData);
                auto szEncoding = pXMLData->Attribute("encoding");
                auto szCompression = pXMLData->Attribute("compression");

                if (!szEncoding)
                {
                    int i = 0;
                    for (auto pXMLTile = pXMLData->FirstChildElement("tile"); pXMLTile; pXMLTile = pXMLTile->NextSiblingElement("tile"), ++i)
                    {
                        auto id = pXMLTile->IntAttribute("gid");
                        assert(i < len);
                        pLayer.tileIds[i] = id;
                    }
                    assert(i == len);
                }
                else
                {
                    auto szData = pXMLData->GetText();
                    assert(szData);
                    auto pos = std::string(szData).find_first_not_of("\n ");
                    assert(pos < strlen(szData));
                    szData = szData + pos;

                    if (!strcmp(szEncoding, "csv"))
                    {
                        auto csvData = splitString(szData, ',');
                        assert(static_cast<int>(csvData.size()) == len);
                        for (int i = 0; i < len; ++i)
                        {
                            try
                            {
                                pLayer.tileIds[i] = static_cast<uint32_t>(std::stoul(csvData[i]));
                            }
                            catch (std::exception e)
                            {
                                assert(false);
                            }
                        }
                    }
                    else if (!strcmp(szEncoding, "base64"))
                    {
                        auto decoded = base64_decode(szData);
                        if (!szCompression)
                        {
                            assert(static_cast<int>(decoded.size()) == len * 4);
                            memcpy(pLayer.tileIds, decoded.data(), 4 * len);
                        }
                        else if (!strcmp(szCompression, "gzip"))
                        {
                            int err;
                            z_stream d_stream; // decompression stream

                            d_stream.zalloc = (alloc_func)0;
                            d_stream.zfree = (free_func)0;
                            d_stream.opaque = (voidpf)0;

                            d_stream.next_in = reinterpret_cast<Bytef*>(decoded.data()); // where deflated is a pointer the the compressed data buffer
                            d_stream.avail_in = static_cast<uInt>(decoded.size()); // where deflatedLen is the length of the compressed data
                            d_stream.next_out = reinterpret_cast<Bytef*>(pLayer.tileIds); // where inflated is a pointer to the resulting uncompressed data buffer
                            d_stream.avail_out = static_cast<uInt>(len * 4); // where inflatedLen is the size of the uncompressed data buffer

                            err = inflateInit2(&d_stream, 31);
                            assert(err == Z_OK);
                            err = inflate(&d_stream, Z_FINISH);
                            assert(err == Z_STREAM_END);
                            err = inflateEnd(&d_stream);
                            assert(err == Z_OK);
                        }
                        else if (!strcmp(szCompression, "zlib"))
                        {
                            int err;
                            z_stream d_stream; // decompression stream

                            d_stream.zalloc = (alloc_func)0;
                            d_stream.zfree = (free_func)0;
                            d_stream.opaque = (voidpf)0;

                            d_stream.next_in = reinterpret_cast<Bytef*>(decoded.data()); // where deflated is a pointer the the compressed data buffer
                            d_stream.avail_in = static_cast<uInt>(decoded.size()); // where deflatedLen is the length of the compressed data
                            d_stream.next_out = reinterpret_cast<Bytef*>(pLayer.tileIds); // where inflated is a pointer to the resulting uncompressed data buffer
                            d_stream.avail_out = static_cast<uInt>(len * 4); // where inflatedLen is the size of the uncompressed data buffer

                            err = inflateInit2(&d_stream, 15 + 32);
                            assert(err == Z_OK);
                            err = inflate(&d_stream, Z_FINISH);
                            assert(err == Z_STREAM_END);
                            err = inflateEnd(&d_stream);
                            assert(err == Z_OK);
                        }
                    }
                }

                // Resolve the tiles to tilesets
                pLayer.tiles = new sTile[len];
                for (int i = 0; i < len; ++i)
                {
                    auto pTile = pLayer.tiles + i;
                    auto tileId = pLayer.tileIds[i];
                    if (tileId == 0)
                    {
                        continue;
                    }
                    auto pTileSet = m_tileSets;
                    for (int j = 0; j < m_tilesetCount; ++j, pTileSet)
                    {
                        if (pTileSet->firstId > static_cast<int>(tileId)) break;
                    }
                    pTile->pTileset = pTileSet;
                    auto texSize = pTileSet->pTexture->getSize();
                    auto fitW = texSize.x / pTile->pTileset->tileWidth;
                    auto fitH = texSize.y / pTile->pTileset->tileHeight;
                    auto onTextureId = tileId - pTileSet->firstId;
                    pTile->UVs.x = static_cast<float>((onTextureId % fitW) * pTileSet->tileWidth) / static_cast<float>(texSize.x);
                    pTile->UVs.y = static_cast<float>((onTextureId / fitH) * pTileSet->tileHeight) / static_cast<float>(texSize.y);
                    pTile->UVs.z = static_cast<float>((onTextureId % fitW + 1) * pTileSet->tileWidth) / static_cast<float>(texSize.x);
                    pTile->UVs.w = static_cast<float>((onTextureId / fitH + 1) * pTileSet->tileHeight) / static_cast<float>(texSize.y);
                    pTile->rect.x = static_cast<float>((i % pLayer.width) * pTileSet->tileWidth);
                    pTile->rect.y = static_cast<float>((i / pLayer.height) * pTileSet->tileHeight);
                    pTile->rect.z = static_cast<float>(pTileSet->tileWidth);
                    pTile->rect.w = static_cast<float>(pTileSet->tileHeight);
                }

                ++m_layerCount;
            }
            else if (!strcmp(pXMLLayer->Name(), "objectgroup"))
            {
                m_layers[m_layerCount] = new sObjectLayer();
                auto &pLayer = *(sObjectLayer*)m_layers[m_layerCount];

                pLayer.name = pXMLLayer->Attribute("name");
                if (pXMLLayer->Attribute("visible"))
                {
                    pLayer.isVisible = pXMLLayer->BoolAttribute("visible");
                }

                for (auto pXMLObject = pXMLLayer->FirstChildElement("object"); pXMLObject; pXMLObject = pXMLObject->NextSiblingElement("object"))
                {
                    pLayer.objectCount++;
                }
                pLayer.pObjects = new sObject[pLayer.objectCount];
                pLayer.objectCount = 0;
                for (auto pXMLObject = pXMLLayer->FirstChildElement("object"); pXMLObject; pXMLObject = pXMLObject->NextSiblingElement("object"))
                {
                    auto &object = pLayer.pObjects[pLayer.objectCount];
                    pLayer.objectCount++;
                    
                    object.id = pXMLObject->IntAttribute("id");
                    if (pXMLObject->Attribute("name")) object.name = pXMLObject->Attribute("name");
                    if (pXMLObject->Attribute("type")) object.type = pXMLObject->Attribute("type");
                    object.position.x = pXMLObject->FloatAttribute("x");
                    object.position.y = pXMLObject->FloatAttribute("y");
                    object.size.x = pXMLObject->FloatAttribute("width");
                    object.size.y = pXMLObject->FloatAttribute("height");

                    auto pXMLProperties = pXMLObject->FirstChildElement("properties");
                    if (pXMLProperties)
                    {
                        for (auto pXMLProperty = pXMLProperties->FirstChildElement("property"); pXMLProperty; pXMLProperty = pXMLProperty->NextSiblingElement("property"))
                        {
                            if (pXMLProperty->Attribute("name") &&
                                pXMLProperty->Attribute("value"))
                            {
                                object.properties[pXMLProperty->Attribute("name")] = pXMLProperty->Attribute("value");
                            }
                        }
                    }
                }

                ++m_layerCount;
            }
        }

        //TODO: Compile the graphics by batch of 16x16 terrain chunks
    }

    TiledMap::~TiledMap()
    {
        if (m_layers)
        {
            for (auto i = 0; i < m_layerCount; ++i)
            {
                if (m_layers[i]) delete m_layers[i];
            }
            delete[] m_layers;
        }
        if (m_tileSets) delete[] m_tileSets;
    }

    TiledMap::sLayer *TiledMap::getLayer(const std::string &name) const
    {
        for (int i = 0; i < m_layerCount; ++i)
        {
            if (m_layers[i]->name == name) return m_layers[i];
        }
        return nullptr;
    }

    static RECT getScreenRECTFromTransform(const Matrix& transform, const POINT& tileSize)
    {
        auto invTransform = transform.Invert();
        Vector2 localScreenPos[4] =
        {
            Vector2::Transform(Vector2::Zero, invTransform),
            Vector2::Transform(Vector2(0.f, OScreenHf), invTransform),
            Vector2::Transform(OScreenf, invTransform),
            Vector2::Transform(Vector2(OScreenWf, 0.f), invTransform)
        };
        Vector2 boundingBox[2] = 
        {
            Vector2(onut::min(localScreenPos[0].x, localScreenPos[1].x, localScreenPos[2].x, localScreenPos[3].x),
                    onut::min(localScreenPos[0].y, localScreenPos[1].y, localScreenPos[2].y, localScreenPos[3].y)),
            Vector2(onut::max(localScreenPos[0].x, localScreenPos[1].x, localScreenPos[2].x, localScreenPos[3].x),
                    onut::max(localScreenPos[0].y, localScreenPos[1].y, localScreenPos[2].y, localScreenPos[3].y))
        };
        return RECT{
            static_cast<LONG>(boundingBox[0].x) / tileSize.x,
            static_cast<LONG>(boundingBox[0].y) / tileSize.y,
            static_cast<LONG>(boundingBox[1].x) / tileSize.x,
            static_cast<LONG>(boundingBox[1].y) / tileSize.y};
    }

    void TiledMap::render()
    {
        if (!m_tileSets) return;
        render(getScreenRECTFromTransform(getTransform(), {m_tileSets->tileWidth, m_tileSets->tileHeight}));
    }

    void TiledMap::renderLayer(int index)
    {
        if (!m_tileSets) return;
        renderLayer(getScreenRECTFromTransform(getTransform(), {m_tileSets->tileWidth, m_tileSets->tileHeight}), index);
    }

    void TiledMap::renderLayer(const std::string &name)
    {
        if (!m_tileSets) return;
        renderLayer(getScreenRECTFromTransform(getTransform(), {m_tileSets->tileWidth, m_tileSets->tileHeight}), name);
    }

    void TiledMap::renderLayer(sLayer *pLayer)
    {
        if (!m_tileSets) return;
        renderLayer(getScreenRECTFromTransform(getTransform(), {m_tileSets->tileWidth, m_tileSets->tileHeight}), pLayer);
    }

    void TiledMap::render(const RECT &rect)
    {
        for (int i = 0; i < m_layerCount; ++i)
        {
            renderLayer(rect, m_layers[i]);
        }
    }

    void TiledMap::renderLayer(const RECT &rect, int index)
    {
        renderLayer(rect, m_layers[index]);
    }

    void TiledMap::renderLayer(const RECT &rect, const std::string &name)
    {
        renderLayer(rect, getLayer(name));
    }

    void TiledMap::renderLayer(const RECT &in_rect, sLayer *in_pLayer)
    {
        if (!in_pLayer->isVisible) return;

        auto pLayer = dynamic_cast<sTileLayerInternal*>(in_pLayer);
        if (!pLayer) return;

        RECT rect = in_rect;
        rect.left = std::max<>(0l, rect.left);
        rect.top = std::max<>(0l, rect.top);
        rect.right = std::min<>(static_cast<LONG>(m_width - 1), rect.right);
        rect.bottom = std::min<>(static_cast<LONG>(m_height - 1), rect.bottom);

        bool manageSB = !OSB->isInBatch();
        if (manageSB) OSB->begin(getTransform());
        for (LONG y = rect.top; y <= rect.bottom; ++y)
        {
            sTile *pTile = pLayer->tiles + y * m_width + rect.left;
            for (LONG x = rect.left; x <= rect.right; ++x, ++pTile)
            {
                if (!pTile->pTileset) continue;
                OSB->drawRectWithUVs(pTile->pTileset->pTexture, pTile->rect, pTile->UVs);
            }
        }
        if (manageSB) OSB->end();
    }

    onut::Texture *TiledMap::getMinimap()
    {
//        //if (pMinimap) return pMinimap;
//        if (!m_tilesetCount) return nullptr;
//
//        pMinimap = OTexture::createRenderTarget({m_width, m_height});
//        ORenderer->bindRenderTarget(pMinimap);
//
//        auto tileWidth = m_tileSets[0].tileWidth;
//
//        setTransform(Matrix::CreateScale(1.f / static_cast<float>(tileWidth)));
//        egStatePush();
//        egFilter(EG_FILTER_TRILINEAR);
//        auto h = 32768 / m_width / 4;
//        for (auto y = 0; y < m_height; y += h)
//        {
//            render({0, y, m_width, std::min<>(y + h, m_height)});
//        }
//        egPostProcess();
//        egStatePop();
//
//        ORenderer->bindRenderTarget(nullptr);
//        return pMinimap;
        return nullptr;
    }
};
