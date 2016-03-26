#pragma once
// Onut
#include <onut/Tween.h>
#include <onut/Maths.h>
#include <onut/Resource.h>
#include <onut/Random.h>

// Third party
#include <rapidjson/document.h>

// STL
#include <string>
#include <unordered_map>
#include <vector>

// Forward
#include <onut/ForwardDeclaration.h>
OForwardDeclare(ContentManager);
OForwardDeclare(ParticleEmitterDesc);
OForwardDeclare(ParticleSystem);
OForwardDeclare(Texture);

namespace onut
{
    enum class PfxFinalValueType
    {
        MULT,
        ADD,
        NORMAL
    };

    template<typename Ttype>
    static void pfxReadUint(Ttype& out, const rapidjson::Value& node)
    {
        if (!node.IsNull())
        {
            out = node.GetUint();
        }
    }

    template<typename Ttype>
    static void pfxReadFloat(Ttype& out, const rapidjson::Value& node)
    {
        if (!node.IsNull())
        {
            out = static_cast<float>(node.GetDouble());
        }
    }

    template<typename TenumType>
    static void pfxReadEnum(TenumType& out, const rapidjson::Value& node, const std::unordered_map<std::string, TenumType>& enumMap)
    {
        if (!node.IsNull())
        {
            out = enumMap.find(node.GetString())->second;
        }
    }

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
        sPfxRange<Ttype> value;
        sPfxRange<Ttype> finalValue;
        bool finalSpecified = false;
        Tween tween = Tween::Linear;
        PfxFinalValueType finalValueType = PfxFinalValueType::NORMAL;

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
                    case PfxFinalValueType::MULT:
                        return from * generateTo();
                    case PfxFinalValueType::ADD:
                        return from + generateTo();
                    case PfxFinalValueType::NORMAL:
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
                            pfxReadEnum<Tween>(tween, finalNode["tween"], {
                                {"NONE", Tween::None},
                                {"LINEAR", Tween::Linear},
                                {"EASE_IN", Tween::EaseIn},
                                {"EASE_OUT", Tween::EaseOut},
                                {"EASE_BOTH", Tween::EaseBoth},
                                {"BOUNCE_IN", Tween::BounceIn},
                                {"BOUNCE_OUT", Tween::BounceOut},
                                {"SPRING_IN", Tween::SpringIn},
                                {"SPRING_OUT", Tween::SpringOut}
                            });
                            pfxReadEnum<PfxFinalValueType>(finalValueType, finalNode["type"], {
                                {"MULT", PfxFinalValueType::MULT},
                                {"ADD", PfxFinalValueType::ADD},
                                {"NORMAL", PfxFinalValueType::NORMAL}
                            });
                        }
                    }
                }
            }
            return *this;
        }
    };

    class ParticleEmitterDesc final
    {
    public:
        enum class Type
        {
            BURST,
            FINITE,
            CONTINOUS
        };

        enum class AccelType
        {
            Gravity,
            Radial
        };

        using Textures = std::vector<OTextureRef>;

        Type type = Type::FINITE;
        unsigned int count = 1;
        float rate;
        unsigned int capacity = 100;
        Textures textures;
        sPfxValue<float> spread = 0;
        Vector3 dir = Vector3(0, 0, 0);
        sPfxRange<float> speed = 0;
        sPfxValue<Color> color = Color::White;
        sPfxValue<float> angle = 0;
        sPfxValue<float> size = 1;
        sPfxValue<unsigned int> image_index = 0;
        sPfxRange<float> life = 1;
        sPfxRange<Vector3> position = Vector3(0, 0, 0);
        Vector3 gravity = Vector3(0, 0, 0);
        sPfxRange<float> duration = 0;
        sPfxValue<float> rotation = 0;
        sPfxValue<float> radialAccel = 0;
        sPfxValue<float> tangentAccel = 0;
        AccelType accelType = AccelType::Gravity;
    };

    class ParticleSystem final : public Resource
    {
    public:
        static OParticleSystemRef createFromFile(const std::string& filename, const OContentManagerRef &pContentManager = nullptr);

        using Emitters = std::vector<OParticleEmitterDescRef>;

        const Emitters& getEmitters() const;

    private:
        Emitters m_emitters;
    };
}

OParticleSystemRef OGetParticleSystem(const std::string& name);
