#include "DocumentView.h"
#include "styles.h"

static const Color g_panelBGColor = OColorHex(2d2d30);
static const Color g_panelDarkenBGColor = Color::fromHexRGBA(0x00000066);
static const Color g_groupOutlineColor = OColorHex(3f3f46);
static const Color g_panelTitleBGColor = OColorHex(3f3f46);
static const Color g_fontColor = OColorHex(f1f1f1);
static const Color g_fontColorFaded = OColorHex(f1f1f1) * .5f;
static const Color g_sizeHandleColor = OColorHex(999999);
static const Color g_btnStatesColors[4][2] = {
    {OColorHex(333337), OColorHex(3e3e42)},
    {OColorHex(999999), OColorHex(3e3e42)},
    {OColorHex(1c97ea), OColorHex(3e3e42)},
    {OColorHex(007acc), OColorHex(333337)}
};
static const Color g_toolBtnHoverColor = OColorHex(3e3e40);
static const Color g_toolBtnDownColor = OColorHex(007acc);
static const Color g_toolBtnDisabledColor = {.4f, .4f, .4f, .4f};
static const Color g_deepViewFill = OColorHex(252526);
static const Color g_deepViewOutline = OColorHex(3f3f46);
static const Color g_treeItemSelectedBGColor = OColorHex(3399ff);
static const Color g_selectedTextColor = OColorHex(ffffff);
static const Color g_cursorColor = OColorHex(dadad9);
static const Color g_cursorSelectionColor = OColorHex(cc6600);
static const Color g_guideColor = OColorHex(3399ff);

static onut::BMFont* g_pFont;
static onut::Texture* g_pTexTreeOpen;
static onut::Texture* g_pTexTreeClose;

static onut::Texture* g_pTexIcoUIControl;
static onut::Texture* g_pTexIcoUIPanel;
static onut::Texture* g_pTexIcoUIButton;
static onut::Texture* g_pTexIcoUILabel;
static onut::Texture* g_pTexIcoUIImage;
static onut::Texture* g_pTexIcoUICheckbox;
static onut::Texture* g_pTexIcoUITextBox;

extern DocumentView* g_pDocument;
OAnimf g_dottedLineAnim = 0.f;

