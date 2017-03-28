// Onut
#include <onut/ContentManager.h>
#include <onut/Crypto.h>
#include <onut/Files.h>
#include <onut/IndexBuffer.h>
#include <onut/Renderer.h>
#include <onut/SpriteBatch.h>
#include <onut/Strings.h>
#include <onut/Texture.h>
#include <onut/TiledMap.h>
#include <onut/VertexBuffer.h>

// Third party
#include <tinyxml2/tinyxml2.h>
#include <zlib/zlib.h>

// STL
#include <cassert>

#include <onut/Sound.h>

namespace onut
{
    static const int CHUNK_SIZE = 16;

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
        if (chunks) delete[] chunks;
    }

    OTiledMapRef TiledMap::create(int width, int height, int tileSize)
    {
        auto pRet = std::make_shared<OTiledMap>();

        pRet->m_width = width;
        pRet->m_height = height;
        pRet->m_tileSize = tileSize;
        auto len = width  * height;
        pRet->m_pCollisionTiles = new bool[len];
        for (int i = 0; i < len; ++i)
        {
            pRet->m_pCollisionTiles[i] = true;
        }

        return pRet;
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
            pRet->m_tileSize = pTileSet.tileWidth = pXMLTileset->IntAttribute("tilewidth");
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
                pLayer.opacity = 1.0f;
                pXMLLayer->QueryFloatAttribute("opacity", &pLayer.opacity);
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
                pLayer.chunkPitch = (pLayer.width + (CHUNK_SIZE - 1)) / CHUNK_SIZE;
                pLayer.chunkRows = (pLayer.height + (CHUNK_SIZE - 1)) / CHUNK_SIZE;
                pLayer.chunks = new Chunk[pLayer.chunkPitch * pLayer.chunkRows];
                for (int i = 0; i < len; ++i)
                {
                    auto pTile = pLayer.tiles + i;
                    auto tileId = pLayer.tileIds[i];
                    if (tileId == 0)
                    {
                        continue;
                    }
                    auto pTileSet = pRet->m_tileSets;
                    for (int j = 0; j < pRet->m_tilesetCount; ++j, ++pTileSet)
                    {
                        if (pTileSet->firstId > static_cast<int>(tileId)) break;
                    }
                    pTile->pTileset = pTileSet;
                    auto texSize = pTileSet->pTexture->getSize();
                    auto fitW = texSize.x / pTile->pTileset->tileWidth;
                    auto fitH = texSize.y / pTile->pTileset->tileHeight;
                    auto onTextureId = tileId - pTileSet->firstId;
                    pTile->UVs.x = static_cast<float>((onTextureId % fitW) * pTileSet->tileWidth) / static_cast<float>(texSize.x);
                    pTile->UVs.y = static_cast<float>((onTextureId / fitW) * pTileSet->tileHeight) / static_cast<float>(texSize.y);
                    pTile->UVs.z = static_cast<float>((onTextureId % fitW + 1) * pTileSet->tileWidth) / static_cast<float>(texSize.x);
                    pTile->UVs.w = static_cast<float>((onTextureId / fitW + 1) * pTileSet->tileHeight) / static_cast<float>(texSize.y);
                    pTile->rect.x = static_cast<float>((i % pLayer.width) * pTileSet->tileWidth);
                    pTile->rect.y = static_cast<float>((i / pLayer.width) * pTileSet->tileHeight);
                    pTile->rect.z = static_cast<float>(pTileSet->tileWidth);
                    pTile->rect.w = static_cast<float>(pTileSet->tileHeight);

                    auto x = (i % pLayer.width) / CHUNK_SIZE;
                    auto y = (i / pLayer.width) / CHUNK_SIZE;
                    auto pChunk = pLayer.chunks + (y * pLayer.chunkPitch + x);
                    pChunk->tileCount++;
                }
                
                len = pLayer.chunkPitch * pLayer.chunkRows;
                auto pChunk = pLayer.chunks;
                for (int i = 0; i < len; ++i, ++pChunk)
                {
                    pChunk->x = (i % pLayer.chunkPitch) * CHUNK_SIZE;
                    pChunk->y = (i / pLayer.chunkPitch) * CHUNK_SIZE;

                    if (!pChunk->tileCount) continue;

                    pRet->refreshChunk(pChunk, &pLayer);
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
        if (m_pCollisionTiles) delete[] m_pCollisionTiles;
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

    TiledMap::Layer* TiledMap::addLayer(const std::string &name)
    {
        Layer** ppNewLayers = new Layer*[m_layerCount + 1];
        memcpy(ppNewLayers, m_layers, sizeof(Layer*) * m_layerCount);
        delete[] m_layers;
        m_layers = ppNewLayers;
        m_layers[m_layerCount] = new TileLayerInternal();
        auto &pLayer = *(TileLayerInternal*)m_layers[m_layerCount];

        pLayer.name = name;
        pLayer.width = m_width;
        pLayer.height = m_height;
        auto len = pLayer.width * pLayer.height;
        pLayer.tileIds = new uint32_t[len];
        memset(pLayer.tileIds, 0, sizeof(uint32_t) * len);

        // Resolve the tiles to tilesets
        pLayer.tiles = new Tile[len];
        pLayer.chunkPitch = (pLayer.width + (CHUNK_SIZE - 1)) / CHUNK_SIZE;
        pLayer.chunkRows = (pLayer.height + (CHUNK_SIZE - 1)) / CHUNK_SIZE;
        pLayer.chunks = new Chunk[pLayer.chunkPitch * pLayer.chunkRows];

        len = pLayer.chunkPitch * pLayer.chunkRows;
        auto pChunk = pLayer.chunks;
        for (int i = 0; i < len; ++i, ++pChunk)
        {
            pChunk->x = (i % pLayer.chunkPitch) * CHUNK_SIZE;
            pChunk->y = (i / pLayer.chunkPitch) * CHUNK_SIZE;

            if (!pChunk->tileCount) continue;
        }

        ++m_layerCount;
        return m_layers[m_layerCount - 1];
    }

    int TiledMap::getLayerIndex(const std::string &name) const
    {
        for (int i = 0; i < m_layerCount; ++i)
        {
            if (m_layers[i]->name == name) return i;
        }
        return -1;
    }

    bool* TiledMap::generateCollisions(const std::string &collisionLayerName)
    {
        if (m_pCollisionTiles) return m_pCollisionTiles;
        auto pLayer = dynamic_cast<TileLayer*>(getLayer(collisionLayerName));
        if (pLayer)
        {
            int len = m_width * m_height;
            m_pCollisionTiles = new bool[len];
            for (int i = 0; i < len; ++i)
            {
                m_pCollisionTiles[i] = pLayer->tileIds[i] == 0;
            }
        }
        return m_pCollisionTiles;
    }

    TiledMap::TileSet* TiledMap::getTileSet(int index) const
    {
        if (index < 0 || index >= m_tilesetCount) return nullptr;
        return m_tileSets + index;
    }

    TiledMap::TileSet* TiledMap::getTileSet(const std::string& name) const
    {
        for (int i = 0; i < m_tilesetCount; ++i)
        {
            if (m_tileSets[i].name == name) return m_tileSets + i;
        }
        return nullptr;
    }

    TiledMap::TileSet* TiledMap::addTileSet(const OTextureRef& pTexture, const std::string& name)
    {
        TileSet* pNewTileSets = new TileSet[m_tilesetCount + 1];
        int idStart = 1;
        for (int i = 0; i < m_tilesetCount; ++i)
        {
            pNewTileSets[i] = m_tileSets[i];
            idStart = m_tileSets[i].firstId + ((m_tileSets[i].pTexture->getSize().x / m_tileSets[i].tileWidth) * (m_tileSets[i].pTexture->getSize().y / m_tileSets[i].tileHeight));
        }
        delete[] m_tileSets;
        m_tileSets = pNewTileSets;
        m_tileSets[m_tilesetCount].firstId = idStart;
        m_tileSets[m_tilesetCount].tileWidth = m_tileSize;
        m_tileSets[m_tilesetCount].tileHeight = m_tileSize;
        m_tileSets[m_tilesetCount].pTexture = pTexture;
        m_tileSets[m_tilesetCount].name = name;
        ++m_tilesetCount;
        return m_tileSets + (m_tilesetCount - 1);
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
        Matrix transform = getTransform();
        if (oSpriteBatch->isInBatch()) transform = transform * oSpriteBatch->getTransform();
        render(getScreenRECTFromTransform(transform, {m_tileSets->tileWidth, m_tileSets->tileHeight}));
    }

    void TiledMap::renderLayer(int index)
    {
        if (!m_tileSets) return;
        Matrix transform = getTransform();
        if (oSpriteBatch->isInBatch()) transform = transform * oSpriteBatch->getTransform();
        renderLayer(getScreenRECTFromTransform(transform, {m_tileSets->tileWidth, m_tileSets->tileHeight}), index);
    }

    void TiledMap::renderLayer(const std::string &name)
    {
        if (!m_tileSets) return;
        Matrix transform = getTransform();
        if (oSpriteBatch->isInBatch()) transform = transform * oSpriteBatch->getTransform();
        renderLayer(getScreenRECTFromTransform(transform, {m_tileSets->tileWidth, m_tileSets->tileHeight}), name);
    }

    void TiledMap::renderLayer(Layer *pLayer)
    {
        if (!m_tileSets) return;
        Matrix transform = getTransform();
        if (oSpriteBatch->isInBatch()) transform = transform * oSpriteBatch->getTransform();
        renderLayer(getScreenRECTFromTransform(transform, {m_tileSets->tileWidth, m_tileSets->tileHeight}), pLayer);
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

    void TiledMap::refreshChunk(Chunk* pChunk, TileLayerInternal* pLayer)
    {
        if (pChunk->isSizeDirty)
        {
            // Indices
            pChunk->pIndexBuffer = OIndexBuffer::createDynamic(pChunk->tileCount * sizeof(uint16_t) * 6);
            auto pIndices = (uint16_t*)pChunk->pIndexBuffer->map();
            for (int j = 0; j < pChunk->tileCount; ++j)
            {
                pIndices[j * 6 + 0] = j * 4 + 0;
                pIndices[j * 6 + 1] = j * 4 + 1;
                pIndices[j * 6 + 2] = j * 4 + 2;
                pIndices[j * 6 + 3] = j * 4 + 0;
                pIndices[j * 6 + 4] = j * 4 + 2;
                pIndices[j * 6 + 5] = j * 4 + 3;
            }
            pChunk->pIndexBuffer->unmap(pChunk->tileCount * sizeof(uint16_t) * 6);
        }

        // Vertices
        if (pChunk->isSizeDirty)
        {
            pChunk->pVertexBuffer = OVertexBuffer::createDynamic(pChunk->tileCount * sizeof(OSpriteBatch::SVertexP2T2C4) * 4);
        }
        auto pVertices = (OSpriteBatch::SVertexP2T2C4*)pChunk->pVertexBuffer->map();
        int j = 0;
        int layerW = pLayer->width;
        int layerH = pLayer->height;
        auto layerTiles = pLayer->tiles;
        auto color = Color::White * pLayer->opacity;
        for (int y = pChunk->y; y < pChunk->y + CHUNK_SIZE && y < layerH; ++y)
        {
            for (int x = pChunk->x; x < pChunk->x + CHUNK_SIZE && x < layerW; ++x)
            {
                auto& tile = layerTiles[y * pLayer->width + x];
                if (!tile.pTileset) continue;

                pChunk->pTileset = tile.pTileset;

                auto& vert0 = pVertices[j * 4 + 0];
                auto& vert1 = pVertices[j * 4 + 1];
                auto& vert2 = pVertices[j * 4 + 2];
                auto& vert3 = pVertices[j * 4 + 3];

                vert0.color = color;
                vert1.color = color;
                vert2.color = color;
                vert3.color = color;

                vert0.texCoord.x = tile.UVs.x;
                vert0.texCoord.y = tile.UVs.y;
                vert1.texCoord.x = tile.UVs.x;
                vert1.texCoord.y = tile.UVs.w;
                vert2.texCoord.x = tile.UVs.z;
                vert2.texCoord.y = tile.UVs.w;
                vert3.texCoord.x = tile.UVs.z;
                vert3.texCoord.y = tile.UVs.y;

                vert0.position = tile.rect.TopLeft();
                vert1.position = tile.rect.BottomLeft();
                vert2.position = tile.rect.BottomRight();
                vert3.position = tile.rect.TopRight();

                ++j;
            }
        }
        pChunk->pVertexBuffer->unmap(pChunk->tileCount * sizeof(OSpriteBatch::SVertexP2T2C4) * 4);

        pChunk->isDirty = false;
        pChunk->isSizeDirty = false;
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
        
        rect.left /= CHUNK_SIZE;
        rect.top /= CHUNK_SIZE;
        rect.right /= CHUNK_SIZE;
        rect.bottom /= CHUNK_SIZE;

        bool isInBatch = oSpriteBatch->isInBatch();
        if (isInBatch)
        {
            oSpriteBatch->end();
            oRenderer->setupFor2D(oSpriteBatch->getTransform() * getTransform());
        }
        else
        {
            oRenderer->setupFor2D(getTransform());
        }
        oRenderer->renderStates.sampleFiltering = m_filtering;
        for (int y = rect.top; y <= rect.bottom; ++y)
        {
            for (int x = rect.left; x <= rect.right; ++x)
            {
                auto pChunk = pLayer->chunks + (y * pLayer->chunkPitch + x);
                if (!pChunk->tileCount) continue;
                if (pChunk->isDirty) refreshChunk(pChunk, pLayer);
                oRenderer->renderStates.textures[0] = pChunk->pTileset->pTexture;
                oRenderer->renderStates.vertexBuffer = pChunk->pVertexBuffer;
                oRenderer->renderStates.indexBuffer = pChunk->pIndexBuffer;
                oRenderer->drawIndexed(pChunk->tileCount * 6);
            }
        }
        if (isInBatch)
        {
            oSpriteBatch->begin(oSpriteBatch->getTransform());
        }
    }

    const OTextureRef& TiledMap::getMinimap()
    {
        m_pMinimap = OTexture::createRenderTarget(Point(m_width, m_height));
        return m_pMinimap;
    }

    uint32_t TiledMap::getTileAt(TileLayer *pLayer, int x, int y) const
    {
        if (x < 0 || y < 0 || x >= pLayer->width || y >= pLayer->height) return 0;
        return pLayer->tileIds[y * pLayer->width + x];
    }

    void TiledMap::setTileAt(TileLayer *pLayer, int x, int y, uint32_t tileId)
    {
        if (x < 0 || y < 0 || x >= pLayer->width || y >= pLayer->height) return;
        auto i = (y * pLayer->width + x);
        if (pLayer->tileIds[i] == tileId) return;

        auto pInternalLayer = (TileLayerInternal*)pLayer;

        auto chunkX = (i % pInternalLayer->width) / CHUNK_SIZE;
        auto chunkY = (i / pInternalLayer->width) / CHUNK_SIZE;
        auto pChunk = pInternalLayer->chunks + (chunkY * pInternalLayer->chunkPitch + chunkX);
        pChunk->isDirty = true;

        if (pLayer->tileIds[i] == 0)
        {
            pChunk->tileCount++;
            pChunk->isSizeDirty = true;
        }

        pLayer->tileIds[i] = tileId;
        auto pTile = pInternalLayer->tiles + i;
        if (tileId == 0)
        {
            pTile->pTileset = nullptr;
            pChunk->tileCount--;
            pChunk->isSizeDirty = true;
            return;
        }
        auto pTileSet = m_tileSets;
        for (int j = 0; j < m_tilesetCount; ++j, ++pTileSet)
        {
            if (pTileSet->firstId > static_cast<int>(tileId)) break;
        }
        pTile->pTileset = pTileSet;
        auto texSize = pTileSet->pTexture->getSize();
        auto fitW = texSize.x / pTile->pTileset->tileWidth;
        auto fitH = texSize.y / pTile->pTileset->tileHeight;
        auto onTextureId = tileId - pTileSet->firstId;
        pTile->UVs.x = static_cast<float>((onTextureId % fitW) * pTileSet->tileWidth) / static_cast<float>(texSize.x);
        pTile->UVs.y = static_cast<float>((onTextureId / fitW) * pTileSet->tileHeight) / static_cast<float>(texSize.y);
        pTile->UVs.z = static_cast<float>((onTextureId % fitW + 1) * pTileSet->tileWidth) / static_cast<float>(texSize.x);
        pTile->UVs.w = static_cast<float>((onTextureId / fitW + 1) * pTileSet->tileHeight) / static_cast<float>(texSize.y);
        pTile->rect.x = static_cast<float>((i % pLayer->width) * pTileSet->tileWidth);
        pTile->rect.y = static_cast<float>((i / pLayer->height) * pTileSet->tileHeight);
        pTile->rect.z = static_cast<float>(pTileSet->tileWidth);
        pTile->rect.w = static_cast<float>(pTileSet->tileHeight);
    }

    void TiledMap::setFiltering(onut::sample::Filtering filtering)
    {
        m_filtering = filtering;
    }
};

OTiledMapRef OGetTiledMap(const std::string& name)
{
    return oContentManager->getResourceAs<OTiledMap>(name);
}
