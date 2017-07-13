#include <onut/Anim.h>
#include <onut/Files.h>
#include <onut/Font.h>
#include <onut/onut.h>
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

#include <fstream>

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

std::string getGameFolderName(const std::string& gameName)
{
    std::string folderName;
    for (const auto& c : gameName)
    {
        if ((c >= 'a' && c <= 'z') ||
            (c >= 'A' && c <= 'Z') ||
            (c >= '0' && c <= '9'))
        {
            folderName += c;
        }
    }
    return folderName;
}

void updateFullPath()
{
    auto gameName = pTxtName->textComponent.text;
    std::string folderName = getGameFolderName(gameName);
    if (pTxtPath->textComponent.text.find_first_of('\\') != std::string::npos)
    {
        pLblFullPath->textComponent.text = pTxtPath->textComponent.text + "\\" + folderName + "\\";
    }
    else
    {
        pLblFullPath->textComponent.text = pTxtPath->textComponent.text + "/" + folderName + "/";
    }
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
    auto path = onut::showOpenFolderDialog("Game Path", pTxtPath->textComponent.text);
    if (path != "")
    {
        pTxtPath->textComponent.text = path;
        updateFullPath();
    }
}

void onCancel(const OUIControlRef&, const onut::UIMouseEvent&)
{
    pCtlMain->isVisible = true;
    pCtlCreateGame->isVisible = false;
    startAnimations();
}

