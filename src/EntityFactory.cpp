// Onut includes
#include <onut/Entity.h>
#include <onut/SpriteComponent.h>
#include <onut/Texture.h>

OEntityRef OCreateSpriteEntity(const OTextureRef& pTexture, const Vector2& position)
{
    auto pRet = OEntity::create();
    pRet->setLocalTransform(Matrix::CreateTranslation(position));
    auto pSpriteComponent = pRet->addComponent<OSpriteComponent>();
    pSpriteComponent->setTexture(pTexture);
    return pRet;
}

OEntityRef OCreateSpriteEntity(const std::string& filename, const Vector2& position)
{
    return OCreateSpriteEntity(OGetTexture(filename), position);
}
