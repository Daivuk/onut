// Required for WinMain
#include <Windows.h>

// Oak Nut include
#include <onut/Camera2DComponent.h>
#include <onut/EntityFactory.h>
#include <onut/onut.h>
#include <onut/Renderer.h>
#include <onut/Settings.h>
#include <onut/TiledMap.h>
#include <onut/TiledMapComponent.h>

void init();
void update();
void render();

// Main
int CALLBACK WinMain(HINSTANCE appInstance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdCount)
{
    oSettings->setGameName("Dungeon Game Sample");
    ORun(init, update, render);
}

void init()
{
    // Create our tiled map entity, and get its actual tiled map.
    auto pTiledMapEntity = OCreateTiledMapEntity("dungeon.tmx");
    auto pTiledMapComponent = pTiledMapEntity->getComponent<OTiledMapComponent>();
    auto pTiledMap = pTiledMapComponent->getTiledMap();

    // Retreive the different layers from our map
    auto pTilesLayer = dynamic_cast<OTiledMap::TileLayer*>(pTiledMap->getLayer("tiles"));
    auto pObjLayer = dynamic_cast<OTiledMap::TileLayer*>(pTiledMap->getLayer("obj"));
    auto pInfoLayer = dynamic_cast<OTiledMap::TileLayer*>(pTiledMap->getLayer("info"));
    auto pEntitiesLayer = dynamic_cast<OTiledMap::ObjectLayer*>(pTiledMap->getLayer("entities"));

    // Place the entities from the entities layer on the map
    for (uint32_t i = 0; i < pEntitiesLayer->objectCount; ++i)
    {
        auto& object = pEntitiesLayer->pObjects[i];

        // Create the entity and set it's position
        auto pMapEntity = OEntity::create();
        auto position = object.position + Vector2(object.size / 2.0f);
        pMapEntity->setLocalTransform(Matrix::CreateTranslation(position));

        if (object.type == "start_location")
        {
            auto pCamera = pMapEntity->addComponent<OCamera2DComponent>();
            pCamera->setZoom(4.0f);
            pCamera->setClear(true);
            pCamera->setClearColor(Color::Black);
        }
        else if (object.type == "chest")
        {
        }
        else if (object.type == "vase")
        {
        }
        else if (object.type == "door")
        {
        }
        else if (object.type == "grumble")
        {
        }
        else if (object.type == "guard")
        {
        }
        else if (object.type == "mummy")
        {
        }
        else if (object.type == "bat")
        {
        }

        // Add the entity to the map
        pTiledMapEntity->add(pMapEntity);
    }
}

void update()
{
}

void render()
{
}
