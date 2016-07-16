#pragma once

// Onut includes
#include <onut/Component.h>

// STL
#include <vector>

// Forward declarations
#include <onut/ForwardDeclaration.h>
OForwardDeclare(TiledMapComponent);
OForwardDeclare(TiledMap);
class b2Body;

namespace onut
{
    class TiledMapComponent final : public Component
    {
    public:
        ~TiledMapComponent();

        void setTiledMap(const OTiledMapRef& pTiledMap);
        const OTiledMapRef& getTiledMap() const;

    private:
        struct CollisionTile
        {
            Point mapPos;
            Point size;
            b2Body* pBody;
        };

        void onCreate() override;
        void onRender2d() override;
        void onAddChild(const OEntityRef& pChild) override;

        void destroyCollisions();

        OTiledMapRef m_pTiledMap;
        std::vector<CollisionTile*> m_collisionTiles;
    };
};
