#include <onut/Anim.h>
#include <onut/Files.h>
#include <onut/Font.h>
#include <onut/Renderer.h>
#include <onut/Settings.h>
#include <onut/SpriteBatch.h>
#include <onut/UIButton.h>
#include <onut/UIContext.h>
#include <onut/UIControl.h>
#include <onut/UIImage.h>
#include <onut/UILabel.h>
#include <onut/UITextBox.h>
#include <onut/Window.h>

// Main
OUIControlRef pCtlMain;
OUIButtonRef pBtnCreateGame;
OUIButtonRef pBtnUpdateGame;
OUIImageRef pImgLogo;
OUILabelRef pLblVersion;

// Create Game
OUIControlRef pCtlCreateGame;
OUITextBoxRef pTxtPath;
OUIButtonRef pBtnBrowse;
OUILabelRef pLblFullPath;
OUITextBoxRef pTxtName;
OUIButtonRef pBtnCancel;
OUIButtonRef pBtnCreate;

static const std::string VERSION = "3.0";

Color BG_COLOR = OColorHex(0b0017);
OAnimFloat alphaAnim;
OAnimFloat buttonAnims[2];

static const Color g_cursorColor = OColorHex(dadad9);
static const Color g_cursorSelectionColor = OColorHex(cc6600);
static const Color g_treeItemSelectedBGColor = OColorHex(3399ff);
static const Color g_selectedTextColor = OColorHex(ffffff);

void startAnimations()
{
    oSettings->setIsEditorMode(false);
    alphaAnim.play(1.0f, 0.0f, 2.0f, OTweenEaseOut, ODontLoop, []
    {
        oSettings->setIsEditorMode(true); // We don't need to update/render every frame anymore
    });
    buttonAnims[0].play(pBtnCreateGame->rect.x + 600, pBtnCreateGame->rect.x, 1.0f, OTweenEaseOut);
    buttonAnims[1].play(pBtnUpdateGame->rect.x + 1200, pBtnUpdateGame->rect.x, 1.0f, OTweenEaseOut);
}

void onCreateGame(const OUIControlRef&, const onut::UIMouseEvent&)
{
    pCtlMain->isVisible = false;
    pCtlCreateGame->isVisible = true;
}

void onUpdateGame(const OUIControlRef&, const onut::UIMouseEvent&)
{
}

void onBrowse(const OUIControlRef&, const onut::UIMouseEvent&)
{
}

void onCancel(const OUIControlRef&, const onut::UIMouseEvent&)
{
    pCtlMain->isVisible = true;
    pCtlCreateGame->isVisible = false;
    startAnimations();
}

void onCreate(const OUIControlRef&, const onut::UIMouseEvent&)
{
}

void initSettings()
{
    oSettings->setResolution({ 600, 400 });
    oSettings->setGameName("Oak Nut Setup");
    oSettings->setIsResizableWindow(false);
    oSettings->setIsEditorMode(true);
    oSettings->setShowFPS(true);
    oSettings->setIsFixedStep(false);
}

