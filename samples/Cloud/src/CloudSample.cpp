// Required for WinMain
#include <Windows.h>

// Oak Nut include
#include <onut/Cloud.h>
#include <onut/Crypto.h>
#include <onut/onut.h>
#include <onut/Renderer.h>
#include <onut/Settings.h>
#include <onut/SpriteBatch.h>
#include <onut/Texture.h>
#include <onut/Timing.h>
#include <onut/UIContext.h>
#include <onut/UIControl.h>
#include <onut/UIImage.h>
#include <onut/UILabel.h>
#include <onut/UIPanel.h>
#include <onut/UITextBox.h>

void init();
void update();
void render();
void showLogin();
void showMainMenu();

enum GameState
{
    LoginPage,
    MainMenu,
    Game,
    GameOver
};

GameState gameState = GameState::LoginPage;

// Main
int CALLBACK WinMain(HINSTANCE appInstance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdCount)
{
    // Those two show up on your game Core App Info on the BrainCloud portal.
    // Visit getbraincloud.com to register a game.
    oSettings->setAppId("10646");
    oSettings->setAppSecret("f4c4a424-c888-4641-af7d-4c2d51195f7d");

    oSettings->setGameName("Cloud Sample");
    ORun(init, update, render);
}

static void onQuitClicked(const OUIControlRef&, const onut::UIMouseEvent&)
{
    OQuit();
}

static void onLoginResult(const OCloud::LoginEvent& event)
{
    OFindUI("logingInDialog")->isVisible = false;
    if (!event.success)
    {
        OFindUI("errorDialog")->isVisible = true;
        return;
    }
    showMainMenu();
}

static void onCreateAccountResult(const OCloud::LoginEvent& event)
{
    OFindUI("logingInDialog")->isVisible = false;
    if (!event.success)
    {
        auto pErr = OFindUI<OUILabel>("lblCreateAccountErrorMsg");
        pErr->textComponent.text = event.errorMessage;
        pErr->isVisible = true;
        return;
    }
    showMainMenu();
}

static void onLoginClicked(const OUIControlRef&, const onut::UIMouseEvent&)
{
    auto email = OFindUI<OUITextBox>("txtEmail")->textComponent.text;
    auto password = OFindUI<OUITextBox>("txtPassword")->textComponent.text;

    if (email.empty() || password.empty())
    {
        OFindUI("errorDialog")->isVisible = true;
        return;
    }

    oSettings->setUserSetting("email", email);
    oSettings->setUserSetting("password", password); // Plain text, not very safe. For example's sake

    // Proceed with login
    OFindUI("logingInDialog")->isVisible = true;
    oCloud->login(email, password, onLoginResult);
}

static void onCreateAccountClicked(const OUIControlRef&, const onut::UIMouseEvent&)
{
    OFindUI("createAccountDialog")->isVisible = true;
    OFindUI("lblCreateAccountErrorMsg")->isVisible = false;
}

static void onCreateAccountCancelClicked(const OUIControlRef&, const onut::UIMouseEvent&)
{
    OFindUI("createAccountDialog")->isVisible = false;
}

static void onCreateAccountCreateClicked(const OUIControlRef&, const onut::UIMouseEvent&)
{
    auto username = OFindUI<OUITextBox>("txtCreateAccountUsername")->textComponent.text;
    auto email = OFindUI<OUITextBox>("txtCreateAccountEmail")->textComponent.text;
    auto password = OFindUI<OUITextBox>("txtCreateAccountPassword")->textComponent.text;
    auto passwordRepeat = OFindUI<OUITextBox>("txtCreateAccountPasswordRepeat")->textComponent.text;

    if (username.empty() || email.empty() || password.empty() || passwordRepeat.empty())
    {
        auto pErr = OFindUI<OUILabel>("lblCreateAccountErrorMsg");
        pErr->textComponent.text = "Please fill all the fields.";
        pErr->isVisible = true;
        return;
    }

    if (password != passwordRepeat)
    {
        auto pErr = OFindUI<OUILabel>("lblCreateAccountErrorMsg");
        pErr->textComponent.text = "Passwords must match.";
        pErr->isVisible = true;
        return;
    }

    if (!OValidateEmail(email))
    {
        auto pErr = OFindUI<OUILabel>("lblCreateAccountErrorMsg");
        pErr->textComponent.text = "Email appears to be invalid.";
        pErr->isVisible = true;
        return;
    }

    OFindUI("logingInDialog")->isVisible = true;
    oCloud->createAccount(username, email, password, onCreateAccountResult);
}

