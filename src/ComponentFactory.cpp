// Onut includes
#include <onut/Camera2DComponent.h>
#include <onut/Collider2DComponent.h>
#include <onut/ComponentFactory.h>
#include <onut/SoundComponent.h>
#include <onut/SpriteAnimComponent.h>
#include <onut/SpriteComponent.h>
#include <onut/TextComponent.h>
#include <onut/TiledMapComponent.h>

OComponentFactoryRef oComponentFactory;

namespace onut
{
    OComponentFactoryRef ComponentFactory::create()
    {
        return std::shared_ptr<ComponentFactory>(new ComponentFactory());
    }

    ComponentFactory::ComponentFactory()
    {
    }

    ComponentFactory::~ComponentFactory()
    {
    }

    void ComponentFactory::registerDefaultComponents()
    {
        ORegisterComponent(Camera2DComponent);
        OBindBoolProperty(Camera2DComponent, Clear);
        OBindColorProperty(Camera2DComponent, ClearColor);
        OBindFloatProperty(Camera2DComponent, Zoom);

        ORegisterComponent(Collider2DComponent);
        OBindVector2Property(Collider2DComponent, Size);
        OBindBoolProperty(Collider2DComponent, Trigger);
        OBindVector2Property(Collider2DComponent, Velocity);

        ORegisterComponent(SoundComponent);
        OBindSoundProperty(SoundComponent, Sound);
        OBindFloatProperty(SoundComponent, Volume);
        OBindFloatProperty(SoundComponent, Pitch);
        OBindBoolProperty(SoundComponent, Loop);

        ORegisterComponent(SpriteComponent);
        OBindTextureProperty(SpriteComponent, Texture);
        OBindVector2Property(SpriteComponent, Scale);
        OBindColorProperty(SpriteComponent, Color);
        OBindVector2Property(SpriteComponent, Origin);

        ORegisterComponent(SpriteAnimComponent);
        OBindSpriteAnimProperty(SpriteAnimComponent, SpriteAnim);
        OBindVector2Property(SpriteAnimComponent, Scale);
        OBindColorProperty(SpriteAnimComponent, Color);
        OBindStringProperty(SpriteAnimComponent, DefaultAnim);

        ORegisterComponent(TextComponent);
        OBindFontProperty(TextComponent, Font);
        OBindStringProperty(TextComponent, Text);
        OBindColorProperty(TextComponent, Color);
        OBindVector2Property(TextComponent, Origin);

        ORegisterComponent(TiledMapComponent);
        OBindTiledMapProperty(TiledMapComponent, TiledMap);
    }

    OComponentRef ComponentFactory::instantiate(const std::string& name) const
    {
        auto it = m_factoryMap.find(name);
        if (it == m_factoryMap.end()) return nullptr;
        auto& pFactory = it->second;
        return pFactory->instantiate();
    }

    void ComponentFactory::setProperty(const OComponentRef& pComponent, const std::string& componentName, const std::string& propertyName, const std::string& propertyValue)
    {
        auto it = m_propertyComponentMap.find(componentName);
        if (it == m_propertyComponentMap.end()) return;
        auto& componentProperties = it->second;
        auto it2 = componentProperties.propertyMap.find(propertyName);
        if (it2 == componentProperties.propertyMap.end()) return;
        auto& pProperty = it2->second;
        pProperty->set(pComponent.get(), propertyValue);
    }

    void ComponentFactory::registerEntity(uint32_t id, const OEntityRef& pEntity)
    {
        m_entityMap[id] = pEntity;
    }

    OEntityRef ComponentFactory::getEntity(uint32_t id)
    {
        return m_entityMap[id].lock();
    }

    void ComponentFactory::clearEntityRegistry()
    {
        m_entityMap.clear();
    }
};
