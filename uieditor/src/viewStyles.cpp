#include "DocumentView.h"
#include "viewStyles.h"

#include "onut/Font.h"
#include "onut/Texture.h"

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

static OFontRef g_pFont;

extern DocumentView* g_pDocument;
extern OUIControlRef g_pUIScreen;
extern OUIContextRef g_pUIContext;

void createViewUIStyles(const OUIContextRef& pContext)
{
    //g_pFont = OGetFont("segeo12.fnt");

    //pContext->onClipping = [](bool enabled, const Rect& rect)
    //{
    //    oSpriteBatch->end();

    //    auto regionRect = (g_pUIScreen->getChild("pnlRegion")->getWorldRect(*g_pUIContext));
    //    auto orect = (rect);
    //    orect.x += regionRect.x;
    //    orect.y += regionRect.y;

    //    oRenderer->renderStates.scissorEnabled = enabled;
    //    if (enabled)
    //    {
    //        oRenderer->renderStates.scissor = {static_cast<int>(orect.x), static_cast<int>(orect.y), static_cast<int>(orect.x + orect.z), static_cast<int>(orect.y + orect.w)};
    //    }
    //    oSpriteBatch->begin();
    //};

    //pContext->drawRect = [pContext](const OUIControlRef& pControl, const Rect &rect, const Color &color)
    //{
    //    oSpriteBatch->drawRect(nullptr, (rect), (color));
    //};

    //pContext->drawTexturedRect = [pContext](const OUIControlRef& pControl, const Rect &rect, const onut::UIImageComponent &image)
    //{
    //    auto pTexture = g_pDocument->pContentManager->getResourceAs<OTexture>(image.filename);
    //    oSpriteBatch->drawRect(pTexture, (rect), (image.color));
    //};

    //pContext->drawScale9Rect = [pContext](const OUIControlRef& pControl, const Rect &rect, const onut::UIScale9Component &scale9Component)
    //{
    //    auto pTexture = g_pDocument->pContentManager->getResourceAs<OTexture>(scale9Component.image.filename);
    //    if (pTexture)
    //    {
    //        if (scale9Component.isRepeat)
    //        {
    //            oSpriteBatch->drawRectScaled9RepeatCenters(pTexture, (rect), (scale9Component.padding), (scale9Component.image.color));
    //        }
    //        else
    //        {
    //            oSpriteBatch->drawRectScaled9(pTexture, (rect), (scale9Component.padding), (scale9Component.image.color));
    //        }
    //    }
    //};

    //pContext->drawText = [pContext](const OUIControlRef& pControl, const Rect &rect, const onut::UITextComponent &textComponent)
    //{
    //    auto pFont = g_pDocument->pContentManager->getResourceAs<OFont>(textComponent.font.typeFace);
    //    if (!pFont) pFont = g_pFont;
    //    auto align = (textComponent.font.align);
    //    auto oRect = (rect);
    //    pFont->draw(textComponent.text, ORectAlign<>(oRect, align), align, (textComponent.font.color));
    //};
}
