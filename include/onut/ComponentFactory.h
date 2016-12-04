#ifndef COMPONENTFACTORY_H_INCLUDED
#define COMPONENTFACTORY_H_INCLUDED


// Onut includes
#include <onut/CSV.h>
#include <onut/Font.h>
#include <onut/Maths.h>
#include <onut/Music.h>
#include <onut/Sound.h>
#include <onut/SpriteAnim.h>
#include <onut/Strings.h>
#include <onut/Texture.h>
#include <onut/TiledMap.h>

// STL
#include <functional>
#include <sstream>
#include <string>
#include <unordered_map>

// Forward declaration
#include <onut/ForwardDeclaration.h>
OForwardDeclare(Component);
OForwardDeclare(ComponentFactory);
OForwardDeclare(Entity);

namespace onut
{
    class ComponentFactory final
    {
    public:
        static OComponentFactoryRef create();

        ~ComponentFactory();

        template<typename Tcomponent>
        void registerComponent(const std::string& name)
        {
            m_factoryMap[name] = OMake<Factory<Tcomponent>>();
        }

        OComponentRef instantiate(const std::string& name) const;

        template<typename Tcomponent, typename Ttype, typename Tgetter, typename Tsetter>
        void bindProperty(const std::string& componentName, const std::string& propertyName, Tgetter getter, Tsetter setter)
        {
            auto pProperty = OMake<Property<Tcomponent, Ttype, Tgetter, Tsetter>>();
            pProperty->getter = getter;
            pProperty->setter = setter;
            pProperty->pComponentFactory = this;
            m_propertyComponentMap[componentName].propertyMap[propertyName] = pProperty;
        }

        void setProperty(const OComponentRef& pComponent, const std::string& componentName, const std::string& propertyName, const std::string& propertyValue);

        void registerDefaultComponents();

        void registerEntity(uint32_t id, const OEntityRef& pEntity);
        OEntityRef getEntity(uint32_t id);
        void clearEntityRegistry();

    private:
        using EntityMap = std::unordered_map<uint32_t, OEntityWeak>;

        class IFactory
        {
        public:
            virtual OComponentRef instantiate() = 0;
        };

        template<typename Tcomponent>
        class Factory final : public IFactory
        {
        public:
            OComponentRef instantiate() override
            {
                return OMake<Tcomponent>();
            }
        };

        class IProperty
        {
        public:
            virtual void set(Component* pCaller, const std::string& valueStr) = 0;
        };

        template<typename Ttype>
        Ttype stringToValue(const std::string& str);

        template<typename Tcomponent, typename Ttype, typename Tgetter, typename Tsetter>
        class Property final : public IProperty
        {
        public:
            Tgetter getter;
            Tsetter setter;
            ComponentFactory* pComponentFactory;

            void set(Component* pCaller, const std::string& valueStr)
            {
                Ttype value = pComponentFactory->stringToValue<Ttype>(valueStr);
                auto pRealCaller = dynamic_cast<Tcomponent*>(pCaller);
                auto fn = std::bind(setter, pRealCaller, std::placeholders::_1);
                fn(value);
            }
        };

        using FactoryMap = std::unordered_map < std::string, std::shared_ptr<IFactory> > ;
        struct PropertyMap
        {
            std::unordered_map<std::string, std::shared_ptr<IProperty>> propertyMap;
        };
        using PropertyComponentMap = std::unordered_map < std::string, PropertyMap > ;

        ComponentFactory();

        FactoryMap m_factoryMap;
        PropertyComponentMap m_propertyComponentMap;
        EntityMap m_entityMap;
    };

    template<>
    inline bool ComponentFactory::stringToValue<bool>(const std::string& str)
    {
        return str == "true";
    }

    template<>
    inline std::string ComponentFactory::stringToValue<std::string>(const std::string& str)
    {
        return str;
    }

    template<>
    inline int ComponentFactory::stringToValue<int>(const std::string& str)
    {
        try
        {
            return std::stoi(str);
        }
        catch (...)
        {
            return 0;
        }
    }

    template<>
    inline float ComponentFactory::stringToValue<float>(const std::string& str)
    {
        try
        {
            return std::stof(str);
        }
        catch (...)
        {
            return 0.0f;
        }
    }

    template<>
    inline Vector2 ComponentFactory::stringToValue<Vector2>(const std::string& str)
    {
        try
        {
            auto split = onut::splitString(str, ',');
            return Vector2(std::stof(split[0]), std::stof(split[1]));
        }
        catch (...)
        {
            return Vector2::Zero;
        }
    }

