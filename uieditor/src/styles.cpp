#include "DocumentView.h"
#include "styles.h"

#include "onut/Anim.h"
#include "onut/Font.h"
#include "onut/SpriteBatch.h"
#include "onut/Texture.h"
#include "onut/UIButton.h"
#include "onut/UICheckBox.h"
#include "onut/UILabel.h"
#include "onut/UIImage.h"
#include "onut/UIPanel.h"
#include "onut/UITextBox.h"
#include "onut/UITreeView.h"

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

static OFontRef g_pFont;
static OTextureRef g_pTexTreeOpen;
static OTextureRef g_pTexTreeClose;

static OTextureRef g_pTexIcoUIControl;
static OTextureRef g_pTexIcoUIPanel;
static OTextureRef g_pTexIcoUIButton;
static OTextureRef g_pTexIcoUILabel;
static OTextureRef g_pTexIcoUIImage;
static OTextureRef g_pTexIcoUICheckbox;
static OTextureRef g_pTexIcoUITextBox;

extern DocumentView* g_pDocument;
OAnimFloat g_dottedLineAnim = 0.f;

void createUIStyles(const OUIContextRef& pContext)
{
    g_pFont = OGetFont("segeo12.fnt");
    g_pTexTreeOpen = OGetTexture("viewItemOpen.png");
    g_pTexTreeClose = OGetTexture("viewItemClose.png");
    g_pTexIcoUIControl = OGetTexture("shape_handles.png");
    g_pTexIcoUIPanel = OGetTexture("shape_square.png");
    g_pTexIcoUIButton = OGetTexture("icoButton.png");
    g_pTexIcoUILabel = OGetTexture("text_dropcaps.png");
    g_pTexIcoUIImage = OGetTexture("picture.png");
    g_pTexIcoUICheckbox = OGetTexture("accept.png");
    g_pTexIcoUITextBox = OGetTexture("textfield.png");
    g_dottedLineAnim.play(0.f, -1.f, .5f, OTweenLinear, OLoop);

    pContext->addStyle<OUIPanel>("", [](const OUIPanelRef& pPanel, const Rect& rect)
    {
        oSpriteBatch->drawRect(nullptr, (rect), g_panelBGColor);
    });

    pContext->addStyle<OUIPanel>("darken", [](const OUIPanelRef& pPanel, const Rect& rect)
    {
        oSpriteBatch->drawRect(nullptr, (rect), g_panelDarkenBGColor);
    });

    pContext->addStyle<OUIPanel>("colorPicker", [pContext](const OUIPanelRef& pPanel, const Rect& rect)
    {
        auto& color = pPanel->color;

        auto state = pPanel->getState(pContext);
        const auto rectOutter = (rect);
        const auto rectInnuer = rectOutter.Grow(-1);
        const auto rectColor = rectOutter.Grow(-4);
        switch (state)
        {
            case OUIControl::State::Disabled:
                oSpriteBatch->drawRect(nullptr, rectOutter, g_btnStatesColors[0][0]);
                oSpriteBatch->drawRect(nullptr, rectInnuer, g_btnStatesColors[0][1]);
                break;
            case OUIControl::State::Normal:
                oSpriteBatch->drawRect(nullptr, rectOutter, g_btnStatesColors[1][0]);
                oSpriteBatch->drawRect(nullptr, rectInnuer, g_btnStatesColors[1][1]);
                break;
            case OUIControl::State::Hover:
                oSpriteBatch->drawRect(nullptr, rectOutter, g_btnStatesColors[2][0]);
                oSpriteBatch->drawRect(nullptr, rectInnuer, g_btnStatesColors[2][1]);
                break;
            case OUIControl::State::Down:
                oSpriteBatch->drawRect(nullptr, rectOutter, g_btnStatesColors[3][0]);
                oSpriteBatch->drawRect(nullptr, rectInnuer, g_btnStatesColors[3][1]);
                break;
        }

        oSpriteBatch->drawRect(nullptr, rectColor, color);
    });

    pContext->addStyle<OUIPanel>("guide", [](const OUIPanelRef& pPanel, const Rect& rect)
    {
        oSpriteBatch->drawRect(nullptr, (rect), g_guideColor);
    });

    pContext->addStyle<OUIPanel>("group", [](const OUIPanelRef& pPanel, const Rect& rect)
    {
        auto orect = (rect);
        oSpriteBatch->drawRect(nullptr, orect, g_groupOutlineColor);
        oSpriteBatch->drawRect(nullptr, orect.Grow(-1), g_panelBGColor);
    });

    pContext->addStyle<OUITreeView>("", [](const OUITreeViewRef& pPanel, const Rect& rect)
    {
        auto orect = (rect);
        oSpriteBatch->drawRect(nullptr, orect.Grow(1), g_deepViewOutline);
        oSpriteBatch->drawRect(nullptr, orect, g_deepViewFill);
    });

    pContext->addStyle<OUITreeViewItem>("", [pContext](const OUITreeViewItemRef& pItem, const Rect& rect)
    {
        auto pTreeView = pItem->getTreeView();
        auto orect = (rect);
        if (pItem->getIsSelected())
        {
            auto selectionRect = orect;
            auto treeViewWorldRect = pTreeView->getWorldRect(pContext);
            selectionRect.x = treeViewWorldRect.x;
            selectionRect.z = treeViewWorldRect.z;
            oSpriteBatch->drawRect(nullptr, selectionRect, g_treeItemSelectedBGColor);
        }
        auto expandClickWidth = pTreeView->expandClickWidth;

        // Expand icon
        if (!pItem->getItems().empty())
        {
            if (pItem->isExpanded)
            {
                oSpriteBatch->drawSprite(g_pTexTreeOpen, orect.Left(expandClickWidth * .5f + 4));
            }
            else
            {
                oSpriteBatch->drawSprite(g_pTexTreeClose, orect.Left(expandClickWidth * .5f + 4));
            }
        }

        const auto& text = pItem->text;
        bool hasText = !text.empty();
        auto pControl = OStaticCast<OUIControl>(pItem->pSharedUserData);
        auto textPos = orect.Left(expandClickWidth + 24);
        auto textColor = g_fontColor;
        if (!pControl->isEnabled)
        {
            textColor = g_fontColorFaded;
        }

        // UI icon
        switch (pControl->getType())
        {
            case OUIControl::Type::Control:
                oSpriteBatch->drawSprite(g_pTexIcoUIControl, orect.Left(expandClickWidth + 12));
                if (!hasText)
                {
                    static const std::string uiText = "UIControl";
                    g_pFont->draw(uiText, textPos, OLeft, textColor);
                }
                break;
            case OUIControl::Type::Panel:
                oSpriteBatch->drawSprite(g_pTexIcoUIPanel, orect.Left(expandClickWidth + 12));
                if (!hasText)
                {
                    static const std::string uiText = "UIPanel";
                    g_pFont->draw(uiText, textPos, OLeft, textColor);
                }
                break;
            case OUIControl::Type::Button:
                oSpriteBatch->drawSprite(g_pTexIcoUIButton, orect.Left(expandClickWidth + 12));
                if (!hasText)
                {
                    static const std::string uiText = "UIButton";
                    g_pFont->draw(uiText, textPos, OLeft, textColor);
                }
                break;
            case OUIControl::Type::Label:
                oSpriteBatch->drawSprite(g_pTexIcoUILabel, orect.Left(expandClickWidth + 12));
                if (!hasText)
                {
                    static const std::string uiText = "UILabel";
                    g_pFont->draw(uiText, textPos, OLeft, textColor);
                }
                break;
            case OUIControl::Type::Image:
                oSpriteBatch->drawSprite(g_pTexIcoUIImage, orect.Left(expandClickWidth + 12));
                if (!hasText)
                {
                    static const std::string uiText = "UIImage";
                    g_pFont->draw(uiText, textPos, OLeft, textColor);
                }
                break;
            case OUIControl::Type::CheckBox:
                oSpriteBatch->drawSprite(g_pTexIcoUICheckbox, orect.Left(expandClickWidth + 12));
                if (!hasText)
                {
                    static const std::string uiText = "UICheckBox";
                    g_pFont->draw(uiText, textPos, OLeft, textColor);
                }
                break;
            case OUIControl::Type::TextBox:
                oSpriteBatch->drawSprite(g_pTexIcoUITextBox, orect.Left(expandClickWidth + 12));
                if (!hasText)
                {
                    static const std::string uiText = "UITextBox";
                    g_pFont->draw(uiText, textPos, OLeft, textColor);
                }
                break;
        }

        // Label
        if (hasText)
        {
            g_pFont->draw(pItem->text, textPos, OLeft, g_fontColor);
        }
    });

    pContext->addStyle<OUIImage>("", [](const OUIImageRef& pImage, const Rect& rect)
    {
        oSpriteBatch->drawRect(OGetTexture(pImage->scale9Component.image.filename.c_str()), (rect));
    });

    pContext->addStyle<OUIImage>("toolBtn", [pContext](const OUIImageRef& pImage, const Rect& rect)
    {
        auto orect = (rect);
        auto state = pImage->getState(pContext);
        auto pos = orect.Center();
        pos.x = std::roundf(pos.x);
        pos.y = std::roundf(pos.y);
        switch (state)
        {
            case OUIControl::State::Normal:
                oSpriteBatch->drawSprite(OGetTexture(pImage->scale9Component.image.filename.c_str()), pos);
                break;
            case OUIControl::State::Hover:
                oSpriteBatch->drawRect(nullptr, orect, g_toolBtnHoverColor);
                oSpriteBatch->drawSprite(OGetTexture(pImage->scale9Component.image.filename.c_str()), pos);
                break;
            case OUIControl::State::Down:
                oSpriteBatch->drawRect(nullptr, orect, g_toolBtnDownColor);
                oSpriteBatch->drawSprite(OGetTexture(pImage->scale9Component.image.filename.c_str()), pos);
                break;
            case OUIControl::State::Disabled:
                oSpriteBatch->drawSprite(OGetTexture(pImage->scale9Component.image.filename.c_str()), pos, g_toolBtnDisabledColor);
                break;
        }
    });
    
    pContext->addStyle<OUIPanel>("toolSeparator", [](const OUIPanelRef& pPanel, const Rect& rect)
    {
        auto orect = (rect);
        auto pos = orect.Center();
        pos.x = std::roundf(pos.x);
        pos.y = std::roundf(pos.y);
        oSpriteBatch->drawSprite(OGetTexture("toolSeparator.png"), pos);
    });

    pContext->addStyle<OUIPanel>("view", [](const OUIPanelRef& pPanel, const Rect& rect)
    {
    });

    pContext->addStyle<OUIPanel>("sizeHandle", [](const OUIPanelRef& pLabel, const Rect& rect)
    {
        const auto orect = (rect);
        oSpriteBatch->drawRect(nullptr, orect, g_sizeHandleColor);
    });

    pContext->addStyle<OUILabel>("", [](const OUILabelRef& pLabel, const Rect& rect)
    {
        g_pFont->draw(pLabel->textComponent.text, (rect).Left(), OLeft, g_fontColor);
    });

    pContext->addStyle<OUILabel>("panelTitle", [](const OUILabelRef& pLabel, const Rect& rect)
    {
        const auto orect = (rect);
        oSpriteBatch->drawRect(nullptr, orect, g_panelTitleBGColor);
        g_pFont->draw(pLabel->textComponent.text, orect.Center(), OCenter, g_fontColor);
    });

    pContext->addStyle<OUIButton>("", [pContext](const OUIButtonRef& pButton, const Rect& rect)
    {
        auto state = pButton->getState(pContext);
        const auto rectOutter = (rect);
        const auto rectInnuer = rectOutter.Grow(-1);
        switch (state)
        {
            case OUIControl::State::Disabled:
                oSpriteBatch->drawRect(nullptr, rectOutter, g_btnStatesColors[0][0]);
                oSpriteBatch->drawRect(nullptr, rectInnuer, g_btnStatesColors[0][1]);
                g_pFont->draw(pButton->textComponent.text, rectInnuer.Center(), OCenter, g_fontColor * .5f);
                break;
            case OUIControl::State::Normal:
                oSpriteBatch->drawRect(nullptr, rectOutter, g_btnStatesColors[1][0]);
                oSpriteBatch->drawRect(nullptr, rectInnuer, g_btnStatesColors[1][1]);
                g_pFont->draw(pButton->textComponent.text, rectInnuer.Center(), OCenter, g_fontColor);
                break;
            case OUIControl::State::Hover:
                oSpriteBatch->drawRect(nullptr, rectOutter, g_btnStatesColors[2][0]);
                oSpriteBatch->drawRect(nullptr, rectInnuer, g_btnStatesColors[2][1]);
                g_pFont->draw(pButton->textComponent.text, rectInnuer.Center(), OCenter, g_fontColor);
                break;
            case OUIControl::State::Down:
                oSpriteBatch->drawRect(nullptr, rectOutter, g_btnStatesColors[3][0]);
                oSpriteBatch->drawRect(nullptr, rectInnuer, g_btnStatesColors[3][1]);
                g_pFont->draw(pButton->textComponent.text, rectInnuer.Center() + Vector2(1, 1), OCenter, g_fontColor);
                break;
        }
    });

    pContext->addStyle<OUIButton>("imgButton", [pContext](const OUIButtonRef& pButton, const Rect& rect)
    {
        auto state = pButton->getState(pContext);
        const auto rectOutter = (rect);
        const auto rectInnuer = rectOutter.Grow(-1);
        auto textRect = rectInnuer;
        textRect.x += 24;
        textRect.z -= 24;
        switch (state)
        {
            case OUIControl::State::Disabled:
                oSpriteBatch->drawRect(nullptr, rectOutter, g_btnStatesColors[0][0]);
                oSpriteBatch->drawRect(nullptr, rectInnuer, g_btnStatesColors[0][1]);
                g_pFont->draw(pButton->textComponent.text, textRect.Left(), OLeft, g_fontColor * .5f);
                break;
            case OUIControl::State::Normal:
                oSpriteBatch->drawRect(nullptr, rectOutter, g_btnStatesColors[1][0]);
                oSpriteBatch->drawRect(nullptr, rectInnuer, g_btnStatesColors[1][1]);
                g_pFont->draw(pButton->textComponent.text, textRect.Left(), OLeft, g_fontColor);
                break;
            case OUIControl::State::Hover:
                oSpriteBatch->drawRect(nullptr, rectOutter, g_btnStatesColors[2][0]);
                oSpriteBatch->drawRect(nullptr, rectInnuer, g_btnStatesColors[2][1]);
                g_pFont->draw(pButton->textComponent.text, textRect.Left(), OLeft, g_fontColor);
                break;
            case OUIControl::State::Down:
                oSpriteBatch->drawRect(nullptr, rectOutter, g_btnStatesColors[3][0]);
                oSpriteBatch->drawRect(nullptr, rectInnuer, g_btnStatesColors[3][1]);
                g_pFont->draw(pButton->textComponent.text, textRect.Left() + Vector2(1, 1), OLeft, g_fontColor);
                break;
        }
    });

    pContext->addStyle<OUIPanel>("gizmo", [](const OUIPanelRef& pPanel, const Rect& rect)
    {
        const Color DOTTED_LINE_COLOR = {1, 1, 1, .5f};

        auto pDottedLineTexture = OGetTexture("dottedLine.png");
        auto dottedLineScale = 1.f / pDottedLineTexture->getSizef().x;
        auto dottedLineOffset = g_dottedLineAnim.get();

        oSpriteBatch->drawRectWithUVs(pDottedLineTexture,
        {rect.x, rect.y, 1, rect.w},
        {dottedLineOffset, dottedLineOffset, dottedLineOffset + dottedLineScale, dottedLineOffset + rect.w * dottedLineScale},
        DOTTED_LINE_COLOR);

        oSpriteBatch->drawRectWithUVs(pDottedLineTexture,
        {rect.x + rect.z - 1, rect.y, 1, rect.w},
        {dottedLineOffset, dottedLineOffset, dottedLineOffset + dottedLineScale, dottedLineOffset + rect.w * dottedLineScale},
        DOTTED_LINE_COLOR);

        oSpriteBatch->drawRectWithUVs(pDottedLineTexture,
        {rect.x, rect.y, rect.z, 1},
        {dottedLineOffset, dottedLineOffset, dottedLineOffset + rect.z * dottedLineScale, dottedLineOffset + dottedLineScale},
        DOTTED_LINE_COLOR);

        oSpriteBatch->drawRectWithUVs(pDottedLineTexture,
        {rect.x, rect.y + rect.w - 1, rect.z, 1},
        {dottedLineOffset, dottedLineOffset, dottedLineOffset + rect.z * dottedLineScale, dottedLineOffset + dottedLineScale},
        DOTTED_LINE_COLOR);
    });

    pContext->addStyle<OUICheckBox>("", [pContext](const OUICheckBoxRef& pCheckBox, const Rect& rect)
    {
        auto state = pCheckBox->getState(pContext);
        const auto rectOutter = (rect);
        auto rectChk = rectOutter;
        rectChk.x += 4;
        rectChk.y = std::floorf(rectChk.y + rectChk.w / 2 - 6);
        rectChk.z = 12;
        rectChk.w = 12;
        switch (state)
        {
            case OUIControl::State::Disabled:
                oSpriteBatch->drawRect(nullptr, rectChk, g_btnStatesColors[0][0]);
                oSpriteBatch->drawRect(nullptr, rectChk.Grow(-1), g_btnStatesColors[0][1]);
                g_pFont->draw(pCheckBox->textComponent.text, rectOutter.Left(20), OLeft, g_fontColor * .5f);
                break;
            case OUIControl::State::Normal:
                oSpriteBatch->drawRect(nullptr, rectChk, g_btnStatesColors[1][0]);
                oSpriteBatch->drawRect(nullptr, rectChk.Grow(-1), g_btnStatesColors[1][1]);
                g_pFont->draw(pCheckBox->textComponent.text, rectOutter.Left(20), OLeft, g_fontColor);
                break;
            case OUIControl::State::Hover:
                oSpriteBatch->drawRect(nullptr, rectChk, g_btnStatesColors[2][0]);
                oSpriteBatch->drawRect(nullptr, rectChk.Grow(-1), g_btnStatesColors[2][1]);
                g_pFont->draw(pCheckBox->textComponent.text, rectOutter.Left(20), OLeft, g_fontColor);
                break;
            case OUIControl::State::Down:
                oSpriteBatch->drawRect(nullptr, rectChk, g_btnStatesColors[3][0]);
                oSpriteBatch->drawRect(nullptr, rectChk.Grow(-1), g_btnStatesColors[3][1]);
                g_pFont->draw(pCheckBox->textComponent.text, rectOutter.Left(20) + Vector2(1, 1), OLeft, g_fontColor);
                break;
        }
        if (pCheckBox->getIsChecked())
        {
            oSpriteBatch->drawRect(nullptr, rectChk.Grow(-2), g_fontColor);
        }
    });

    pContext->addStyle<OUICheckBox>("align", [pContext](const OUICheckBoxRef& pCheckBox, const Rect& rect)
    {
        const auto orect = (rect);
        auto texture = OGetTexture("align.png");
        Vector4 UVs;
        auto textureSizeX = static_cast<float>(texture->getSize().x);
        auto textureSizeY = static_cast<float>(texture->getSize().y);
        switch (pCheckBox->align)
        {
            case OTopLeft:
                UVs.x = 0.f;
                UVs.z = 13.f / textureSizeX;
                UVs.y = 0.f;
                UVs.w = 13.f / textureSizeY;
                break;
            case OTop:
                UVs.x = 13.f / textureSizeX;
                UVs.z = 26.f / textureSizeX;
                UVs.y = 0.f;
                UVs.w = 13.f / textureSizeY;
                break;
            case OTopRight:
                UVs.x = 26.f / textureSizeX;
                UVs.z = 39.f / textureSizeX;
                UVs.y = 0.f;
                UVs.w = 13.f / textureSizeY;
                break;
            case OLeft:
                UVs.x = 0.f;
                UVs.z = 13.f / textureSizeX;
                UVs.y = 13.f / textureSizeY;
                UVs.w = 26.f / textureSizeY;
                break;
            case OCenter:    
                UVs.x = 13.f / textureSizeX;
                UVs.z = 26.f / textureSizeX;
                UVs.y = 13.f / textureSizeY;
                UVs.w = 26.f / textureSizeY;
                break;
            case ORight:
                UVs.x = 26.f / textureSizeX;
                UVs.z = 39.f / textureSizeX;
                UVs.y = 13.f / textureSizeY;
                UVs.w = 26.f / textureSizeY;
                break;
            case OBottomLeft:
                UVs.x = 0.f;
                UVs.z = 13.f / textureSizeX;
                UVs.y = 26.f / textureSizeY;
                UVs.w = 39.f / textureSizeY;
                break;
            case OBottom:
                UVs.x = 13.f / textureSizeX;
                UVs.z = 26.f / textureSizeX;
                UVs.y = 26.f / textureSizeY;
                UVs.w = 39.f / textureSizeY;
                break;
            case OBottomRight:
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
            auto state = pCheckBox->getState(pContext);
            switch (state)
            {
                case OUIControl::State::Hover:
                    UVs.x += 39.f / textureSizeX;
                    UVs.z += 39.f / textureSizeX;
                    break;
                case OUIControl::State::Down:
                    UVs.y += 39.f / textureSizeY;
                    UVs.w += 39.f / textureSizeY;
                    break;
            }
        }
        oSpriteBatch->drawRectWithUVs(texture, orect, UVs);
    });

    pContext->addStyle<OUIButton>("align", [pContext](const OUIButtonRef& pButton, const Rect& rect)
    {
        const auto orect = (rect);
        auto texture = OGetTexture("align.png");
        Vector4 UVs;
        switch (pButton->align)
        {
            case OTopLeft:
                UVs.x = 0.f;
                UVs.z = 13.f / texture->getSizef().x;
                UVs.y = 0.f;
                UVs.w = 13.f / texture->getSizef().y;
                break;
            case OTop:
                UVs.x = 13.f / texture->getSizef().x;
                UVs.z = 26.f / texture->getSizef().x;
                UVs.y = 0.f;
                UVs.w = 13.f / texture->getSizef().y;
                break;
            case OTopRight:
                UVs.x = 26.f / texture->getSizef().x;
                UVs.z = 39.f / texture->getSizef().x;
                UVs.y = 0.f;
                UVs.w = 13.f / texture->getSizef().y;
                break;
            case OLeft:
                UVs.x = 0.f;
                UVs.z = 13.f / texture->getSizef().x;
                UVs.y = 13.f / texture->getSizef().y;
                UVs.w = 26.f / texture->getSizef().y;
                break;
            case OCenter:    
                UVs.x = 13.f / texture->getSizef().x;
                UVs.z = 26.f / texture->getSizef().x;
                UVs.y = 13.f / texture->getSizef().y;
                UVs.w = 26.f / texture->getSizef().y;
                break;
            case ORight:
                UVs.x = 26.f / texture->getSizef().x;
                UVs.z = 39.f / texture->getSizef().x;
                UVs.y = 13.f / texture->getSizef().y;
                UVs.w = 26.f / texture->getSizef().y;
                break;
            case OBottomLeft:
                UVs.x = 0.f;
                UVs.z = 13.f / texture->getSizef().x;
                UVs.y = 26.f / texture->getSizef().y;
                UVs.w = 39.f / texture->getSizef().y;
                break;
            case OBottom:
                UVs.x = 13.f / texture->getSizef().x;
                UVs.z = 26.f / texture->getSizef().x;
                UVs.y = 26.f / texture->getSizef().y;
                UVs.w = 39.f / texture->getSizef().y;
                break;
            case OBottomRight:
                UVs.x = 26.f / texture->getSizef().x;
                UVs.z = 39.f / texture->getSizef().x;
                UVs.y = 26.f / texture->getSizef().y;
                UVs.w = 39.f / texture->getSizef().y;
                break;
        }
        auto state = pButton->getState(pContext);
        switch (state)
        {
            case OUIControl::State::Hover:
                UVs.x += 39.f / texture->getSizef().x;
                UVs.z += 39.f / texture->getSizef().x;
                break;
            case OUIControl::State::Down:
                UVs.y += 39.f / texture->getSizef().y;
                UVs.w += 39.f / texture->getSizef().y;
                break;
        }
        oSpriteBatch->drawRectWithUVs(texture, orect, UVs);
    });

    pContext->addTextCaretSolver<OUITextBox>("", [pContext](const OUITextBoxRef& pTextBox, const Vector2& localPos) -> decltype(std::string().size())
    {
        auto& text = pTextBox->textComponent.text;
        return g_pFont->caretPos(text, localPos.x - 4);
    });

    pContext->addStyle<OUITextBox>("", [pContext](const OUITextBoxRef& pTextBox, const Rect& rect)
    {
        auto state = pTextBox->getState(pContext);
        const auto rectOutter = (rect);
        const auto rectInnuer = rectOutter.Grow(-1);
        auto hasFocus = pTextBox->hasFocus(pContext);
        switch (state)
        {
            case OUIControl::State::Disabled:
                oSpriteBatch->drawRect(nullptr, rectOutter, g_btnStatesColors[0][0]);
                oSpriteBatch->drawRect(nullptr, rectInnuer, hasFocus ? g_btnStatesColors[3][1] : g_btnStatesColors[0][1]);
                break;
            case OUIControl::State::Normal:
                oSpriteBatch->drawRect(nullptr, rectOutter, g_btnStatesColors[1][0]);
                oSpriteBatch->drawRect(nullptr, rectInnuer, hasFocus ? g_btnStatesColors[3][1] : g_btnStatesColors[1][1]);
                break;
            case OUIControl::State::Hover:
                oSpriteBatch->drawRect(nullptr, rectOutter, g_btnStatesColors[2][0]);
                oSpriteBatch->drawRect(nullptr, rectInnuer, hasFocus ? g_btnStatesColors[3][1] : g_btnStatesColors[2][1]);
                break;
            case OUIControl::State::Down:
                oSpriteBatch->drawRect(nullptr, rectOutter, g_btnStatesColors[3][0]);
                oSpriteBatch->drawRect(nullptr, rectInnuer, hasFocus ? g_btnStatesColors[3][1] : g_btnStatesColors[3][1]);
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
                oSpriteBatch->drawRect(nullptr, regionRect, g_treeItemSelectedBGColor);

                g_pFont->draw(textBefore, rectInnuer.Left(4), OLeft, g_fontColor);
                g_pFont->draw(regionText, rectInnuer.Left(4 + beforeSize.x), OLeft, g_selectedTextColor);
                g_pFont->draw(textAfter, rectInnuer.Left(4 + beforeSize.x + regionSize.x), OLeft, g_fontColor);
            }
            else
            {
                g_pFont->draw(text, rectInnuer.Left(4), OLeft, g_fontColor);
            }

            // Draw cursor
            if (pTextBox->isCursorVisible())
            {
                auto color = g_cursorColor;
                if (selectedRegion[1] - selectedRegion[0] > 0) color = g_cursorSelectionColor;

                auto textToCursor = text.substr(0, pTextBox->getCursorPos());
                auto cursorPos = g_pFont->measure(textToCursor);
                auto left = rectInnuer.Left(4);

                oSpriteBatch->drawRect(nullptr, {left.x + cursorPos.x, regionRect.y, 1, cursorPos.y}, color);
            }
        }
        else
        {
            if (state == OUIControl::State::Disabled)
            {
                g_pFont->draw(pTextBox->textComponent.text, rectInnuer.Left(4), OLeft, g_fontColor * .5f);
            }
            else
            {
                g_pFont->draw(pTextBox->textComponent.text, rectInnuer.Left(4), OLeft, g_fontColor);
            }
        }
    });
}
