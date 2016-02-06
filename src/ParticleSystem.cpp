#include "onut.h"
#include "ParticleSystem.h"
#include "rapidjson/document.h"
#include "rapidjson/filestream.h"
#include "tinyxml2.h"
#include "zlib/zlib.h"

#define PFX_READ_ENUM(__node__, __target__, __name__, __enumType__, ...) \
{ \
    std::unordered_map<std::string, __enumType__> enumMap = {__VA_ARGS__}; \
    const auto& json = __node__[#__name__]; \
    if (!json.IsNull()) \
    { \
        const auto& val = json.GetString(); \
        __target__.__name__ = enumMap[val]; \
    } \
}

namespace onut
{
    inline bool sPfxValueUtils<bool>::getNodeValue(const rapidjson::Value& node)
    {
        return node.GetBool();
    }

    inline float sPfxValueUtils<float>::getNodeValue(const rapidjson::Value& node)
    {
        return static_cast<float>(node.GetDouble());
    }

    inline double sPfxValueUtils<double>::getNodeValue(const rapidjson::Value& node)
    {
        return node.GetDouble();
    }

    inline int sPfxValueUtils<int>::getNodeValue(const rapidjson::Value& node)
    {
        return node.GetInt();
    }

    inline int64_t sPfxValueUtils<int64_t>::getNodeValue(const rapidjson::Value& node)
    {
        return node.GetInt64();
    }

    inline std::string sPfxValueUtils<std::string>::getNodeValue(const rapidjson::Value& node)
    {
        return node.GetString();
    }

    inline unsigned int sPfxValueUtils<unsigned int>::getNodeValue(const rapidjson::Value& node)
    {
        return node.GetUint();
    }

    inline uint64_t sPfxValueUtils<uint64_t>::getNodeValue(const rapidjson::Value& node)
    {
        return node.GetUint64();
    }

    inline Vector2 sPfxValueUtils<Vector2>::getNodeValue(const rapidjson::Value& node)
    {
        return{
            static_cast<float>(node[static_cast<decltype(node.Size())>(0)].GetDouble()),
            static_cast<float>(node[static_cast<decltype(node.Size())>(1)].GetDouble())
        };
    }

    inline Vector3 sPfxValueUtils<Vector3>::getNodeValue(const rapidjson::Value& node)
    {
        return{
            static_cast<float>(node[static_cast<decltype(node.Size())>(0)].GetDouble()),
            static_cast<float>(node[static_cast<decltype(node.Size())>(1)].GetDouble()),
            static_cast<float>(node[static_cast<decltype(node.Size())>(2)].GetDouble())
        };
    }

    inline Vector4 sPfxValueUtils<Vector4>::getNodeValue(const rapidjson::Value& node)
    {
        return{
            static_cast<float>(node[static_cast<decltype(node.Size())>(0)].GetDouble()),
            static_cast<float>(node[static_cast<decltype(node.Size())>(1)].GetDouble()),
            static_cast<float>(node[static_cast<decltype(node.Size())>(2)].GetDouble()),
            static_cast<float>(node[static_cast<decltype(node.Size())>(3)].GetDouble())
        };
    }

    inline Color sPfxValueUtils<Color>::getNodeValue(const rapidjson::Value& node)
    {
        return{
            static_cast<float>(node[static_cast<decltype(node.Size())>(0)].GetDouble()),
            static_cast<float>(node[static_cast<decltype(node.Size())>(1)].GetDouble()),
            static_cast<float>(node[static_cast<decltype(node.Size())>(2)].GetDouble()),
            static_cast<float>(node[static_cast<decltype(node.Size())>(3)].GetDouble())
        };
    }

    struct sPEX
    {
        enum class Type
        {
            Gravity,
            Radial
        };

        bool absolutePosition = false;
        float yCoordFlipped = -1;
        Vector2 sourcePosition;
        Vector2 sourcePositionVariance;
        float speed = 0;
        float speedVariance = 0;
        float particleLifeSpan = 1;
        float particleLifespanVariance = 0;
        float angle = 0;
        float angleVariance = 0;
        Vector2 gravity;
        float radialAcceleration = 0;
        float radialAccelVariance = 0;
        float tangentialAcceleration = 0;
        float tangentialAccelVariance = 0;
        Color startColor = Color::White;
        Color startColorVariance = Color(0, 0, 0, 0);
        Color finishColor = Color::White;
        Color finishColorVariance = Color(0, 0, 0, 0);
        unsigned int maxParticles = 100;
        float startParticleSize = 1;
        float startParticleSizeVariance = 0;
        float finishParticleSize = 1;
        float finishParticleSizeVariance = 0;
        float duration = 0;
        Type type = Type::Gravity;
        float maxRadius = 0;
        float maxRadiusVariance = 0;
        float minRadius = 0;
        float minRadiusVariance = 0;
        float rotatePerSecond = 0;
        float rotatePerSecondVariance = 0;
        unsigned int blendFuncSource = 1;
        unsigned int blendFuncDestination = 771;
        float rotationStart = 0;
        float rotationStartVariance = 0;
        float rotationEnd = 0;
        float rotationEndVariance = 0;
        std::string texture;
        bool hasTextureData = false;
    };

    sPEX loadPEXFile(const std::string& filename)
    {
        tinyxml2::XMLDocument doc;
        doc.LoadFile(filename.c_str());
        assert(!doc.Error());
        auto pXmlParticleEmitterConfig = doc.FirstChildElement("particleEmitterConfig");
        assert(pXmlParticleEmitterConfig);

        sPEX pex;
        std::string name;
        for (auto pXMLElement = pXmlParticleEmitterConfig->FirstChildElement(); pXMLElement; pXMLElement = pXMLElement->NextSiblingElement())
        {
            name = pXMLElement->Name();

            if (name == "absolutePosition")
            {
                pXMLElement->QueryAttribute("value", &pex.absolutePosition);
            }
            else if (name == "yCoordFlipped")
            {
                pXMLElement->QueryAttribute("value", &pex.yCoordFlipped);
            }
            else if (name == "sourcePosition")
            {
                pXMLElement->QueryAttribute("x", &pex.sourcePosition.x);
                pXMLElement->QueryAttribute("y", &pex.sourcePosition.y);
            }
            else if (name == "sourcePositionVariance")
            {
                pXMLElement->QueryAttribute("x", &pex.sourcePositionVariance.x);
                pXMLElement->QueryAttribute("y", &pex.sourcePositionVariance.y);
            }
            else if (name == "speed")
            {
                pXMLElement->QueryAttribute("value", &pex.speed);
            }
            else if (name == "speedVariance")
            {
                pXMLElement->QueryAttribute("value", &pex.speedVariance);
            }
            else if (name == "particleLifeSpan")
            {
                pXMLElement->QueryAttribute("value", &pex.particleLifeSpan);
            }
            else if (name == "particleLifespanVariance")
            {
                pXMLElement->QueryAttribute("value", &pex.particleLifespanVariance);
            }
            else if (name == "angle")
            {
                pXMLElement->QueryAttribute("value", &pex.angle);
            }
            else if (name == "angleVariance")
            {
                pXMLElement->QueryAttribute("value", &pex.angleVariance);
            }
            else if (name == "gravity")
            {
                pXMLElement->QueryAttribute("x", &pex.gravity.x);
                pXMLElement->QueryAttribute("y", &pex.gravity.y);
            }
            else if (name == "radialAcceleration")
            {
                pXMLElement->QueryAttribute("value", &pex.radialAcceleration);
            }
            else if (name == "radialAccelVariance")
            {
                pXMLElement->QueryAttribute("value", &pex.radialAccelVariance);
            }
            else if (name == "tangentialAcceleration")
            {
                pXMLElement->QueryAttribute("value", &pex.tangentialAcceleration);
            }
            else if (name == "tangentialAccelVariance")
            {
                pXMLElement->QueryAttribute("value", &pex.tangentialAccelVariance);
            }
            else if (name == "startColor")
            {
                pXMLElement->QueryAttribute("red", &pex.startColor.x);
                pXMLElement->QueryAttribute("green", &pex.startColor.y);
                pXMLElement->QueryAttribute("blue", &pex.startColor.z);
                pXMLElement->QueryAttribute("alpha", &pex.startColor.w);
            }
            else if (name == "startColorVariance")
            {
                pXMLElement->QueryAttribute("red", &pex.startColorVariance.x);
                pXMLElement->QueryAttribute("green", &pex.startColorVariance.y);
                pXMLElement->QueryAttribute("blue", &pex.startColorVariance.z);
                pXMLElement->QueryAttribute("alpha", &pex.startColorVariance.w);
            }
            else if (name == "finishColor")
            {
                pXMLElement->QueryAttribute("red", &pex.finishColor.x);
                pXMLElement->QueryAttribute("green", &pex.finishColor.y);
                pXMLElement->QueryAttribute("blue", &pex.finishColor.z);
                pXMLElement->QueryAttribute("alpha", &pex.finishColor.w);
            }
            else if (name == "finishColorVariance")
            {
                pXMLElement->QueryAttribute("red", &pex.finishColorVariance.x);
                pXMLElement->QueryAttribute("green", &pex.finishColorVariance.y);
                pXMLElement->QueryAttribute("blue", &pex.finishColorVariance.z);
                pXMLElement->QueryAttribute("alpha", &pex.finishColorVariance.w);
            }
            else if (name == "maxParticles")
            {
                pXMLElement->QueryAttribute("value", &pex.maxParticles);
            }
            else if (name == "startParticleSize")
            {
                pXMLElement->QueryAttribute("value", &pex.startParticleSize);
            }
            else if (name == "startParticleSizeVariance")
            {
                pXMLElement->QueryAttribute("value", &pex.startParticleSizeVariance);
            }
            else if (name == "finishParticleSize")
            {
                pXMLElement->QueryAttribute("value", &pex.finishParticleSize);
            }
            else if (name == "finishParticleSizeVariance")
            {
                pXMLElement->QueryAttribute("value", &pex.finishParticleSizeVariance);
            }
            else if (name == "duration")
            {
                pXMLElement->QueryAttribute("value", &pex.duration);
            }
            else if (name == "emitterType")
            {
                unsigned int type = 0;
                pXMLElement->QueryAttribute("value", &type);
                switch (type)
                {
                    case 0:
                        pex.type = sPEX::Type::Gravity;
                        break;
                    case 1:
                        pex.type = sPEX::Type::Radial;
                        break;
                }
            }
            else if (name == "maxRadius")
            {
                pXMLElement->QueryAttribute("value", &pex.maxRadius);
            }
            else if (name == "maxRadiusVariance")
            {
                pXMLElement->QueryAttribute("value", &pex.maxRadiusVariance);
            }
            else if (name == "minRadius")
            {
                pXMLElement->QueryAttribute("value", &pex.minRadius);
            }
            else if (name == "minRadiusVariance")
            {
                pXMLElement->QueryAttribute("value", &pex.minRadiusVariance);
            }
            else if (name == "rotatePerSecond")
            {
                pXMLElement->QueryAttribute("value", &pex.rotatePerSecond);
            }
            else if (name == "rotatePerSecondVariance")
            {
                pXMLElement->QueryAttribute("value", &pex.rotatePerSecondVariance);
            }
            else if (name == "blendFuncSource")
            {
                pXMLElement->QueryAttribute("value", &pex.blendFuncSource);
            }
            else if (name == "blendFuncDestination")
            {
                pXMLElement->QueryAttribute("value", &pex.blendFuncDestination);
            }
            else if (name == "rotationStart")
            {
                pXMLElement->QueryAttribute("value", &pex.rotationStart);
            }
            else if (name == "rotationStartVariance")
            {
                pXMLElement->QueryAttribute("value", &pex.rotationStartVariance);
            }
            else if (name == "rotationEnd")
            {
                pXMLElement->QueryAttribute("value", &pex.rotationEnd);
            }
            else if (name == "rotationEndVariance")
            {
                pXMLElement->QueryAttribute("value", &pex.rotationEndVariance);
            }
            else if (name == "texture")
            {
                auto pText = pXMLElement->Attribute("name");
                std::string data = pXMLElement->Attribute("data");
                if (data.empty())
                {
                    if (pText) pex.texture = pText;
                }
                else
                {
                    pex.hasTextureData = true;
                    pex.texture = std::move(data);
                }
            }
            else
            {
                OLogW("PEX " + name + " unsupported");
            }
        }

        return std::move(pex);
    }

    ParticleSystem* ParticleSystem::createFromFile(const std::string& filename, std::function<Texture*(const char*)> loadTextureFn)
    {
        auto extPos = filename.find_last_of('.');
        if (extPos == std::string::npos)
        {
            return nullptr;
        }
        auto extension = filename.substr(extPos + 1);
        if (toUpper(extension) == "PFX")
        {
            ParticleSystem* pRet = new ParticleSystem();

            FILE* pFile = nullptr;
            auto fopenRet = fopen_s(&pFile, filename.c_str(), "r");
            assert(!fopenRet);
            rapidjson::FileStream is(pFile);
            rapidjson::Document doc;
            doc.ParseStream<0>(is);

            pfxReadUint(pRet->desc.capacity, doc["capacity"]);

            const auto& jonsEmitters = doc["emitters"];
            pRet->emitters.resize(static_cast<decltype(pRet->emitters.size())>(jonsEmitters.Size()));
            for (decltype(jonsEmitters.Size()) i = 0; i < jonsEmitters.Size(); ++i)
            {
                const auto& jsonEmitter = jonsEmitters[i];
                auto& emitter = pRet->emitters[static_cast<decltype(pRet->emitters.size())>(i)];

                pfxReadEnum<eEmitterType>(emitter.type, jsonEmitter["type"], {
                    {"BURST", eEmitterType::BURST},
                    {"FINITE", eEmitterType::FINITE},
                    {"CONTINOUS", eEmitterType::CONTINOUS}});
                pfxReadUint(emitter.count, jsonEmitter["count"]);
                pfxReadFloat(emitter.rate, jsonEmitter["rate"]);
                emitter.spread = jsonEmitter["spread"];
                emitter.speed = jsonEmitter["speed"];
                emitter.color = jsonEmitter["color"];
                emitter.angle = jsonEmitter["angle"];
                emitter.size = jsonEmitter["size"];
                emitter.image_index = jsonEmitter["image_index"];
                emitter.life = jsonEmitter["life"];
                emitter.position = jsonEmitter["position"];
                const auto& images = jsonEmitter["images"];
                for (decltype(images.Size()) j = 0; j < images.Size(); ++j)
                {
                    const auto& jsonImage = images[j];
                    assert(jsonImage.IsString());
                    emitter.textures.push_back(loadTextureFn(jsonImage.GetString()));
                }
            }

            fclose(pFile);

            return pRet;
        }
        else if (toUpper(extension) == "PEX")
        {
            auto pex = loadPEXFile(filename);

            ParticleSystem* pRet = new ParticleSystem();
            pRet->emitters.resize(1);
            auto& emitter = pRet->emitters.front();

            if (pex.hasTextureData)
            {
                assert(false);
                //auto imageData = base64_decode(pex.texture);

                //int err;
                //z_stream d_stream; // decompression stream

                //d_stream.zalloc = (alloc_func)0;
                //d_stream.zfree = (free_func)0;
                //d_stream.opaque = (voidpf)0;

                //static uint8_t inflatedData[1024 * 1024 * 4]; // 4 meg

                //d_stream.next_in = reinterpret_cast<Bytef*>(imageData.data()); // where deflated is a pointer the the compressed data buffer
                //d_stream.avail_in = static_cast<uInt>(imageData.size()); // where deflatedLen is the length of the compressed data
                //d_stream.next_out = reinterpret_cast<Bytef*>(inflatedData); // where inflated is a pointer to the resulting uncompressed data buffer
                //d_stream.avail_out = static_cast<uInt>(1024 * 1024 * 4); // where inflatedLen is the size of the uncompressed data buffer

                //err = inflateInit2(&d_stream, 15 + 32);
                //assert(err == Z_OK);
                //err = inflate(&d_stream, Z_FINISH);
                //assert(err == Z_STREAM_END);
                //err = inflateEnd(&d_stream);
                //assert(err == Z_OK);

                //auto pTexture = OTexture::createFromFileData(inflatedData, 1024 * 1024 * 4);
                //emitter.textures.push_back(pTexture);
            }
            else
            {
                emitter.textures.push_back(loadTextureFn(pex.texture.c_str()));
            }
            emitter.spread = pex.angleVariance;
            emitter.speed.from = pex.speed - pex.speedVariance;
            emitter.speed.to = pex.speed + pex.speedVariance;

            emitter.dir = Vector3(
                cosf(DirectX::XMConvertToRadians(pex.angle)),
                -sinf(DirectX::XMConvertToRadians(pex.angle)),
                0);

            pex.startColor.Premultiply();
            emitter.color.value.from = pex.startColor - pex.startColorVariance;
            emitter.color.value.to = pex.startColor + pex.startColorVariance;
            emitter.color.finalValue.from = pex.finishColor - pex.finishColorVariance;
            emitter.color.finalValue.to = pex.finishColor + pex.finishColorVariance;
            emitter.color.finalSpecified = true;

            emitter.angle.value.from = pex.rotationStart - pex.rotationStartVariance;
            emitter.angle.value.to = pex.rotationStart + pex.rotationStartVariance;
            emitter.angle.finalValue.from = pex.rotationEnd - pex.rotationEndVariance;
            emitter.angle.finalValue.to = pex.rotationEnd + pex.rotationEndVariance;
            emitter.angle.finalSpecified = true;

            emitter.gravity = Vector3(pex.gravity, 0);

            auto textureSize = emitter.textures.front()->getSizef();
            emitter.size.value.from = std::max<>(0.f, pex.startParticleSize - pex.startParticleSizeVariance);
            emitter.size.value.to = std::max<>(0.f, pex.startParticleSize + pex.startParticleSizeVariance);
            emitter.size.finalValue.from = std::max<>(0.f, pex.finishParticleSize - pex.finishParticleSizeVariance);
            emitter.size.finalValue.to = std::max<>(0.f, pex.finishParticleSize + pex.finishParticleSizeVariance);
            emitter.size.finalSpecified = true;

            emitter.radialAccel.value.from = pex.radialAcceleration - pex.radialAccelVariance;
            emitter.radialAccel.value.to = pex.radialAcceleration + pex.radialAccelVariance;
            emitter.tangentAccel.value.from = pex.tangentialAcceleration - pex.tangentialAccelVariance;
            emitter.tangentAccel.value.to = pex.tangentialAcceleration + pex.tangentialAccelVariance;

            emitter.rotation.value.from = pex.rotatePerSecond - pex.rotatePerSecondVariance;
            emitter.rotation.value.to = pex.rotatePerSecond + pex.rotatePerSecondVariance;

            emitter.life.from = std::max<>(0.01f, pex.particleLifeSpan - pex.particleLifespanVariance);
            emitter.life.to = std::max<>(0.01f, pex.particleLifeSpan + pex.particleLifespanVariance);

            emitter.position.from = Vector3(pex.sourcePosition - pex.sourcePositionVariance, 0);
            emitter.position.to = Vector3(pex.sourcePosition + pex.sourcePositionVariance, 0);

            emitter.duration = pex.duration;

            switch (pex.type)
            {
                case sPEX::Type::Gravity:
                    emitter.accelType = sEmitterDesc::AccelType::Gravity;
                    break;
                case sPEX::Type::Radial:
                    assert(false); // Unsupported for now
                    emitter.accelType = sEmitterDesc::AccelType::Radial;
                    break;
            }

            if (pex.duration == -1)
            {
                emitter.rate = (static_cast<float>(pex.maxParticles) / pex.particleLifeSpan);
                emitter.type = eEmitterType::CONTINOUS;
            }
            else if (pex.duration == 0)
            {
                emitter.count = pex.maxParticles;
                emitter.type = eEmitterType::BURST;
            }
            else
            {
                emitter.rate = (static_cast<float>(pex.maxParticles) / pex.particleLifeSpan);
                emitter.type = eEmitterType::FINITE;
            }

            return pRet;
        }

        return nullptr;
    }

    ParticleSystem::ParticleSystem()
    {
    }

    ParticleSystem::~ParticleSystem()
    {
    }
}
