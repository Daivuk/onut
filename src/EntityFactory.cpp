// Onut includes
#include <onut/ComponentFactory.h>
#include <onut/Entity.h>
#include <onut/Files.h>
#include <onut/Font.h>
#include <onut/Log.h>
#include <onut/Sound.h>
#include <onut/SoundComponent.h>
#include <onut/SpriteComponent.h>
#include <onut/Strings.h>
#include <onut/Texture.h>
#include <onut/TextComponent.h>
#include <onut/TiledMap.h>
#include <onut/TiledMapComponent.h>

OEntityRef OCreateSoundEntity(const std::string& sound, const Vector2& position, float volume)
{
    auto pRet = OEntity::create();
    pRet->setLocalTransform(Matrix::CreateTranslation(position));
    auto pSoundComponent = pRet->addComponent<OSoundComponent>();
    pSoundComponent->setSound(OGetSound(sound));
    pSoundComponent->setVolume(volume);
    return pRet;
}

OEntityRef OCreateSpriteEntity(const std::string& filename, const Vector2& position)
{
    auto pRet = OEntity::create();
    pRet->setLocalTransform(Matrix::CreateTranslation(position));
    auto pSpriteComponent = pRet->addComponent<OSpriteComponent>();
    pSpriteComponent->setTexture(OGetTexture(filename));
    return pRet;
}

OEntityRef OCreateTextEntity(const std::string& font, const std::string& text, const Vector2& position)
{
    auto pRet = OEntity::create();
    pRet->setLocalTransform(Matrix::CreateTranslation(position));
    auto pTextComponent = pRet->addComponent<OTextComponent>();
    pTextComponent->setFont(OGetFont(font));
    pTextComponent->setText(text);
    return pRet;
}

OEntityRef OCreateTiledMapEntity(const std::string& filename)
{
    using ComponentMap = std::unordered_map<std::string, OComponentRef>;

    auto pRet = OEntity::create();
    pRet->setName(onut::getFilenameWithoutExtension(filename));
    auto pTiledMapComponent = pRet->addComponent<OTiledMapComponent>();
    auto pTiledMap = OGetTiledMap(filename);
    pTiledMapComponent->setTiledMap(pTiledMap);
    auto pEntitiesLayer = dynamic_cast<OTiledMap::ObjectLayer*>(pTiledMap->getLayer("entities"));

    if (pEntitiesLayer)
    {
        for (uint32_t i = 0; i < pEntitiesLayer->objectCount; ++i)
        {
            auto& object = pEntitiesLayer->pObjects[i];

            // Create the entity and set it's position
            auto pMapEntity = OEntity::create();
            auto position = object.position + Vector2(object.size / 2.0f);
            pMapEntity->setLocalTransform(Matrix::CreateTranslation(position));
            pMapEntity->setName(object.name);

            // Load his components
            ComponentMap componentMap;
            for (auto& kv : object.properties)
            {
                auto split = onut::splitString(kv.first, ':');
                if (split.size() == 0) continue;
                auto& componentName = split[0];
                auto it = componentMap.find(componentName);
                OComponentRef pComponent;
                if (it == componentMap.end())
                {
                    pComponent = oComponentFactory->instantiate(componentName);
                    if (!pComponent)
                    {
                        OLogW("Component not registered \"" + componentName + "\"");
                        continue;
                    }
                    componentMap[componentName] = pComponent;
                    pMapEntity->addComponent(pComponent);
                }
                else
                {
                    pComponent = it->second;
                }
                if (split.size() == 2)
                {
                    auto& propertyName = split[1];
                    auto& value = kv.second;
                    oComponentFactory->setProperty(pComponent, componentName, propertyName, value);
                }
            }

            //auto pCamera = pMapEntity->addComponent<OCamera2DComponent>();
            //pCamera->setZoom(4.0f);
            //pCamera->setClear(true);
            //pCamera->setClearColor(Color::Black);

            // Add the entity to the map
            pRet->add(pMapEntity);
        }
    }

    return pRet;
}
