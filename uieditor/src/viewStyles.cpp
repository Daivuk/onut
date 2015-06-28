#include "DocumentView.h"
#include "viewStyles.h"

static const Color g_panelBGColor = OColorHex(2d2d30);
static const Color g_outlineColor = Color::Lerp(OColorHex(ff9933), g_panelBGColor, .75f);
static const Color g_panelTitleBGColor = OColorHex(3f3f46);
static const Color g_fontColor = OColorHex(f1f1f1);
static const Color g_sizeHandleColor = OColorHex(999999);
static const Color g_btnStatesColors[4][2] = {
    {OColorHex(333337), OColorHex(3e3e42)},
    {OColorHex(999999), OColorHex(3e3e42)},
    {OColorHex(1c97ea), OColorHex(3e3e42)},
    {OColorHex(007acc), OColorHex(333337)}
};

static onut::BMFont* g_pFont;

extern DocumentView* g_pDocument;
extern onut::UIControl* g_pUIScreen;
extern onut::UIContext* g_pUIContext;

void createViewUIStyles(onut::UIContext* pContext)
{
    g_pFont = OGetBMFont("segeo12.fnt");

    pContext->onClipping = [](bool enabled, const onut::sUIRect& rect)
    {
        OSB->end();

        auto regionRect = onut::UI2Onut(g_pUIScreen->getChild("pnlRegion")->getWorldRect(*g_pUIContext));
        auto orect = onut::UI2Onut(rect);
        orect.x += regionRect.x;
        orect.y += regionRect.y;

        ORenderer->setScissor(enabled, orect);
        OSB->begin();
    };

    pContext->drawRect = [pContext](onut::UIControl *pControl, const onut::sUIRect &rect, const onut::sUIColor &color)
    {
        OSB->drawRect(nullptr, onut::UI2Onut(rect), onut::UI2Onut(color));
    };

    pContext->drawTexturedRect = [pContext](onut::UIControl *pControl, const onut::sUIRect &rect, const onut::sUIImageComponent &image)
    {
        auto pTexture = g_pDocument->contentManager.getResource<OTexture>(image.filename);
        OSB->drawRect(pTexture, onut::UI2Onut(rect), onut::UI2Onut(image.color));
    };

    pContext->drawScale9Rect = [pContext](onut::UIControl *pControl, const onut::sUIRect &rect, const onut::sUIScale9Component &scale9Component)
    {
        auto pTexture = g_pDocument->contentManager.getResource<OTexture>(scale9Component.image.filename);
        if (pTexture)
        {
            if (scale9Component.isRepeat)
            {
                OSB->drawRectScaled9RepeatCenters(pTexture, onut::UI2Onut(rect), onut::UI2Onut(scale9Component.padding), onut::UI2Onut(scale9Component.image.color));
            }
            else
            {
                OSB->drawRectScaled9(pTexture, onut::UI2Onut(rect), onut::UI2Onut(scale9Component.padding), onut::UI2Onut(scale9Component.image.color));
            }
        }
    };

    pContext->drawText = [pContext](onut::UIControl *pControl, const onut::sUIRect &rect, const onut::sUITextComponent &textComponent)
    {
        auto pFont = g_pDocument->contentManager.getResource<OFont>("../fonts/" + textComponent.font.typeFace);
        if (!pFont) pFont = g_pFont;
        auto align = onut::UI2Onut(textComponent.font.align);
        auto oRect = onut::UI2Onut(rect);
        pFont->draw<>(textComponent.text, ORectAlign<>(oRect, align), onut::UI2Onut(textComponent.font.color), OSB, align);
    };
}
