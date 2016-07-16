#pragma once

// Onut includes
#include <onut/Component.h>
#include <onut/Maths.h>

// Forward declarations
#include <onut/ForwardDeclaration.h>
OForwardDeclare(Collider2DComponent);

namespace onut
{
    class Collider2DComponent final : public Component
    {
    public:
        void setSize(const Vector2& color);
        const Vector2& getSize() const;

    private:
        void onUpdate() override;

        Vector2 m_size;
    };
};
