// Onut
#include <onut/ContentManager.h>
#include <onut/Crypto.h>
#include <onut/Files.h>
#include <onut/Renderer.h>
#include <onut/SpriteBatch.h>
#include <onut/Strings.h>
#include <onut/Texture.h>
#include <onut/TiledMap.h>

// Third party
#include <tinyxml2/tinyxml2.h>
#include <zlib/zlib.h>

// STL
#include <cassert>

namespace onut
{
    TiledMap::Layer::~Layer()
    {
    }

    TiledMap::TileLayer::~TileLayer()
    {
        if (tileIds) delete[] tileIds;
    }

    TiledMap::ObjectLayer::~ObjectLayer()
    {
        if (pObjects) delete[] pObjects;
    }

    TiledMap::TileLayerInternal::~TileLayerInternal()
    {
        if (tiles) delete[] tiles;
    }

    OTiledMapRef TiledMap::createFromFile(const std::string &filename, const OContentManagerRef& in_pContentManager)
    {
        OContentManagerRef pContentManager = in_pContentManager;
        if (!pContentManager) pContentManager = oContentManager;

        auto pRet = std::make_shared<OTiledMap>();

        tinyxml2::XMLDocument doc;
        doc.LoadFile(filename.c_str());
        assert(!doc.Error());
        auto pXMLMap = doc.FirstChildElement("map");
        assert(pXMLMap);

        // Tilesets
        for (auto pXMLTileset = pXMLMap->FirstChildElement("tileset"); pXMLTileset; pXMLTileset = pXMLTileset->NextSiblingElement("tileset"))
        {
            pRet->m_tilesetCount++;
        }
        assert(pRet->m_tilesetCount);
        pRet->m_tileSets = new TileSet[pRet->m_tilesetCount];
        pRet->m_tilesetCount = 0;
        for (auto pXMLTileset = pXMLMap->FirstChildElement("tileset"); pXMLTileset; pXMLTileset = pXMLTileset->NextSiblingElement("tileset"))
        {
            auto &pTileSet = pRet->m_tileSets[pRet->m_tilesetCount];

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
            pTileSet.pTexture = pContentManager->getResourceAs<OTexture>(onut::getFilename(szImageFilename));

            pRet->m_tilesetCount++;
        }

        // Layers
        for (auto pXMLLayer = pXMLMap->FirstChildElement(); pXMLLayer; pXMLLayer = pXMLLayer->NextSiblingElement())
        {
            if (!strcmp(pXMLLayer->Name(), "layer") ||
                !strcmp(pXMLLayer->Name(), "objectgroup"))
            {
                pRet->m_layerCount++;
            }
        }
        assert(pRet->m_layerCount);
        pRet->m_layers = new Layer*[pRet->m_layerCount];
        pRet->m_layerCount = 0;
        for (auto pXMLLayer = pXMLMap->FirstChildElement(); pXMLLayer; pXMLLayer = pXMLLayer->NextSiblingElement())
        {
            if (!strcmp(pXMLLayer->Name(), "layer"))
            {
                pRet->m_layers[pRet->m_layerCount] = new TileLayerInternal();
                auto &pLayer = *(TileLayerInternal*)pRet->m_layers[pRet->m_layerCount];

                pLayer.name = pXMLLayer->Attribute("name");
                if (pXMLLayer->Attribute("visible"))
                {
                    pLayer.isVisible = pXMLLayer->BoolAttribute("visible");
                }

                pLayer.width = pXMLLayer->IntAttribute("width");
                assert(pLayer.width);
                pLayer.height = pXMLLayer->IntAttribute("height");
                assert(pLayer.height);
                pRet->m_width = std::max<>(pRet->m_width, pLayer.width);
                pRet->m_height = std::max<>(pRet->m_height, pLayer.height);
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
                        auto decoded = onut::crypto::base64_decode(szData);
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
                pLayer.tiles = new Tile[len];
                for (int i = 0; i < len; ++i)
                {
                    auto pTile = pLayer.tiles + i;
                    auto tileId = pLayer.tileIds[i];
                    if (tileId == 0)
                    {
                        continue;
                    }
                    auto pTileSet = pRet->m_tileSets;
                    for (int j = 0; j < pRet->m_tilesetCount; ++j, pTileSet)
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

                pRet->m_layerCount++;
            }
            else if (!strcmp(pXMLLayer->Name(), "objectgroup"))
            {
                pRet->m_layers[pRet->m_layerCount] = new ObjectLayer();
                auto &pLayer = *(ObjectLayer*)pRet->m_layers[pRet->m_layerCount];

                pLayer.name = pXMLLayer->Attribute("name");
                if (pXMLLayer->Attribute("visible"))
                {
                    pLayer.isVisible = pXMLLayer->BoolAttribute("visible");
                }

                for (auto pXMLObject = pXMLLayer->FirstChildElement("object"); pXMLObject; pXMLObject = pXMLObject->NextSiblingElement("object"))
                {
                    pLayer.objectCount++;
                }
                pLayer.pObjects = new Object[pLayer.objectCount];
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

                pRet->m_layerCount++;
            }
        }

        //TODO: Compile the graphics by batch of 16x16 terrain chunks
        return pRet;
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

    TiledMap::Layer *TiledMap::getLayer(const std::string &name) const
    {
        for (int i = 0; i < m_layerCount; ++i)
        {
            if (m_layers[i]->name == name) return m_layers[i];
        }
        return nullptr;
    }

    TiledMap::TileSet* TiledMap::getTileSet(int index) const
    {
        if (index < 0 || index >= m_tilesetCount) return nullptr;
        return m_tileSets + index;
    }

    static iRect getScreenRECTFromTransform(const Matrix& transform, const Point& tileSize)
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
        return iRect{
            static_cast<int>(boundingBox[0].x) / tileSize.x,
            static_cast<int>(boundingBox[0].y) / tileSize.y,
            static_cast<int>(boundingBox[1].x) / tileSize.x,
            static_cast<int>(boundingBox[1].y) / tileSize.y};
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

    void TiledMap::renderLayer(Layer *pLayer)
    {
        if (!m_tileSets) return;
        renderLayer(getScreenRECTFromTransform(getTransform(), {m_tileSets->tileWidth, m_tileSets->tileHeight}), pLayer);
    }

    void TiledMap::render(const iRect &rect)
    {
        for (int i = 0; i < m_layerCount; ++i)
        {
            renderLayer(rect, m_layers[i]);
        }
    }

    void TiledMap::renderLayer(const iRect &rect, int index)
    {
        renderLayer(rect, m_layers[index]);
    }

    void TiledMap::renderLayer(const iRect &rect, const std::string &name)
    {
        renderLayer(rect, getLayer(name));
    }

    void TiledMap::renderLayer(const iRect &in_rect, Layer *in_pLayer)
    {
        if (!in_pLayer->isVisible) return;

        auto pLayer = dynamic_cast<TileLayerInternal*>(in_pLayer);
        if (!pLayer) return;

        iRect rect = in_rect;
        rect.left = std::max<>(0, rect.left);
        rect.top = std::max<>(0, rect.top);
        rect.right = std::min<>((m_width - 1), rect.right);
        rect.bottom = std::min<>((m_height - 1), rect.bottom);

        bool manageSB = !oSpriteBatch->isInBatch();
        if (manageSB) oSpriteBatch->begin(getTransform());
        for (int y = rect.top; y <= rect.bottom; ++y)
        {
            Tile *pTile = pLayer->tiles + y * m_width + rect.left;
            for (int x = rect.left; x <= rect.right; ++x, ++pTile)
            {
                if (!pTile->pTileset) continue;
                oSpriteBatch->drawRectWithUVs(pTile->pTileset->pTexture, pTile->rect, pTile->UVs);
            }
        }
        if (manageSB) oSpriteBatch->end();
    }

    const OTextureRef& TiledMap::getMinimap()
    {
//        //if (pMinimap) return pMinimap;
//        if (!m_tilesetCount) return nullptr;
//
//        pMinimap = OTexture::createRenderTarget({m_width, m_height});
//        oRenderer->bindRenderTarget(pMinimap);
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
//        oRenderer->bindRenderTarget(nullptr);
//        return pMinimap;
        return m_pMinimap;
    }
};

OTiledMapRef OGetTiledMap(const std::string& name)
{
    return oContentManager->getResourceAs<OTiledMap>(name);
}
