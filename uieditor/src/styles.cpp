#include "DocumentView.h"
#include "styles.h"

static const Color g_panelBGColor = OColorHex(2d2d30);
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
OAnimf g_dottedLineAnim = 0.f;

void createUIStyles(onut::UIContext* pContext)
{
    g_pFont = OGetBMFont("segeo12.fnt");    
    g_dottedLineAnim.start(0.f, -1.f, .5f, OLinear, OLoop);

    pContext->addStyle<onut::UIPanel>("panel", [](const onut::UIPanel* pPanel, const onut::sUIRect& rect)
    {
        OSB->drawRect(nullptr, onut::UI2Onut(rect), g_panelBGColor);
    });

    pContext->addStyle<onut::UIPanel>("sizableRegion", [](const onut::UIPanel* pPanel, const onut::sUIRect& rect)
    {
        OSB->drawRect(nullptr, onut::UI2Onut(rect), Color::Black);
        OSB->end();

        // Render edited UIs
        ORenderer->set2DCamera({std::roundf(-rect.position.x), std::roundf(-rect.position.y)});
        OSB->begin();
        g_pDocument->render();
        OSB->end();

        ORenderer->set2DCamera({0, 0});
        OSB->begin();
    });

    pContext->addStyle<onut::UIImage>("icon", [](const onut::UIImage* pImage, const onut::sUIRect& rect)
    {
        OSB->drawRect(OGetTexture(pImage->getImage().c_str()), onut::UI2Onut(rect));
    });

    pContext->addStyle<onut::UIPanel>("view", [](const onut::UIPanel* pPanel, const onut::sUIRect& rect)
    {
    });

    pContext->addStyle<onut::UIPanel>("sizeHandle", [](const onut::UIPanel* pLabel, const onut::sUIRect& rect)
    {
        const auto orect = onut::UI2Onut(rect);
        OSB->drawRect(nullptr, orect, g_sizeHandleColor);
    });

    pContext->addStyle<onut::UILabel>("label", [](const onut::UILabel* pLabel, const onut::sUIRect& rect)
    {
        g_pFont->draw<OLeft>(pLabel->getText(), onut::UI2Onut(rect).Left(), g_fontColor);
    });

    pContext->addStyle<onut::UILabel>("panelTitle", [](const onut::UILabel* pLabel, const onut::sUIRect& rect)
    {
        const auto orect = onut::UI2Onut(rect);
        OSB->drawRect(nullptr, orect, g_panelTitleBGColor);
        g_pFont->draw<OCenter>(pLabel->getText(), orect.Center(), g_fontColor);
    });

    pContext->addStyle<onut::UIButton>("button", [pContext](const onut::UIButton* pButton, const onut::sUIRect& rect)
    {
        auto state = pButton->getState(*pContext);
        const auto rectOutter = onut::UI2Onut(rect);
        const auto rectInnuer = rectOutter.Grow(-1);
        switch (state)
        {
            case onut::eUIState::DISABLED:
                OSB->drawRect(nullptr, rectOutter, g_btnStatesColors[0][0]);
                OSB->drawRect(nullptr, rectInnuer, g_btnStatesColors[0][1]);
                g_pFont->draw<OCenter>(pButton->getCaption(), rectInnuer.Center(), g_fontColor * .5f);
                break;
            case onut::eUIState::NORMAL:
                OSB->drawRect(nullptr, rectOutter, g_btnStatesColors[1][0]);
                OSB->drawRect(nullptr, rectInnuer, g_btnStatesColors[1][1]);
                g_pFont->draw<OCenter>(pButton->getCaption(), rectInnuer.Center(), g_fontColor);
                break;
            case onut::eUIState::HOVER:
                OSB->drawRect(nullptr, rectOutter, g_btnStatesColors[2][0]);
                OSB->drawRect(nullptr, rectInnuer, g_btnStatesColors[2][1]);
                g_pFont->draw<OCenter>(pButton->getCaption(), rectInnuer.Center(), g_fontColor);
                break;
            case onut::eUIState::DOWN:
                OSB->drawRect(nullptr, rectOutter, g_btnStatesColors[3][0]);
                OSB->drawRect(nullptr, rectInnuer, g_btnStatesColors[3][1]);
                g_pFont->draw<OCenter>(pButton->getCaption(), rectInnuer.Center() + Vector2(1, 1), g_fontColor);
                break;
        }
    });

    pContext->addStyle<onut::UIButton>("imgButton", [pContext](const onut::UIButton* pButton, const onut::sUIRect& rect)
    {
        auto state = pButton->getState(*pContext);
        const auto rectOutter = onut::UI2Onut(rect);
        const auto rectInnuer = rectOutter.Grow(-1);
        auto textRect = rectInnuer;
        textRect.x += 24;
        textRect.z -= 24;
        switch (state)
        {
            case onut::eUIState::DISABLED:
                OSB->drawRect(nullptr, rectOutter, g_btnStatesColors[0][0]);
                OSB->drawRect(nullptr, rectInnuer, g_btnStatesColors[0][1]);
                g_pFont->draw<OLeft>(pButton->getCaption(), textRect.Left(), g_fontColor * .5f);
                break;
            case onut::eUIState::NORMAL:
                OSB->drawRect(nullptr, rectOutter, g_btnStatesColors[1][0]);
                OSB->drawRect(nullptr, rectInnuer, g_btnStatesColors[1][1]);
                g_pFont->draw<OLeft>(pButton->getCaption(), textRect.Left(), g_fontColor);
                break;
            case onut::eUIState::HOVER:
                OSB->drawRect(nullptr, rectOutter, g_btnStatesColors[2][0]);
                OSB->drawRect(nullptr, rectInnuer, g_btnStatesColors[2][1]);
                g_pFont->draw<OLeft>(pButton->getCaption(), textRect.Left(), g_fontColor);
                break;
            case onut::eUIState::DOWN:
                OSB->drawRect(nullptr, rectOutter, g_btnStatesColors[3][0]);
                OSB->drawRect(nullptr, rectInnuer, g_btnStatesColors[3][1]);
                g_pFont->draw<OLeft>(pButton->getCaption(), textRect.Left() + Vector2(1, 1), g_fontColor);
                break;
        }
    });

    pContext->addStyle<onut::UIPanel>("gizmo", [](const onut::UIPanel* pPanel, const onut::sUIRect& rect)
    {
        const Color DOTTED_LINE_COLOR = {1, 1, 1, .5f};

        auto pDottedLineTexture = OGetTexture("dottedLine.png");
        auto dottedLineScale = 1.f / pDottedLineTexture->getSizef().x;
        auto dottedLineOffset = g_dottedLineAnim.get();

        OSB->drawRectWithUVs(pDottedLineTexture,
        {rect.position.x, rect.position.y, 1, rect.size.y},
        {dottedLineOffset, dottedLineOffset, dottedLineOffset + dottedLineScale, dottedLineOffset + rect.size.y * dottedLineScale},
        DOTTED_LINE_COLOR);

        OSB->drawRectWithUVs(pDottedLineTexture,
        {rect.position.x + rect.size.x - 1, rect.position.y, 1, rect.size.y},
        {dottedLineOffset, dottedLineOffset, dottedLineOffset + dottedLineScale, dottedLineOffset + rect.size.y * dottedLineScale},
        DOTTED_LINE_COLOR);

        OSB->drawRectWithUVs(pDottedLineTexture,
        {rect.position.x, rect.position.y, rect.size.x, 1},
        {dottedLineOffset, dottedLineOffset, dottedLineOffset + rect.size.x * dottedLineScale, dottedLineOffset + dottedLineScale},
        DOTTED_LINE_COLOR);

        OSB->drawRectWithUVs(pDottedLineTexture,
        {rect.position.x, rect.position.y + rect.size.y - 1, rect.size.x, 1},
        {dottedLineOffset, dottedLineOffset, dottedLineOffset + rect.size.x * dottedLineScale, dottedLineOffset + dottedLineScale},
        DOTTED_LINE_COLOR);
    });
}
