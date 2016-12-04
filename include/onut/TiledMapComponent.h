#ifndef TILEDMAPCOMPONENT_H_INCLUDED
#define TILEDMAPCOMPONENT_H_INCLUDED


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
        TiledMapComponent();
        ~TiledMapComponent();

        void setTiledMap(const OTiledMapRef& pTiledMap);
        const OTiledMapRef& getTiledMap() const;

        bool getPassable(const Point& mapPos) const;
        void setPassable(const Point& mapPos, bool passable);

    private:
        struct CollisionTile
        {
            Point mapPos;
            Point size;
            b2Body* pBody;
        };

        void onCreate() override;
        void onRender2d() override;
        void onUpdate() override;
        void onAddChild(const OEntityRef& pChild) override;

        void destroyCollisions();
        void createCollisionTiles(const iRect& rect);

        OTiledMapRef m_pTiledMap;
        std::vector<CollisionTile*> m_collisionTiles;
    };
};

#endif