void onCreate(const OUIControlRef&, const onut::UIMouseEvent&)
{
    std::string path = pTxtPath->textComponent.text;
    std::string fullPath = pLblFullPath->textComponent.text;
    std::string gameName = pTxtName->textComponent.text;
    std::string gameFolderName = getGameFolderName(gameName);

    if (path.empty())
    {
        onut::showMessageBox(
            "Error Creating game", 
            "Path is empty",
            onut::MessageBoxType::Ok,
            onut::MessageBoxLevel::Error);
        return;
    }

    if (gameName.empty() || gameFolderName.empty())
    {
        onut::showMessageBox(
            "Error Creating game",
            "Game Name is empty or invalid",
            onut::MessageBoxType::Ok,
            onut::MessageBoxLevel::Error);
        return;
    }

    // Create paths
    if (!onut::createFolder(fullPath))
    {
        onut::showMessageBox(
            "Error Creating game", 
            "Error creating path:\n" + fullPath,
            onut::MessageBoxType::Ok,
            onut::MessageBoxLevel::Error);
        return;
    }
    if (!onut::createFolder(fullPath + ".vscode"))
    {
        onut::showMessageBox(
            "Error Creating game", 
            "Error creating folder: .vscode",
            onut::MessageBoxType::Ok,
            onut::MessageBoxLevel::Error);
        return;
    }
    if (!onut::createFolder(fullPath + "game"))
    {
        onut::showMessageBox(
            "Error Creating game", 
            "Error creating folder: game",
            onut::MessageBoxType::Ok,
            onut::MessageBoxLevel::Error);
        return;
    }
    if (!onut::createFolder(fullPath + "game/assets"))
    {
        onut::showMessageBox(
            "Error Creating game", 
            "Error creating folder: assets",
            onut::MessageBoxType::Ok,
            onut::MessageBoxLevel::Error);
        return;
    }
    if (!onut::createFolder(fullPath + "game/assets/fonts"))
    {
        onut::showMessageBox(
            "Error Creating game", 
            "Error creating folder: fonts",
            onut::MessageBoxType::Ok,
            onut::MessageBoxLevel::Error);
        return;
    }
    if (!onut::createFolder(fullPath + "game/assets/scripts"))
    {
        onut::showMessageBox(
            "Error Creating game", 
            "Error creating folder: scripts",
            onut::MessageBoxType::Ok,
            onut::MessageBoxLevel::Error);
        return;
    }
    if (!onut::createFolder(fullPath + "typings"))
    {
        onut::showMessageBox(
            "Error Creating game", 
            "Error creating folder: typings",
            onut::MessageBoxType::Ok,
            onut::MessageBoxLevel::Error);
        return;
    }

    // Copy files
    if (!onut::copyFile("template/jsconfig.json", fullPath + "jsconfig.json"))
    {
        onut::showMessageBox(
            "Error Creating game", 
            "Error copying file: jsconfig.json",
            onut::MessageBoxType::Ok,
            onut::MessageBoxLevel::Error);
        return;
    }
    if (!onut::copyFile("template/typings/onut.d.ts", fullPath + "typings/onut.d.ts"))
    {
        onut::showMessageBox(
            "Error Creating game", 
            "Error copying file: onut.d.ts",
            onut::MessageBoxType::Ok,
            onut::MessageBoxLevel::Error);
        return;
    }
    if (!onut::copyFile("template/game/assets/fonts/font.fnt", fullPath + "game/assets/fonts/font.fnt"))
    {
        onut::showMessageBox(
            "Error Creating game", 
            "Error copying file: font.fnt",
            onut::MessageBoxType::Ok,
            onut::MessageBoxLevel::Error);
        return;
    }
    if (!onut::copyFile("template/game/assets/fonts/font.png", fullPath + "game/assets/fonts/font.png"))
    {
        onut::showMessageBox(
            "Error Creating game", 
            "Error copying file: font.png",
            onut::MessageBoxType::Ok,
            onut::MessageBoxLevel::Error);
        return;
    }
    if (!onut::copyFile("template/game/assets/scripts/main.js", fullPath + "game/assets/scripts/main.js"))
    {
        onut::showMessageBox(
            "Error Creating game", 
            "Error copying file: main.js",
            onut::MessageBoxType::Ok,
            onut::MessageBoxLevel::Error);
        return;
    }
    if (!onut::copyFile("template/game/GameNameWin32.exe", fullPath + "game/" + gameFolderName + "Win32.exe"))
    {
        onut::showMessageBox(
            "Error Creating game", 
            "Error copying file: GameNameWin32.exe",
            onut::MessageBoxType::Ok,
            onut::MessageBoxLevel::Error);
        return;
    }
    if (!onut::copyFile("template/game/GameNameOSX", fullPath + "game/" + gameFolderName + "OSX"))
    {
        onut::showMessageBox(
            "Error Creating game", 
            "Error copying file: GameNameOSX",
            onut::MessageBoxType::Ok,
            onut::MessageBoxLevel::Error);
        return;
    }
    system(("chmod +x " + fullPath + "game/" + gameFolderName + "OSX").c_str());
    if (!onut::copyFile("template/game/GameNameLinux", fullPath + "game/" + gameFolderName + "Linux"))
    {
        onut::showMessageBox(
            "Error Creating game", 
            "Error copying file: GameNameLinux",
            onut::MessageBoxType::Ok,
            onut::MessageBoxLevel::Error);
        return;
    }

    // Create custom files
    if (!onut::createTextFile(fullPath + ".vscode/tasks.json",
"{\n\
    \"version\": \"2.0.0\",\n\
    \"tasks\": [\n\
        {\n\
            \"taskName\": \"" + gameFolderName + "\",\n\
            \"type\": \"shell\",\n\
            \"windows\": {\n\
                \"command\": \".\\\\game\\\\" + gameFolderName + "Win32.exe .\\\\game\\\\\"\n\
            },\n\
            \"osx\": {\n\
                \"command\": \"./game/" + gameFolderName + "OSX ./game/\"\n\
            },\n\
            \"linux\": {\n\
                \"command\": \"./game/" + gameFolderName + "Linux ./game/\"\n\
            },\n\
            \"group\": \"test\",\n\
            \"presentation\": {\n\
                \"reveal\": \"always\",\n\
                \"panel\": \"shared\"\n\
            }\n\
        }\n\
    ]\n\
}"))
    {
        onut::showMessageBox(
            "Error Creating game", 
            "Error creating file: tasks.json",
            onut::MessageBoxType::Ok,
            onut::MessageBoxLevel::Error);
        return;
    }
    if (!onut::createTextFile(fullPath + "game/settings.txt",
"# Windowed resolution\n\
resolution = 1024 x 600\n\
\n\
# Retro resolution if retro mode is enabled\n\
retroResolution = 256 x 240\n\
\n\
# Retro mode renders the game in a smaller resolution and fill the screen\n\
retroMode = false\n\
\n\
# Name of the game to sbe displayed in the window's title bar\n\
gameName = " + gameName + "\n\
\n\
# If the window is resizable and can be maximized\n\
resizableWindow = true\n\
\n\
# Run the game in full screen.This is borderless fullscreen taking up your desktrop resolution\n\
fullscreen = false\n\
\n\
# Display FPS in the upper left corner\n\
showFPS = true\n\
\n\
# Fixed step update.Games where simulation or physic is important should use that\n\
fixedStep = true\n\
\n\
# The number of frames per second to update if fixed step is enabled\n\
updateFPS = 60\n\
\n\
# Braincloud app application ID\n\
appID = null\n\
\n\
# Braincloud app secret\n\
appSecret = null\n\
"))
    {
        onut::showMessageBox(
            "Error Creating game",
            "Error creating file: settings.txt",
            onut::MessageBoxType::Ok,
            onut::MessageBoxLevel::Error);
        return;
    }

    // Now open explorer in the folder
    onut::showInExplorer(fullPath);

    OQuit();
}

void onNameChanged(const OUITextBoxRef&, const onut::UITextBoxEvent&)
{
    updateFullPath();
}

void initSettings()
{
    oSettings->setResolution({ 600, 400 });
    oSettings->setGameName("Oak Nut Setup");
    oSettings->setIsResizableWindow(false);
    oSettings->setIsEditorMode(true);
    oSettings->setShowFPS(false);
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
    pTxtName->onTextChanged = onNameChanged;

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
