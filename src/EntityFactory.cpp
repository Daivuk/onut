// Onut includes
#include <onut/Entity.h>
#include <onut/Font.h>
#include <onut/Sound.h>
#include <onut/SoundComponent.h>
#include <onut/SpriteComponent.h>
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
    auto pRet = OEntity::create();
    auto pTiledMapComponent = pRet->addComponent<OTiledMapComponent>();
    pTiledMapComponent->setTiledMap(OGetTiledMap(filename));
    return pRet;
}
