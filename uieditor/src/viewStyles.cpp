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

void createViewUIStyles(onut::UIContext* pContext)
{
    g_pFont = OGetBMFont("segeo12.fnt");

    pContext->addStyle<onut::UIPanel>("", [pContext](const onut::UIPanel* pPanel, const onut::sUIRect& rect)
    {
        const auto rectOutter = onut::UI2Onut(rect);
        auto& color = pPanel->color;

        OSB->drawRect(nullptr, rectOutter, {color.r, color.g, color.b, color.a});
    });

    pContext->addStyle<onut::UIButton>("", [pContext](const onut::UIButton* pButton, const onut::sUIRect& rect)
    {
        const auto rectOutter = onut::UI2Onut(rect);
        const auto rectInnuer = rectOutter.Grow(-1);
        auto textColor = onut::UI2Onut(pButton->textComponent.font.color);

        if (pButton->getState(*pContext) == onut::eUIState::DISABLED)
        {
            OSB->drawRect(nullptr, rectOutter, g_btnStatesColors[0][0]);
            OSB->drawRect(nullptr, rectInnuer, g_btnStatesColors[0][1]);
            g_pFont->draw<OCenter>(pButton->textComponent.text, rectInnuer.Center(), textColor * .5f);
        }
        else
        {
            OSB->drawRect(nullptr, rectOutter, g_btnStatesColors[1][0]);
            OSB->drawRect(nullptr, rectInnuer, g_btnStatesColors[1][1]);
            g_pFont->draw<OCenter>(pButton->textComponent.text, rectInnuer.Center(), textColor);
        }
    });

    pContext->addStyle<onut::UILabel>("", [pContext](const onut::UILabel* pLabel, const onut::sUIRect& rect)
    {
        g_pFont->draw<OLeft>(pLabel->textComponent.text, onut::UI2Onut(rect).Left(), g_fontColor);
    });

    pContext->addStyle<onut::UIImage>("", [pContext](const onut::UIImage* pImage, const onut::sUIRect& rect)
    {
        auto pTexture = g_pDocument->contentManager.getResource<OTexture>(pImage->scale9Component.image.filename);
        auto color = onut::UI2Onut(pImage->scale9Component.image.color);
        if (pImage->scale9Component.isScaled9 && pTexture)
        {
            OSB->drawRectScaled9(pTexture, onut::UI2Onut(rect), onut::UI2Onut(pImage->scale9Component.padding), color);
        }
        else
        {
            OSB->drawRect(pTexture, onut::UI2Onut(rect), color);
        }
    });
}
