#pragma once

// Onut includes
#include <onut/Component.h>

// Forward declarations
#include <onut/ForwardDeclaration.h>
OForwardDeclare(TiledMapComponent);
OForwardDeclare(TiledMap);

namespace onut
{
    class TiledMapComponent final : public Component
    {
    public:
        void setTiledMap(const OTiledMapRef& pTiledMap);
        const OTiledMapRef& getTiledMap() const;

    private:
        void onRender2d() override;

        OTiledMapRef m_pTiledMap;
    };
};
