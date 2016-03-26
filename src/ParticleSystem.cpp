// Onut
#include <onut/ContentManager.h>
#include <onut/Log.h>
#include <onut/ParticleSystem.h>
#include <onut/Strings.h>
#include <onut/Texture.h>

// Third party
#include <rapidjson/filestream.h>
#include <tinyxml2/tinyxml2.h>
#include <zlib/zlib.h>

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

    OParticleSystemRef ParticleSystem::createFromFile(const std::string& filename, const OContentManagerRef &in_pContentManager)
    {
        OContentManagerRef pContentManager = in_pContentManager;
        if (!pContentManager) pContentManager = oContentManager;

        auto extPos = filename.find_last_of('.');
        if (extPos == std::string::npos)
        {
            return nullptr;
        }
        auto extension = filename.substr(extPos + 1);
        if (toUpper(extension) == "PFX")
        {
            auto pRet = std::make_shared<OParticleSystem>();

            FILE* pFile = nullptr;
            auto fopenRet = fopen_s(&pFile, filename.c_str(), "r");
            assert(!fopenRet);
            rapidjson::FileStream is(pFile);
            rapidjson::Document doc;
            doc.ParseStream<0>(is);

            const auto& jonsEmitters = doc["emitters"];
            for (decltype(jonsEmitters.Size()) i = 0; i < jonsEmitters.Size(); ++i)
            {
                const auto& jsonEmitter = jonsEmitters[i];
                OParticleEmitterDescRef pEmitter = std::make_shared<OParticleEmitterDesc>();
                pRet->m_emitters.push_back(pEmitter);

                pfxReadEnum<ParticleEmitterDesc::Type>(pEmitter->type, jsonEmitter["type"], {
                    {"BURST", ParticleEmitterDesc::Type::BURST},
                    {"FINITE", ParticleEmitterDesc::Type::FINITE},
                    {"CONTINOUS", ParticleEmitterDesc::Type::CONTINOUS}});
                pfxReadUint(pEmitter->count, jsonEmitter["count"]);
                pfxReadFloat(pEmitter->rate, jsonEmitter["rate"]);
                pEmitter->spread = jsonEmitter["spread"];
                pEmitter->speed = jsonEmitter["speed"];
                pEmitter->color = jsonEmitter["color"];
                pEmitter->angle = jsonEmitter["angle"];
                pEmitter->size = jsonEmitter["size"];
                pEmitter->image_index = jsonEmitter["image_index"];
                pEmitter->life = jsonEmitter["life"];
                pEmitter->position = jsonEmitter["position"];
                const auto& images = jsonEmitter["images"];
                for (decltype(images.Size()) j = 0; j < images.Size(); ++j)
                {
                    const auto& jsonImage = images[j];
                    assert(jsonImage.IsString());
                    pEmitter->textures.push_back(pContentManager->getResourceAs<OTexture>(jsonImage.GetString()));
                }
            }

            fclose(pFile);

            return pRet;
        }
        else if (toUpper(extension) == "PEX")
        {
            auto pex = loadPEXFile(filename);

            auto pRet = std::make_shared<OParticleSystem>();
            OParticleEmitterDescRef pEmitter = std::make_shared<OParticleEmitterDesc>();
            pRet->m_emitters.push_back(pEmitter);

            if (pex.hasTextureData)
            {
                assert(false); // We don't support their embedded texture format.
            }
            else
            {
                pEmitter->textures.push_back(pContentManager->getResourceAs<OTexture>(pex.texture.c_str()));
            }
            pEmitter->spread = pex.angleVariance;
            pEmitter->speed.from = pex.speed - pex.speedVariance;
            pEmitter->speed.to = pex.speed + pex.speedVariance;

            pEmitter->dir = Vector3(
                cosf(DirectX::XMConvertToRadians(pex.angle)),
                -sinf(DirectX::XMConvertToRadians(pex.angle)),
                0);

            pex.startColor.Premultiply();
            pEmitter->color.value.from = pex.startColor - pex.startColorVariance;
            pEmitter->color.value.to = pex.startColor + pex.startColorVariance;
            pEmitter->color.finalValue.from = pex.finishColor - pex.finishColorVariance;
            pEmitter->color.finalValue.to = pex.finishColor + pex.finishColorVariance;
            pEmitter->color.finalSpecified = true;

            pEmitter->angle.value.from = pex.rotationStart - pex.rotationStartVariance;
            pEmitter->angle.value.to = pex.rotationStart + pex.rotationStartVariance;
            pEmitter->angle.finalValue.from = pex.rotationEnd - pex.rotationEndVariance;
            pEmitter->angle.finalValue.to = pex.rotationEnd + pex.rotationEndVariance;
            pEmitter->angle.finalSpecified = true;

            pEmitter->gravity = Vector3(pex.gravity, 0);

            auto textureSize = pEmitter->textures.front()->getSizef();
            pEmitter->size.value.from = std::max<>(0.f, pex.startParticleSize - pex.startParticleSizeVariance);
            pEmitter->size.value.to = std::max<>(0.f, pex.startParticleSize + pex.startParticleSizeVariance);
            pEmitter->size.finalValue.from = std::max<>(0.f, pex.finishParticleSize - pex.finishParticleSizeVariance);
            pEmitter->size.finalValue.to = std::max<>(0.f, pex.finishParticleSize + pex.finishParticleSizeVariance);
            pEmitter->size.finalSpecified = true;

            pEmitter->radialAccel.value.from = pex.radialAcceleration - pex.radialAccelVariance;
            pEmitter->radialAccel.value.to = pex.radialAcceleration + pex.radialAccelVariance;
            pEmitter->tangentAccel.value.from = pex.tangentialAcceleration - pex.tangentialAccelVariance;
            pEmitter->tangentAccel.value.to = pex.tangentialAcceleration + pex.tangentialAccelVariance;

            pEmitter->rotation.value.from = pex.rotatePerSecond - pex.rotatePerSecondVariance;
            pEmitter->rotation.value.to = pex.rotatePerSecond + pex.rotatePerSecondVariance;

            pEmitter->life.from = std::max<>(0.01f, pex.particleLifeSpan - pex.particleLifespanVariance);
            pEmitter->life.to = std::max<>(0.01f, pex.particleLifeSpan + pex.particleLifespanVariance);

            pEmitter->position.from = Vector3(pex.sourcePosition - pex.sourcePositionVariance, 0);
            pEmitter->position.to = Vector3(pex.sourcePosition + pex.sourcePositionVariance, 0);

            pEmitter->duration = pex.duration;

            switch (pex.type)
            {
                case sPEX::Type::Gravity:
                    pEmitter->accelType = ParticleEmitterDesc::AccelType::Gravity;
                    break;
                case sPEX::Type::Radial:
                    assert(false); // Unsupported for now
                    pEmitter->accelType = ParticleEmitterDesc::AccelType::Radial;
                    break;
            }

            if (pex.duration == -1)
            {
                pEmitter->rate = (static_cast<float>(pex.maxParticles) / pex.particleLifeSpan);
                pEmitter->type = ParticleEmitterDesc::Type::CONTINOUS;
            }
            else if (pex.duration == 0)
            {
                pEmitter->count = pex.maxParticles;
                pEmitter->type = ParticleEmitterDesc::Type::BURST;
            }
            else
            {
                pEmitter->rate = (static_cast<float>(pex.maxParticles) / pex.particleLifeSpan);
                pEmitter->type = ParticleEmitterDesc::Type::FINITE;
            }

            return pRet;
        }

        return nullptr;
    }

    const ParticleSystem::Emitters& ParticleSystem::getEmitters() const
    {
        return m_emitters;
    }
}

OParticleSystemRef OGetParticleSystem(const std::string& name)
{
    return oContentManager->getResourceAs<OParticleSystem>(name);
}
