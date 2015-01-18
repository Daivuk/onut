#include "onut.h"
#include "ParticleSystem.h"
#include "rapidjson/document.h"
#include "rapidjson/filestream.h"

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

    ParticleSystem* ParticleSystem::createFromFile(const std::string& filename, std::function<Texture*(const char*)> loadTextureFn)
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
            pfxReadUint(emitter.rate, jsonEmitter["rate"]);
            emitter.spread = jsonEmitter["spread"];
            emitter.speed = jsonEmitter["speed"];
            emitter.color = jsonEmitter["color"];
            emitter.angle = jsonEmitter["angle"];
            emitter.size = jsonEmitter["size"];
            emitter.image_index = jsonEmitter["image_index"];
            emitter.life = jsonEmitter["life"];
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

    ParticleSystem::ParticleSystem()
    {
    }

    ParticleSystem::~ParticleSystem()
    {
    }
}
