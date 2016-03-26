// Required for WinMain
#include <Windows.h>

// Oak Nut include
#include <onut/ActionManager.h>
#include <onut/Anim.h>
#include <onut/Input.h>
#include <onut/Font.h>
#include <onut/Maths.h>
#include <onut/onut.h>
#include <onut/Random.h>
#include <onut/Renderer.h>
#include <onut/Settings.h>
#include <onut/SpriteBatch.h>
#include <onut/Texture.h>

void init();
void update();
void render();

struct Item
{
    Vector2 position;
};

using ItemRef = std::shared_ptr<Item>;
using Items = std::vector<ItemRef>;

Items items;
ItemRef pDraggingItem;
Vector2 dragStartMousePos;
Vector2 dragStartItemPos;

// Main
int CALLBACK WinMain(HINSTANCE appInstance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdCount)
{
    oSettings->setGameName("Undo/Redo Sample");
    ORun(init, update, render);
}

void init()
{
}

ItemRef getItemAt(const Vector2& pos)
{
    // We loop backward to get the top most
    for (auto it = items.rbegin(); it != items.rend(); ++it)
    {
        auto& pItem = *it;
        if (pos.x >= pItem->position.x - 64.f &&
            pos.y >= pItem->position.y - 64.f &&
            pos.x <= pItem->position.x + 64.f &&
            pos.y <= pItem->position.y + 64.f)
        {
            return pItem;
        }
    }

    return nullptr;
}

void removeItem(const ItemRef& pItem)
{
    for (auto it = items.begin(); it != items.end(); ++it)
    {
        if (*it == pItem)
        {
            items.erase(it);
            return;
        }
    }
}

void update()
{
    if (pDraggingItem)
    {
        // Dragging item logic
        auto mouseDiff = oInput->mousePosf - dragStartMousePos;
        pDraggingItem->position = dragStartItemPos + mouseDiff;

        if (OInputJustReleased(OMouse1))
        {
            if (pDraggingItem->position != dragStartItemPos)
            {
                auto pItem = pDraggingItem;
                auto startPos = dragStartItemPos;
                auto endPos = pDraggingItem->position;

                oActionManager->doAction("Move Item",
                    [=]{ // Redo
                        pItem->position = endPos;
                    },
                    [=]{ // Undo
                        pItem->position = startPos;
                    });
            }
            pDraggingItem = nullptr;
        }
    }
    else
    {
        // App logic
        if (OInputJustPressed(OKeySpaceBar))
        {
            auto pItem = OMake<Item>();
            pItem->position = ORandVector2(OScreenf);

            oActionManager->doAction("Create Item",
                [=]{ // Redo
                    items.push_back(pItem);
                },
                [=]{ // Undo
                    removeItem(pItem);
                });
        }
        else if (OInputJustPressed(OKeyDelete))
        {
            auto pItem = getItemAt(oInput->mousePosf);
            if (pItem)
            {
                oActionManager->doAction("Remove Item",
                    [=]{ // Redo
                        removeItem(pItem);
                    },
                    [=]{ // Undo
                        items.push_back(pItem);
                    });
            }
        }

        if (OInputPressed(OKeyLeftControl) && !OInputPressed(OKeyLeftShift) && OInputJustPressed(OKeyZ))
        {
            // Undo!
            oActionManager->undo();
        }

        if (OInputPressed(OKeyLeftControl) && OInputPressed(OKeyLeftShift) && OInputJustPressed(OKeyZ))
        {
            // Redo!
            oActionManager->redo();
        }

        if (OInputJustPressed(OMouse1))
        {
            dragStartMousePos = oInput->mousePosf;
            pDraggingItem = getItemAt(dragStartMousePos);
            if (pDraggingItem)
            {
                dragStartItemPos = pDraggingItem->position;
            }
        }
    }
}

void render()
{
    auto pNutTexture = OGetTexture("onutLogo.png");
    auto pFont = OGetFont("font.fnt");

    // Clear
    oRenderer->clear(OColorHex(1d232d));

    // Begin a batch
    oSpriteBatch->begin();

    // Legend
    pFont->draw("Press space to create an item", Vector2(0, 0), OTopLeft, Color(1, 1, 0, 1));
    pFont->draw("Hold and drag to move an item around", Vector2(0, 15), OTopLeft, Color(1, 1, 0, 1));
    pFont->draw("Press delete to delete the hovering item", Vector2(0, 30), OTopLeft, Color(1, 1, 0, 1));
    pFont->draw("Press Ctrl+Z to Undo", Vector2(0, 45), OTopLeft, Color(1, 1, 0, 1));
    pFont->draw("Press Ctrl+Shift+Z to Redo", Vector2(0, 60), OTopLeft, Color(1, 1, 0, 1));

    // Draw history
    Vector2 drawPos = Vector2(OScreenWf, 0);
    pFont->draw("History", drawPos, OTopRight, Color(1, 1, 0, 1));
    drawPos.y += 15;

    bool inUndo = true;
    auto& history = oActionManager->getHistory();
    auto historyPosition = oActionManager->getHistoryPosition();
    for (auto it = history.begin(); it != history.end(); ++it)
    {
        if (historyPosition == it) inUndo = false;

        auto pAction = *it;

        if (inUndo)
        {
            pFont->draw(pAction->getName(), drawPos, OTopRight, Color(1, 1, 1, 1));
        }
        else
        {
            pFont->draw(pAction->getName(), drawPos, OTopRight, Color(.4f, .4f, .4f, 1));
        }
        drawPos.y += 15;
    }

    // Draw items
    for (auto& item : items)
    {
        oSpriteBatch->drawSprite(pNutTexture, item->position);
    }

    // End and flush the batch
    oSpriteBatch->end();
}
