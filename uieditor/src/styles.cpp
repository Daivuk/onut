#include "styles.h"

static Color g_panelBGColor = OColorHex(2d2d30);

void createUIStyles(onut::UIContext* pContext)
{
    pContext->addStyle("panel", [](const onut::UIControl* pControl, const onut::sUIRect& rect)
    {
        OSB->drawRect(nullptr, onut::UI2Onut(rect), g_panelBGColor);
    });
}