void init()
{
    oUI->add(OLoadUI("main.json"));

    // Fint UIs
    pCtlMain = OFindUI("ctlMain");
    pBtnCreateGame = ODynamicCast<OUIButton>(OFindUI("btnCreateGame"));
    pBtnUpdateGame = ODynamicCast<OUIButton>(OFindUI("btnUpdateGame"));
    pImgLogo = ODynamicCast<OUIImage>(OFindUI("imgLogo"));
    pLblVersion = ODynamicCast<OUILabel>(OFindUI("lblVersion"));

    pCtlCreateGame = OFindUI("ctlCreateGame");
    pTxtPath = ODynamicCast<OUITextBox>(OFindUI("txtPath"));
    pBtnBrowse = ODynamicCast<OUIButton>(OFindUI("btnBrowse"));
    pLblFullPath = ODynamicCast<OUILabel>(OFindUI("lblFullPath"));
    pTxtName = ODynamicCast<OUITextBox>(OFindUI("txtName"));
    pBtnCancel = ODynamicCast<OUIButton>(OFindUI("btnCancel"));
    pBtnCreate = ODynamicCast<OUIButton>(OFindUI("btnCreate"));

    // Set version properly
    pLblVersion->textComponent.text = "Version " + VERSION;

    // Callbacks
    pBtnCreateGame->onClick = onCreateGame;
    pBtnUpdateGame->onClick = onUpdateGame;
    pBtnBrowse->onClick = onBrowse;
    pBtnCancel->onClick = onCancel;
    pBtnCreate->onClick = onCreate;

    // Intro animations
    startAnimations();

    // So UIs textbox work better
    oUIContext->addTextCaretSolver<OUITextBox>("", [](const OUITextBoxRef& pTextBox, const Vector2& localPos) -> decltype(std::string().size())
    {
        auto& text = pTextBox->textComponent.text;
        auto pFont = OGetFont(pTextBox->textComponent.font.typeFace);
        return pFont->caretPos(text, localPos.x - 4);
    });
    oUIContext->addStyle<OUITextBox>("", [](const OUITextBoxRef& pTextBox, const Rect& rect)
    {
        auto state = pTextBox->getState(oUIContext);
        const auto rectOutter = (rect);
        const auto rectInnuer = rectOutter.Grow(-1);
        auto hasFocus = pTextBox->hasFocus(oUIContext);
        auto pFont = OGetFont(pTextBox->textComponent.font.typeFace);
        auto fontColor = pTextBox->textComponent.font.color;
        oUIContext->drawScale9Rect(pTextBox, rect, pTextBox->scale9Component);
        if (hasFocus)
        {
            auto selectedRegion = pTextBox->getSelectedTextRegion();

            auto& text = pTextBox->textComponent.text;
            auto textBefore = text.substr(0, selectedRegion[0]);
            auto regionLen = selectedRegion[1] - selectedRegion[0];
            auto regionText = text.substr(selectedRegion[0], regionLen);
            auto textAfter = text.substr(selectedRegion[1]);

            auto beforeSize = pFont->measure(textBefore);
            auto afterSize = pFont->measure(textAfter);

            auto regionSize = pFont->measure(regionText);
            auto regionPos = pFont->measure(textBefore);
            Rect regionRect{ rectOutter.Left(4), regionSize };
            regionRect.x += regionPos.x;
            regionRect.y -= regionSize.y * .5f;
            ++regionRect.z;

            if (regionLen)
            {
                oSpriteBatch->drawRect(nullptr, regionRect, g_treeItemSelectedBGColor);

                pFont->draw(textBefore, rectInnuer.Left(4), OLeft, fontColor);
                pFont->draw(regionText, rectInnuer.Left(4 + beforeSize.x), OLeft, g_selectedTextColor);
                pFont->draw(textAfter, rectInnuer.Left(4 + beforeSize.x + regionSize.x), OLeft, fontColor);
            }
            else
            {
                pFont->draw(text, rectInnuer.Left(4), OLeft, fontColor);
            }

            // Draw cursor
            if (pTextBox->isCursorVisible())
            {
                auto color = g_cursorColor;
                if (selectedRegion[1] - selectedRegion[0] > 0) color = g_cursorSelectionColor;

                auto textToCursor = text.substr(0, pTextBox->getCursorPos());
                auto cursorPos = pFont->measure(textToCursor);
                auto left = rectInnuer.Left(4);

                oSpriteBatch->drawRect(nullptr, { left.x + cursorPos.x, regionRect.y, 1, cursorPos.y }, color);
            }
        }
        else
        {
            if (state == OUIControl::State::Disabled)
            {
                pFont->draw(pTextBox->textComponent.text, rectInnuer.Left(4), OLeft, fontColor * .5f);
            }
            else
            {
                pFont->draw(pTextBox->textComponent.text, rectInnuer.Left(4), OLeft, fontColor);
            }
        }
    });
}

void update()
{
    pBtnCreateGame->rect.x = buttonAnims[0].get();
    pBtnUpdateGame->rect.x = buttonAnims[1].get();
}

void render()
{
    // Clear the background to the color of the UI in case it leaks
    oRenderer->clear(BG_COLOR);
}

void postRender()
{
    // Post render renders on top of UIs
    oSpriteBatch->begin();
    oSpriteBatch->drawRect(nullptr, ORectFullScreen, BG_COLOR * alphaAnim.get());
    oSpriteBatch->end();
}
