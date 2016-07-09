// Onut include
#include <onut/Entity.h>
#include <onut/Maths.h>
#include <onut/Texture.h>

OEntityRef OCreateSpriteEntity(const OTextureRef& pTexture, const Vector2& position = Vector2::Zero);
OEntityRef OCreateSpriteEntity(const std::string& filename, const Vector2& position = Vector2::Zero);
