#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include "Anim.h"
#include "Random.h"
#include "rapidjson/document.h"
#include "SimpleMath.h"
using namespace DirectX::SimpleMath;

namespace onut
{
    class IParticleSystemManager;
    class Texture;

    template<typename Ttype>
    void pfxReadUint(Ttype& out, const rapidjson::Value& node)
    {
        if (!node.IsNull())
        {
            out = node.GetUint();
        }
    }

    template<typename Ttype>
    void pfxReadFloat(Ttype& out, const rapidjson::Value& node)
    {
        if (!node.IsNull())
        {
            out = static_cast<float>(node.GetDouble());
        }
    }

    template<typename TenumType>
    void pfxReadEnum(TenumType& out, const rapidjson::Value& node, const std::unordered_map<std::string, TenumType>& enumMap)
    {
        if (!node.IsNull())
        {
            out = enumMap.find(node.GetString())->second;
        }
    }

    enum class eEmitterType
    {
        BURST,
        FINITE,
        CONTINOUS
    };

    enum class ePfxFinalValueType
    {
        MULT,
        ADD,
        NORMAL
    };

    template<typename Ttype>
    struct sPfxValueUtils
    {
        static Ttype getNodeValue(const rapidjson::Value& node);
    };

    template<typename Ttype>
    struct sPfxRange
    {
        Ttype from;
        Ttype to;

        sPfxRange(const Ttype& other) :
            from(other),
            to(other)
        {
        }

        sPfxRange<Ttype>& operator=(const Ttype& other)
        {
            from = to = other;
            return *this;
        }

        sPfxRange<Ttype>& operator=(const rapidjson::Value& node)
        {
            if (!node.IsNull())
            {
                if (node.IsObject())
                {
                    if (node.HasMember("range"))
                    {
                        const auto& range = node["range"];
                        from = sPfxValueUtils<Ttype>::getNodeValue(range[static_cast<decltype(range.Size())>(0)]);
                        to = sPfxValueUtils<Ttype>::getNodeValue(range[static_cast<decltype(range.Size())>(1)]);
                    }
                    else if (node.HasMember("value"))
                    {
                        to = from = sPfxValueUtils<Ttype>::getNodeValue(node["value"]);
                    }
                }
                else
                {
                    to = from = sPfxValueUtils<Ttype>::getNodeValue(node);
                }
            }
            return *this;
        }

        Ttype generate() const
        {
            return randt<Ttype>(from, to);
        }
    };

    template<typename Ttype>
    struct sPfxValue
    {
        sPfxRange<Ttype>    value;
        sPfxRange<Ttype>    finalValue;
        bool                finalSpecified = false;
        TweenType           tween = TweenType::LINEAR;
        ePfxFinalValueType  finalValueType = ePfxFinalValueType::NORMAL;

        sPfxValue(const Ttype& other) :
            value(other),
            finalValue(other)
        {
        }

        sPfxValue<Ttype>& operator=(const Ttype& other)
        {
            finalValue = value = other;
            return *this;
        }

        Ttype generateFrom() const
        {
            return value.generate();
        }

        Ttype generateTo() const
        {
            return finalValue.generate();
        }

        Ttype generateTo(const Ttype& from) const
        {
            if (finalSpecified)
            {
                switch (finalValueType)
                {
                    case ePfxFinalValueType::MULT:
                        return from * generateTo();
                    case ePfxFinalValueType::ADD:
                        return from + generateTo();
                    case ePfxFinalValueType::NORMAL:
                        return generateTo();
                }
                return Ttype();
            }
            else
            {
                return from;
            }
        }

        sPfxValue<Ttype>& operator=(const rapidjson::Value& node)
        {
            if (!node.IsNull())
            {
                value = node;
                finalValue = value;
                if (node.IsObject())
                {
                    if (node.HasMember("finalValue"))
                    {
                        finalSpecified = true;
                        const auto& finalNode = node["finalValue"];
                        finalValue = finalNode;
                        if (finalNode.IsObject())
                        {
                            pfxReadEnum<TweenType>(tween, finalNode["tween"], {
                                {"NONE", TweenType::NONE},
                                {"LINEAR", TweenType::LINEAR},
                                {"EASE_IN", TweenType::EASE_IN},
                                {"EASE_OUT", TweenType::EASE_OUT},
                                {"EASE_BOTH", TweenType::EASE_BOTH},
                                {"BOUNCE_IN", TweenType::BOUNCE_IN},
                                {"BOUNCE_OUT", TweenType::BOUNCE_OUT},
                                {"SPRING_IN", TweenType::SPRING_IN},
                                {"SPRING_OUT", TweenType::SPRING_OUT}
                            });
                            pfxReadEnum<ePfxFinalValueType>(finalValueType, finalNode["type"], {
                                {"MULT", ePfxFinalValueType::MULT},
                                {"ADD", ePfxFinalValueType::ADD},
                                {"NORMAL", ePfxFinalValueType::NORMAL}
                            });
                        }
                    }
                }
            }
            return *this;
        }
    };

    struct sEmitterDesc
    {
        enum AccelType
        {
            Gravity,
            Radial
        };

        eEmitterType type = eEmitterType::FINITE;
        union
        {
            unsigned int count = 1;
            float rate;
        };
        std::vector<Texture*>   textures;
        sPfxValue<float>        spread = 0;
        Vector3                 dir = Vector3(0, 0, 0);
        sPfxRange<float>        speed = 0;
        sPfxValue<Color>        color = Color::White;
        sPfxValue<float>        angle = 0;
        sPfxValue<float>        size = 1;
        sPfxValue<unsigned int> image_index = 0;
        sPfxRange<float>        life = 1;
        sPfxRange<Vector3>      position = Vector3(0, 0, 0);
        Vector3                 gravity = Vector3(0, 0, 0);
        sPfxRange<float>        duration = 0;
        sPfxValue<float>        rotation = 0;
        sPfxValue<float>        radialAccel = 0;
        sPfxValue<float>        tangentAccel = 0;
        AccelType               accelType = AccelType::Gravity;
    };

    struct sParticleSystemDesc
    {
        unsigned int capacity = 100;
    };

    class ParticleSystem
    {
    public:
        template<typename TcontentManagerType>
        static ParticleSystem* createFromFile(const std::string& filename, TcontentManagerType* pContentManager)
        {
            return ParticleSystem::createFromFile(filename, [pContentManager](const char* pFilename)
            {
                return pContentManager->getResource<Texture>(pFilename);
            });
        }
        static ParticleSystem* createFromFile(const std::string& filename, std::function<Texture*(const char*)> loadTextureFn);

        ParticleSystem();
        virtual ~ParticleSystem();

    public:
        sParticleSystemDesc         desc;
        std::vector<sEmitterDesc>   emitters;
    };
}

typedef onut::ParticleSystem OPfx;