void createUIStyles(onut::UIContext* pContext)
{
    g_pFont = OGetBMFont("segeo12.fnt");
    g_pTexTreeOpen = OGetTexture("viewItemOpen.png");
    g_pTexTreeClose = OGetTexture("viewItemClose.png");
    g_pTexIcoUIControl = OGetTexture("shape_handles.png");
    g_pTexIcoUIPanel = OGetTexture("shape_square.png");
    g_pTexIcoUIButton = OGetTexture("icoButton.png");
    g_pTexIcoUILabel = OGetTexture("text_dropcaps.png");
    g_pTexIcoUIImage = OGetTexture("picture.png");
    g_pTexIcoUICheckbox = OGetTexture("accept.png");
    g_pTexIcoUITextBox = OGetTexture("textfield.png");
    g_dottedLineAnim.start(0.f, -1.f, .5f, OLinear, OLoop);

    pContext->addStyle<onut::UIPanel>("", [](const onut::UIPanel* pPanel, const onut::sUIRect& rect)
    {
        OSB->drawRect(nullptr, onut::UI2Onut(rect), g_panelBGColor);
    });

    pContext->addStyle<onut::UIPanel>("darken", [](const onut::UIPanel* pPanel, const onut::sUIRect& rect)
    {
        OSB->drawRect(nullptr, onut::UI2Onut(rect), g_panelDarkenBGColor);
    });

    pContext->addStyle<onut::UIPanel>("colorPicker", [pContext](const onut::UIPanel* pPanel, const onut::sUIRect& rect)
    {
        auto& color = pPanel->color;

        auto state = pPanel->getState(*pContext);
        const auto rectOutter = onut::UI2Onut(rect);
        const auto rectInnuer = rectOutter.Grow(-1);
        const auto rectColor = rectOutter.Grow(-4);
        switch (state)
        {
            case onut::eUIState::DISABLED:
                OSB->drawRect(nullptr, rectOutter, g_btnStatesColors[0][0]);
                OSB->drawRect(nullptr, rectInnuer, g_btnStatesColors[0][1]);
                break;
            case onut::eUIState::NORMAL:
                OSB->drawRect(nullptr, rectOutter, g_btnStatesColors[1][0]);
                OSB->drawRect(nullptr, rectInnuer, g_btnStatesColors[1][1]);
                break;
            case onut::eUIState::HOVER:
                OSB->drawRect(nullptr, rectOutter, g_btnStatesColors[2][0]);
                OSB->drawRect(nullptr, rectInnuer, g_btnStatesColors[2][1]);
                break;
            case onut::eUIState::DOWN:
                OSB->drawRect(nullptr, rectOutter, g_btnStatesColors[3][0]);
                OSB->drawRect(nullptr, rectInnuer, g_btnStatesColors[3][1]);
                break;
        }

        OSB->drawRect(nullptr, rectColor, {color.r, color.g, color.b, color.a});
    });

    pContext->addStyle<onut::UIPanel>("guide", [](const onut::UIPanel* pPanel, const onut::sUIRect& rect)
    {
        OSB->drawRect(nullptr, onut::UI2Onut(rect), g_guideColor);
    });

    pContext->addStyle<onut::UIPanel>("group", [](const onut::UIPanel* pPanel, const onut::sUIRect& rect)
    {
        auto orect = onut::UI2Onut(rect);
        OSB->drawRect(nullptr, orect, g_groupOutlineColor);
        OSB->drawRect(nullptr, orect.Grow(-1), g_panelBGColor);
    });

    pContext->addStyle<onut::UITreeView>("", [](const onut::UITreeView* pPanel, const onut::sUIRect& rect)
    {
        auto orect = onut::UI2Onut(rect);
        OSB->drawRect(nullptr, orect.Grow(1), g_deepViewOutline);
        OSB->drawRect(nullptr, orect, g_deepViewFill);
    });

    pContext->addStyle<onut::UITreeViewItem>("", [pContext](const onut::UITreeViewItem* pItem, const onut::sUIRect& rect)
    {
        auto pTreeView = pItem->getTreeView();
        auto orect = onut::UI2Onut(rect);
        if (pItem->getIsSelected())
        {
            auto selectionRect = orect;
            auto treeViewWorldRect = pTreeView->getWorldRect(*pContext);
            selectionRect.x = treeViewWorldRect.position.x;
            selectionRect.z = treeViewWorldRect.size.x;
            OSB->drawRect(nullptr, selectionRect, g_treeItemSelectedBGColor);
        }
        auto expandClickWidth = pTreeView->expandClickWidth;

        // Expand icon
        if (!pItem->getItems().empty())
        {
            if (pItem->isExpanded)
            {
                OSB->drawSprite(g_pTexTreeOpen, orect.Left(expandClickWidth * .5f + 4));
            }
            else
            {
                OSB->drawSprite(g_pTexTreeClose, orect.Left(expandClickWidth * .5f + 4));
            }
        }

        const auto& text = pItem->text;
        bool hasText = !text.empty();
        auto pControl = static_cast<onut::UIControl*>(pItem->pUserData);
        auto textPos = orect.Left(expandClickWidth + 24);
        auto textColor = g_fontColor;
        if (!pControl->isEnabled)
        {
            textColor = g_fontColorFaded;
        }

        // UI icon
        switch (pControl->getType())
        {
            case onut::eUIType::UI_CONTROL:
                OSB->drawSprite(g_pTexIcoUIControl, orect.Left(expandClickWidth + 12));
                if (!hasText)
                {
                    static const std::string uiText = "UIControl";
                    g_pFont->draw<OLeft>(uiText, textPos, textColor);
                }
                break;
            case onut::eUIType::UI_PANEL:
                OSB->drawSprite(g_pTexIcoUIPanel, orect.Left(expandClickWidth + 12));
                if (!hasText)
                {
                    static const std::string uiText = "UIPanel";
                    g_pFont->draw<OLeft>(uiText, textPos, textColor);
                }
                break;
            case onut::eUIType::UI_BUTTON:
                OSB->drawSprite(g_pTexIcoUIButton, orect.Left(expandClickWidth + 12));
                if (!hasText)
                {
                    static const std::string uiText = "UIButton";
                    g_pFont->draw<OLeft>(uiText, textPos, textColor);
                }
                break;
            case onut::eUIType::UI_LABEL:
                OSB->drawSprite(g_pTexIcoUILabel, orect.Left(expandClickWidth + 12));
                if (!hasText)
                {
                    static const std::string uiText = "UILabel";
                    g_pFont->draw<OLeft>(uiText, textPos, textColor);
                }
                break;
            case onut::eUIType::UI_IMAGE:
                OSB->drawSprite(g_pTexIcoUIImage, orect.Left(expandClickWidth + 12));
                if (!hasText)
                {
                    static const std::string uiText = "UIImage";
                    g_pFont->draw<OLeft>(uiText, textPos, textColor);
                }
                break;
            case onut::eUIType::UI_CHECKBOX:
                OSB->drawSprite(g_pTexIcoUICheckbox, orect.Left(expandClickWidth + 12));
                if (!hasText)
                {
                    static const std::string uiText = "UICheckBox";
                    g_pFont->draw<OLeft>(uiText, textPos, textColor);
                }
                break;
            case onut::eUIType::UI_TEXTBOX:
                OSB->drawSprite(g_pTexIcoUITextBox, orect.Left(expandClickWidth + 12));
                if (!hasText)
                {
                    static const std::string uiText = "UITextBox";
                    g_pFont->draw<OLeft>(uiText, textPos, textColor);
                }
                break;
        }

        // Label
        if (hasText)
        {
            g_pFont->draw<OLeft>(pItem->text, textPos, g_fontColor);
        }
    });

    pContext->addStyle<onut::UIImage>("", [](const onut::UIImage* pImage, const onut::sUIRect& rect)
    {
        OSB->drawRect(OGetTexture(pImage->scale9Component.image.filename.c_str()), onut::UI2Onut(rect));
    });

    pContext->addStyle<onut::UIImage>("toolBtn", [pContext](const onut::UIImage* pImage, const onut::sUIRect& rect)
    {
        auto orect = onut::UI2Onut(rect);
        auto state = pImage->getState(*pContext);
        auto pos = orect.Center();
        pos.x = std::roundf(pos.x);
        pos.y = std::roundf(pos.y);
        switch (state)
        {
            case onut::eUIState::NORMAL:
                OSB->drawSprite(OGetTexture(pImage->scale9Component.image.filename.c_str()), pos);
                break;
            case onut::eUIState::HOVER:
                OSB->drawRect(nullptr, orect, g_toolBtnHoverColor);
                OSB->drawSprite(OGetTexture(pImage->scale9Component.image.filename.c_str()), pos);
                break;
            case onut::eUIState::DOWN:
                OSB->drawRect(nullptr, orect, g_toolBtnDownColor);
                OSB->drawSprite(OGetTexture(pImage->scale9Component.image.filename.c_str()), pos);
                break;
            case onut::eUIState::DISABLED:
                OSB->drawSprite(OGetTexture(pImage->scale9Component.image.filename.c_str()), pos, g_toolBtnDisabledColor);
                break;
        }
    });
    
    pContext->addStyle<onut::UIPanel>("toolSeparator", [](const onut::UIPanel* pPanel, const onut::sUIRect& rect)
    {
        auto orect = onut::UI2Onut(rect);
        auto pos = orect.Center();
        pos.x = std::roundf(pos.x);
        pos.y = std::roundf(pos.y);
        OSB->drawSprite(OGetTexture("toolSeparator.png"), pos);
    });

    pContext->addStyle<onut::UIPanel>("view", [](const onut::UIPanel* pPanel, const onut::sUIRect& rect)
    {
    });

    pContext->addStyle<onut::UIPanel>("sizeHandle", [](const onut::UIPanel* pLabel, const onut::sUIRect& rect)
    {
        const auto orect = onut::UI2Onut(rect);
        OSB->drawRect(nullptr, orect, g_sizeHandleColor);
    });

    pContext->addStyle<onut::UILabel>("", [](const onut::UILabel* pLabel, const onut::sUIRect& rect)
    {
        g_pFont->draw<OLeft>(pLabel->textComponent.text, onut::UI2Onut(rect).Left(), g_fontColor);
    });

    pContext->addStyle<onut::UILabel>("panelTitle", [](const onut::UILabel* pLabel, const onut::sUIRect& rect)
    {
        const auto orect = onut::UI2Onut(rect);
        OSB->drawRect(nullptr, orect, g_panelTitleBGColor);
        g_pFont->draw<OCenter>(pLabel->textComponent.text, orect.Center(), g_fontColor);
    });

    pContext->addStyle<onut::UIButton>("", [pContext](const onut::UIButton* pButton, const onut::sUIRect& rect)
    {
        auto state = pButton->getState(*pContext);
        const auto rectOutter = onut::UI2Onut(rect);
        const auto rectInnuer = rectOutter.Grow(-1);
        switch (state)
        {
            case onut::eUIState::DISABLED:
                OSB->drawRect(nullptr, rectOutter, g_btnStatesColors[0][0]);
                OSB->drawRect(nullptr, rectInnuer, g_btnStatesColors[0][1]);
                g_pFont->draw<OCenter>(pButton->textComponent.text, rectInnuer.Center(), g_fontColor * .5f);
                break;
            case onut::eUIState::NORMAL:
                OSB->drawRect(nullptr, rectOutter, g_btnStatesColors[1][0]);
                OSB->drawRect(nullptr, rectInnuer, g_btnStatesColors[1][1]);
                g_pFont->draw<OCenter>(pButton->textComponent.text, rectInnuer.Center(), g_fontColor);
                break;
            case onut::eUIState::HOVER:
                OSB->drawRect(nullptr, rectOutter, g_btnStatesColors[2][0]);
                OSB->drawRect(nullptr, rectInnuer, g_btnStatesColors[2][1]);
                g_pFont->draw<OCenter>(pButton->textComponent.text, rectInnuer.Center(), g_fontColor);
                break;
            case onut::eUIState::DOWN:
                OSB->drawRect(nullptr, rectOutter, g_btnStatesColors[3][0]);
                OSB->drawRect(nullptr, rectInnuer, g_btnStatesColors[3][1]);
                g_pFont->draw<OCenter>(pButton->textComponent.text, rectInnuer.Center() + Vector2(1, 1), g_fontColor);
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
                g_pFont->draw<OLeft>(pButton->textComponent.text, textRect.Left(), g_fontColor * .5f);
                break;
            case onut::eUIState::NORMAL:
                OSB->drawRect(nullptr, rectOutter, g_btnStatesColors[1][0]);
                OSB->drawRect(nullptr, rectInnuer, g_btnStatesColors[1][1]);
                g_pFont->draw<OLeft>(pButton->textComponent.text, textRect.Left(), g_fontColor);
                break;
            case onut::eUIState::HOVER:
                OSB->drawRect(nullptr, rectOutter, g_btnStatesColors[2][0]);
                OSB->drawRect(nullptr, rectInnuer, g_btnStatesColors[2][1]);
                g_pFont->draw<OLeft>(pButton->textComponent.text, textRect.Left(), g_fontColor);
                break;
            case onut::eUIState::DOWN:
                OSB->drawRect(nullptr, rectOutter, g_btnStatesColors[3][0]);
                OSB->drawRect(nullptr, rectInnuer, g_btnStatesColors[3][1]);
                g_pFont->draw<OLeft>(pButton->textComponent.text, textRect.Left() + Vector2(1, 1), g_fontColor);
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

    pContext->addStyle<onut::UICheckBox>("", [pContext](const onut::UICheckBox* pCheckBox, const onut::sUIRect& rect)
    {
        auto state = pCheckBox->getState(*pContext);
        const auto rectOutter = onut::UI2Onut(rect);
        auto rectChk = rectOutter;
        rectChk.x += 4;
        rectChk.y = std::floorf(rectChk.y + rectChk.w / 2 - 6);
        rectChk.z = 12;
        rectChk.w = 12;
        switch (state)
        {
            case onut::eUIState::DISABLED:
                OSB->drawRect(nullptr, rectChk, g_btnStatesColors[0][0]);
                OSB->drawRect(nullptr, rectChk.Grow(-1), g_btnStatesColors[0][1]);
                g_pFont->draw<OLeft>(pCheckBox->textComponent.text, rectOutter.Left(20), g_fontColor * .5f);
                break;
            case onut::eUIState::NORMAL:
                OSB->drawRect(nullptr, rectChk, g_btnStatesColors[1][0]);
                OSB->drawRect(nullptr, rectChk.Grow(-1), g_btnStatesColors[1][1]);
                g_pFont->draw<OLeft>(pCheckBox->textComponent.text, rectOutter.Left(20), g_fontColor);
                break;
            case onut::eUIState::HOVER:
                OSB->drawRect(nullptr, rectChk, g_btnStatesColors[2][0]);
                OSB->drawRect(nullptr, rectChk.Grow(-1), g_btnStatesColors[2][1]);
                g_pFont->draw<OLeft>(pCheckBox->textComponent.text, rectOutter.Left(20), g_fontColor);
                break;
            case onut::eUIState::DOWN:
                OSB->drawRect(nullptr, rectChk, g_btnStatesColors[3][0]);
                OSB->drawRect(nullptr, rectChk.Grow(-1), g_btnStatesColors[3][1]);
                g_pFont->draw<OLeft>(pCheckBox->textComponent.text, rectOutter.Left(20) + Vector2(1, 1), g_fontColor);
                break;
        }
        if (pCheckBox->getIsChecked())
        {
            OSB->drawRect(nullptr, rectChk.Grow(-2), g_fontColor);
        }
    });

    pContext->addStyle<onut::UICheckBox>("align", [pContext](const onut::UICheckBox* pCheckBox, const onut::sUIRect& rect)
    {
        const auto orect = onut::UI2Onut(rect);
        auto texture = OGetTexture("align.png");
        Vector4 UVs;
        auto textureSizeX = static_cast<float>(texture->getSize().x);
        auto textureSizeY = static_cast<float>(texture->getSize().y);
        switch (pCheckBox->align)
        {
            case onut::eUIAlign::TOP_LEFT:
                UVs.x = 0.f;
                UVs.z = 13.f / textureSizeX;
                UVs.y = 0.f;
                UVs.w = 13.f / textureSizeY;
                break;
            case onut::eUIAlign::TOP:
                UVs.x = 13.f / textureSizeX;
                UVs.z = 26.f / textureSizeX;
                UVs.y = 0.f;
                UVs.w = 13.f / textureSizeY;
                break;
            case onut::eUIAlign::TOP_RIGHT:
                UVs.x = 26.f / textureSizeX;
                UVs.z = 39.f / textureSizeX;
                UVs.y = 0.f;
                UVs.w = 13.f / textureSizeY;
                break;
            case onut::eUIAlign::LEFT:
                UVs.x = 0.f;
                UVs.z = 13.f / textureSizeX;
                UVs.y = 13.f / textureSizeY;
                UVs.w = 26.f / textureSizeY;
                break;
            case onut::eUIAlign::CENTER:    
                UVs.x = 13.f / textureSizeX;
                UVs.z = 26.f / textureSizeX;
                UVs.y = 13.f / textureSizeY;
                UVs.w = 26.f / textureSizeY;
                break;
            case onut::eUIAlign::RIGHT:
                UVs.x = 26.f / textureSizeX;
                UVs.z = 39.f / textureSizeX;
                UVs.y = 13.f / textureSizeY;
                UVs.w = 26.f / textureSizeY;
                break;
            case onut::eUIAlign::BOTTOM_LEFT:
                UVs.x = 0.f;
                UVs.z = 13.f / textureSizeX;
                UVs.y = 26.f / textureSizeY;
                UVs.w = 39.f / textureSizeY;
                break;
            case onut::eUIAlign::BOTTOM:
                UVs.x = 13.f / textureSizeX;
                UVs.z = 26.f / textureSizeX;
                UVs.y = 26.f / textureSizeY;
                UVs.w = 39.f / textureSizeY;
                break;
            case onut::eUIAlign::BOTTOM_RIGHT:
                UVs.x = 26.f / textureSizeX;
                UVs.z = 39.f / textureSizeX;
                UVs.y = 26.f / textureSizeY;
                UVs.w = 39.f / textureSizeY;
                break;
        }
        if (pCheckBox->getIsChecked())
        {
            UVs.x += 39.f / textureSizeX;
            UVs.z += 39.f / textureSizeX;
            UVs.y += 39.f / textureSizeY;
            UVs.w += 39.f / textureSizeY;
        }
        else
        {
            auto state = pCheckBox->getState(*pContext);
            switch (state)
            {
                case onut::eUIState::HOVER:
                    UVs.x += 39.f / textureSizeX;
                    UVs.z += 39.f / textureSizeX;
                    break;
                case onut::eUIState::DOWN:
                    UVs.y += 39.f / textureSizeY;
                    UVs.w += 39.f / textureSizeY;
                    break;
            }
        }
        OSB->drawRectWithUVs(texture, orect, UVs);
    });

    pContext->addStyle<onut::UIButton>("align", [pContext](const onut::UIButton* pButton, const onut::sUIRect& rect)
    {
        const auto orect = onut::UI2Onut(rect);
        auto texture = OGetTexture("align.png");
        Vector4 UVs;
        switch (pButton->align)
        {
            case onut::eUIAlign::TOP_LEFT:
                UVs.x = 0.f;
                UVs.z = 13.f / texture->getSizef().x;
                UVs.y = 0.f;
                UVs.w = 13.f / texture->getSizef().y;
                break;
            case onut::eUIAlign::TOP:
                UVs.x = 13.f / texture->getSizef().x;
                UVs.z = 26.f / texture->getSizef().x;
                UVs.y = 0.f;
                UVs.w = 13.f / texture->getSizef().y;
                break;
            case onut::eUIAlign::TOP_RIGHT:
                UVs.x = 26.f / texture->getSizef().x;
                UVs.z = 39.f / texture->getSizef().x;
                UVs.y = 0.f;
                UVs.w = 13.f / texture->getSizef().y;
                break;
            case onut::eUIAlign::LEFT:
                UVs.x = 0.f;
                UVs.z = 13.f / texture->getSizef().x;
                UVs.y = 13.f / texture->getSizef().y;
                UVs.w = 26.f / texture->getSizef().y;
                break;
            case onut::eUIAlign::CENTER:    
                UVs.x = 13.f / texture->getSizef().x;
                UVs.z = 26.f / texture->getSizef().x;
                UVs.y = 13.f / texture->getSizef().y;
                UVs.w = 26.f / texture->getSizef().y;
                break;
            case onut::eUIAlign::RIGHT:
                UVs.x = 26.f / texture->getSizef().x;
                UVs.z = 39.f / texture->getSizef().x;
                UVs.y = 13.f / texture->getSizef().y;
                UVs.w = 26.f / texture->getSizef().y;
                break;
            case onut::eUIAlign::BOTTOM_LEFT:
                UVs.x = 0.f;
                UVs.z = 13.f / texture->getSizef().x;
                UVs.y = 26.f / texture->getSizef().y;
                UVs.w = 39.f / texture->getSizef().y;
                break;
            case onut::eUIAlign::BOTTOM:
                UVs.x = 13.f / texture->getSizef().x;
                UVs.z = 26.f / texture->getSizef().x;
                UVs.y = 26.f / texture->getSizef().y;
                UVs.w = 39.f / texture->getSizef().y;
                break;
            case onut::eUIAlign::BOTTOM_RIGHT:
                UVs.x = 26.f / texture->getSizef().x;
                UVs.z = 39.f / texture->getSizef().x;
                UVs.y = 26.f / texture->getSizef().y;
                UVs.w = 39.f / texture->getSizef().y;
                break;
        }
        auto state = pButton->getState(*pContext);
        switch (state)
        {
            case onut::eUIState::HOVER:
                UVs.x += 39.f / texture->getSizef().x;
                UVs.z += 39.f / texture->getSizef().x;
                break;
            case onut::eUIState::DOWN:
                UVs.y += 39.f / texture->getSizef().y;
                UVs.w += 39.f / texture->getSizef().y;
                break;
        }
        OSB->drawRectWithUVs(texture, orect, UVs);
    });

    pContext->addTextCaretSolver<onut::UITextBox>("", [pContext](const onut::UITextBox* pTextBox, const onut::sUIVector2& localPos) -> decltype(std::string().size())
    {
        auto& text = pTextBox->textComponent.text;
        return g_pFont->caretPos(text, localPos.x - 4);
    });

    pContext->addStyle<onut::UITextBox>("", [pContext](const onut::UITextBox* pTextBox, const onut::sUIRect& rect)
    {
        auto state = pTextBox->getState(*pContext);
        const auto rectOutter = onut::UI2Onut(rect);
        const auto rectInnuer = rectOutter.Grow(-1);
        auto hasFocus = pTextBox->hasFocus(*pContext);
        switch (state)
        {
            case onut::eUIState::DISABLED:
                OSB->drawRect(nullptr, rectOutter, g_btnStatesColors[0][0]);
                OSB->drawRect(nullptr, rectInnuer, hasFocus ? g_btnStatesColors[3][1] : g_btnStatesColors[0][1]);
                break;
            case onut::eUIState::NORMAL:
                OSB->drawRect(nullptr, rectOutter, g_btnStatesColors[1][0]);
                OSB->drawRect(nullptr, rectInnuer, hasFocus ? g_btnStatesColors[3][1] : g_btnStatesColors[1][1]);
                break;
            case onut::eUIState::HOVER:
                OSB->drawRect(nullptr, rectOutter, g_btnStatesColors[2][0]);
                OSB->drawRect(nullptr, rectInnuer, hasFocus ? g_btnStatesColors[3][1] : g_btnStatesColors[2][1]);
                break;
            case onut::eUIState::DOWN:
                OSB->drawRect(nullptr, rectOutter, g_btnStatesColors[3][0]);
                OSB->drawRect(nullptr, rectInnuer, hasFocus ? g_btnStatesColors[3][1] : g_btnStatesColors[3][1]);
                break;
        }
        if (hasFocus)
        {
            auto selectedRegion = pTextBox->getSelectedTextRegion();

            auto& text = pTextBox->textComponent.text;
            auto textBefore = text.substr(0, selectedRegion[0]);
            auto regionLen = selectedRegion[1] - selectedRegion[0];
            auto regionText = text.substr(selectedRegion[0], regionLen);
            auto textAfter = text.substr(selectedRegion[1]);

            auto beforeSize = g_pFont->measure(textBefore);
            auto afterSize = g_pFont->measure(textAfter);
            
            auto regionSize = g_pFont->measure(regionText);
            auto regionPos = g_pFont->measure(textBefore);
            Rect regionRect{rectOutter.Left(4), regionSize};
            regionRect.x += regionPos.x;
            regionRect.y -= regionSize.y * .5f;
            ++regionRect.z;

            if (regionLen)
            {
                OSB->drawRect(nullptr, regionRect, g_treeItemSelectedBGColor);

                g_pFont->draw<OLeft>(textBefore, rectInnuer.Left(4), g_fontColor);
                g_pFont->draw<OLeft>(regionText, rectInnuer.Left(4 + beforeSize.x), g_selectedTextColor);
                g_pFont->draw<OLeft>(textAfter, rectInnuer.Left(4 + beforeSize.x + regionSize.x), g_fontColor);
            }
            else
            {
                g_pFont->draw<OLeft>(text, rectInnuer.Left(4), g_fontColor);
            }

            // Draw cursor
            if (pTextBox->isCursorVisible())
            {
                auto color = g_cursorColor;
                if (selectedRegion[1] - selectedRegion[0] > 0) color = g_cursorSelectionColor;

                auto textToCursor = text.substr(0, pTextBox->getCursorPos());
                auto cursorPos = g_pFont->measure(textToCursor);
                auto left = rectInnuer.Left(4);

                OSB->drawRect(nullptr, {left.x + cursorPos.x, regionRect.y, 1, cursorPos.y}, color);
            }
        }
        else
        {
            if (state == onut::eUIState::DISABLED)
            {
                g_pFont->draw<OLeft>(pTextBox->textComponent.text, rectInnuer.Left(4), g_fontColor * .5f);
            }
            else
            {
                g_pFont->draw<OLeft>(pTextBox->textComponent.text, rectInnuer.Left(4), g_fontColor);
            }
        }
    });
}