    template<>
    inline Vector3 ComponentFactory::stringToValue<Vector3>(const std::string& str)
    {
        try
        {
            auto split = onut::splitString(str, ',');
            return Vector3(std::stof(split[0]), std::stof(split[1]), std::stof(split[2]));
        }
        catch (...)
        {
            return Vector3::Zero;
        }
    }

    template<>
    inline Vector4 ComponentFactory::stringToValue<Vector4>(const std::string& str)
    {
        try
        {
            auto split = onut::splitString(str, ',');
            return Vector4(std::stof(split[0]), std::stof(split[1]), std::stof(split[2]), std::stof(split[3]));
        }
        catch (...)
        {
            return Vector4::Zero;
        }
    }

    template<>
    inline Color ComponentFactory::stringToValue<Color>(const std::string& str)
    {
        try
        {
            auto split = onut::splitString(str, ',');
            if (split.size() == 3)
            {
                return OColorRGB(std::stoi(split[0]), std::stoi(split[1]), std::stoi(split[2]));
            }
            else if (split.size() == 4)
            {
                return OColorRGBA(std::stoi(split[0]), std::stoi(split[1]), std::stoi(split[2]), std::stoi(split[3]));
            }
            else
            {
                return Color::Black;
            }
        }
        catch (...)
        {
            return Color::Black;
        }
    }

    template<>
    inline OEntityRef ComponentFactory::stringToValue<OEntityRef>(const std::string& str)
    {
        uint32_t entityId = 0;
        try
        {
            entityId = (uint32_t)std::stoi(str);
            return m_entityMap[entityId].lock();
        }
        catch (...)
        {
            return nullptr;
        }
    }

    #define DECL_RES_STR_TO_VAL(__res__) \
    template<> \
    inline O ## __res__ ## Ref ComponentFactory::stringToValue<O ## __res__ ## Ref>(const std::string& str) \
    { \
        return OGet ## __res__(str); \
    }
    DECL_RES_STR_TO_VAL(CSV);
    DECL_RES_STR_TO_VAL(Font);
    DECL_RES_STR_TO_VAL(Music);
    DECL_RES_STR_TO_VAL(Sound);
    DECL_RES_STR_TO_VAL(Texture);
    DECL_RES_STR_TO_VAL(TiledMap);
    DECL_RES_STR_TO_VAL(SpriteAnim);
};

extern OComponentFactoryRef oComponentFactory;

#define ORegisterComponent(__class__) oComponentFactory->registerComponent<__class__>(#__class__)

// Properties
#define OBindProperty(__type__, __class__, __prop__) oComponentFactory->bindProperty<__class__, __type__>(#__class__, #__prop__, &__class__::get ## __prop__, &__class__::set ## __prop__)

#define OBindBoolProperty(__class__, __prop__) OBindProperty(bool, __class__, __prop__)
#define OBindIntProperty(__class__, __prop__) OBindProperty(int, __class__, __prop__)
#define OBindFloatProperty(__class__, __prop__) OBindProperty(float, __class__, __prop__)
#define OBindVector2Property(__class__, __prop__) OBindProperty(Vector2, __class__, __prop__)
#define OBindVector3Property(__class__, __prop__) OBindProperty(Vector3, __class__, __prop__)
#define OBindVector4Property(__class__, __prop__) OBindProperty(Vector4, __class__, __prop__)
#define OBindColorProperty(__class__, __prop__) OBindProperty(Color, __class__, __prop__)
#define OBindStringProperty(__class__, __prop__) OBindProperty(std::string, __class__, __prop__)
#define OBindEntityProperty(__class__, __prop__) OBindProperty(OEntityRef, __class__, __prop__)

// Resources
#define OBindCSVProperty(__class__, __prop__) OBindProperty(OCSVRef, __class__, __prop__)
#define OBindFontProperty(__class__, __prop__) OBindProperty(OFontRef, __class__, __prop__)
#define OBindMusicProperty(__class__, __prop__) OBindProperty(OMusicRef, __class__, __prop__)
#define OBindSoundProperty(__class__, __prop__) OBindProperty(OSoundRef, __class__, __prop__)
#define OBindTextureProperty(__class__, __prop__) OBindProperty(OTextureRef, __class__, __prop__)
#define OBindTiledMapProperty(__class__, __prop__) OBindProperty(OTiledMapRef, __class__, __prop__)
#define OBindSpriteAnimProperty(__class__, __prop__) OBindProperty(OSpriteAnimRef, __class__, __prop__)

#endif
