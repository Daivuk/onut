// Onut includes
#include <onut/Camera2DComponent.h>
#include <onut/Entity.h>
#include <onut/SceneManager.h>

namespace onut
{
    Camera2DComponent::Camera2DComponent()
        : Component(FLAG_UPDATABLE)
    {
    }

    void Camera2DComponent::setClear(bool clear)
    {
        m_clear = clear;
    }

    bool Camera2DComponent::getClear() const
    {
        return m_clear;
    }

    void Camera2DComponent::setClearColor(const Color& color)
    {
        m_clearColor = color;
    }

    const Color& Camera2DComponent::getClearColor() const
    {
        return m_clearColor;
    }

    void Camera2DComponent::setZoom(float zoom)
    {
        m_zoom = zoom;
    }

    const float Camera2DComponent::getZoom() const
    {
        return m_zoom;
    }

    void Camera2DComponent::onUpdate()
    {
        getEntity()->getSceneManager()->setActiveCamera2D(ODynamicCast<Camera2DComponent>(OThis));
    }
};