#pragma once

// Onut includes
#include <onut/Component.h>
#include <onut/Maths.h>

// Forward declarations
#include <onut/ForwardDeclaration.h>
OForwardDeclare(TiledMap);
OForwardDeclare(TiledMapColliderComponent);

namespace onut
{
    class TiledMapColliderComponent final : public Component
    {
    public:
        bool getCollideWithTiles() const;
        void setCollideWithTiles(bool collideWithTiles);

        const Vector2& getSize() const;
        void setSize(const Vector2& size);

        void setTiledMap(const OTiledMapRef& pTiledMap);
        const OTiledMapRef& getTiledMap() const;

    private:
        bool m_collideWithTiles = false;
        Vector2 m_size = Vector2::One;
        OTiledMapRef m_pTiledMap;
    };
};
