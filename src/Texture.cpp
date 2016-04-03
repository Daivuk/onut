// Onut
#include <onut/ContentManager.h>
#include <onut/Renderer.h>
#include <onut/Texture.h>

bool oGenerateMipmaps = true;

namespace onut
{
    Texture::~Texture()
    {
    }

    const Point& Texture::getSize() const
    {
        return m_size;
    }

    Vector2 Texture::getSizef() const
    {
        return Vector2(static_cast<float>(m_size.x), static_cast<float>(m_size.y));
    }

    bool Texture::isRenderTarget() const
    {
        return m_type == Type::RenderTarget || m_type == Type::ScreenRenderTarget;
    }

    bool Texture::isDynamic() const
    {
        return m_type == Type::Dynamic;
    }

    void Texture::bind(int slot)
    {
        assert(slot >= 0 && slot < RenderStates::MAX_TEXTURES);
        oRenderer->renderStates.textures[slot] = shared_from_this();
    }
}

OTextureRef OGetTexture(const std::string& name)
{
    return oContentManager->getResourceAs<OTexture>(name);
}
