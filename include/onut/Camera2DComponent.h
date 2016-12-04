#ifndef CAMERA2DCOMPONENT_H_INCLUDED
#define CAMERA2DCOMPONENT_H_INCLUDED


// Onut includes
#include <onut/Component.h>
#include <onut/Maths.h>

// Forward declarations
#include <onut/ForwardDeclaration.h>
OForwardDeclare(Camera2DComponent);

namespace onut
{
    class Camera2DComponent final : public Component
    {
    public:
        Camera2DComponent();

        void setZoom(float zoom);
        const float getZoom() const;

        void setClearColor(const Color& color);
        const Color& getClearColor() const;

        void setClear(bool clear);
        bool getClear() const;

    private:
        void onUpdate() override;

        bool m_clear = true;
        Color m_clearColor = OColorHex(1d232d);
        float m_zoom = 1.0f;
    };
};

#endif