static void onCloseErrorDialogClicked(const OUIControlRef&, const onut::UIMouseEvent&)
{
    OFindUI("errorDialog")->isVisible = false;
}

static void onAchievementsClicked(const OUIControlRef&, const onut::UIMouseEvent&)
{
    auto pAchievementDialog = OFindUI("achievementsDialog");
    auto pAchievementsContainer = pAchievementDialog->getChild("achievementsContainer");
    auto pTemplate = pAchievementsContainer->getChild("achievementCardTemplate");
    auto pSpinner = pAchievementDialog->getChild("spinner");

    pAchievementsContainer->removeAll();
    pAchievementsContainer->add(pTemplate);

    pSpinner->isVisible = true;
    pAchievementDialog->isVisible = true;

    oCloud->getAchievements([=](const OCloud::Achievements& achievements)
    {
        float yPos = pTemplate->rect.y;
        for (auto& achievement : achievements)
        {
            auto pCard = pTemplate->copy();
            pCard->rect.y = yPos;
            pCard->getChild<OUIImage>("imgIcon")->scale9Component.image.pTexture = achievement.iconTexture;
            pCard->getChild<OUILabel>("lblTitle")->textComponent.text = achievement.title;
            pCard->getChild<OUILabel>("lblDescription")->textComponent.text = achievement.description;
            pCard->isVisible = true;
            if (achievement.achieved) pCard->isEnabled = true;
            pAchievementsContainer->add(pCard);
            yPos += pCard->rect.w;
        }
        pSpinner->isVisible = false;
    });
}

static void onAchievementsCloseClicked(const OUIControlRef&, const onut::UIMouseEvent&)
{
    OFindUI("achievementsDialog")->isVisible = false;
}

static void onSignOut(const OUIControlRef&, const onut::UIMouseEvent&)
{
    oUI->removeAll();
    oCloud->logout([](const OCloud::LoginEvent& event)
    {
        showLogin();
    });
}

void init()
{
    // Add a style for the spinner
    oUIContext->addStyle<OUIPanel>("spinner", [](const OUIPanelRef& pControl, const Rect& rect)
    {
        // Use the user data to store the animation
        auto anim = reinterpret_cast<uintptr_t>(pControl->pUserData);
        auto animf = static_cast<float>(anim) / 100.f;
        animf += oTiming->getRenderDeltaTime() * 360.f;
        anim = static_cast<uintptr_t>(animf * 100.f);
        pControl->pUserData = reinterpret_cast<void*>(anim);

        // Draw the spinner
        auto pNut = OGetTexture("onutLogo.png");
        oSpriteBatch->drawSprite(pNut, rect.Center(), Color::White, animf, .5f);
    });

    showLogin();
}

static void showLogin()
{
    gameState = GameState::LoginPage;

    // Show the login dialog
    oUI->removeAll();
    OLoadUI("cloud.json");
    OFindUI<OUITextBox>("txtEmail")->textComponent.text = oSettings->getUserSetting("email");
    OFindUI<OUITextBox>("txtPassword")->textComponent.text = oSettings->getUserSetting("password");

    // Bind actions
    OFindUI("btnQuit")->onClick = onQuitClicked;
    OFindUI("btnLogin")->onClick = onLoginClicked;
    OFindUI("btnCloseErrorDialog")->onClick = onCloseErrorDialogClicked;
    OFindUI("btnCreateAccount")->onClick = onCreateAccountClicked;
    OFindUI("btnCreateAccountCancel")->onClick = onCreateAccountCancelClicked;
    OFindUI("btnCreateAccountCreate")->onClick = onCreateAccountCreateClicked;
}

static void showMainMenu()
{
    gameState = GameState::MainMenu;

    oUI->removeAll();
    OLoadUI("mainMenu.json");

    OFindUI<OUILabel>("lblWelcome")->textComponent.text = "Welcome ^999" + oCloud->getUsername();

    // Bind actions
    OFindUI("btnQuit")->onClick = onQuitClicked;
    OFindUI("btnAchievements")->onClick = onAchievementsClicked;
    OFindUI("btnCloseAchievements")->onClick = onAchievementsCloseClicked;
    OFindUI("btnSignOut")->onClick = onSignOut;
}

void update()
{
}

void render()
{
    // Clear
    oRenderer->clear(OColorHex(1d232d));
}
